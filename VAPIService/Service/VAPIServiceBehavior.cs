using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Headers;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using DataContracts;
using log4net;
using Newtonsoft.Json.Linq;
using RestSharp;
using VAPIService.Properties;
using WcfSvcBase;

namespace VAPIService
{
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Single, InstanceContextMode = InstanceContextMode.PerCall, UseSynchronizationContext = false)]
    public class VAPIServiceBehavior : IVAPIServiceContract
    {
        static ILog logger = LogManager.GetLogger(typeof(VAPIServiceBehavior));
        static Dictionary<string, string> algorithmMatch = new Dictionary<string, string>()
        { {"C", "Cognitec" },{ "N", "NewCognitec"},{"S", "Safr" },{ "F", "FaceMe"}, { "Q","Kist"}};
        static Dictionary<string, Algorithms_ast> algorithmsUtils = new Dictionary<string, Algorithms_ast>()
        { {"Cognitec", new Cognitec_94() }, { "NewCognitec", new Cognitec_96()}, { "Safr", new Safr()},{"FaceMe", new FaceMe() }, { "Kist", new Kist()} };
        static string MainAlgorithm = "NewCognitec";

        private void allowCORS()
        {
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Origin", "*");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Cache-Control", "no-cache, no-store");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Request-Methods", "GET, POST, OPTIONS");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Headers", "Origin, Content-Type, Accept");
        }

        public AddVideoResult AddVideo(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[AddVideo] {Utils.PrintDic(data)}");

                if (data.TryGetValue("video_id", out dynamic video_id) == false)
                    throw new Exception("param video_id not found");

                //data["video_id"] = AESUtil.Instance.Decrypt256(video_id);

                // 작업 수행
                data.Add("max_eye", "0.4");
                using (DBClient dbClient = new DBClient())
                    dbClient.AddVideo(data);

                HttpClient client = new HttpClient();
                MultipartFormDataContent form = new MultipartFormDataContent();
                //foreach (var item in data)
                //    form.Add(new StringContent(item.Value == null ? "" : $"{item.Value.ToString()}"), $"\"{item.Key}\"");
                form.Add(new StringContent(video_id), $"\"video_id\"");

                // 결과 생성
                var response = client.PostAsync($"{Properties.Settings.Default.apiHost}:9091/face/REST/analyze_video", form).Result;

                if (response.IsSuccessStatusCode)
                {
                    AddVideoResult result = response.Content.ReadAsAsync<AddVideoResult>().Result;
                    logger.Debug($"[AddVideo] {result}");

                    return result;
                }
                else
                {
                    AddVideoResult result = new AddVideoResult() { result_code = "F" };
                    logger.Debug($"[AddVideo] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[AddVideo] {ex.Message}");

                return AddVideoResult.Fail(ex.Message);
            }
        }

        public RemoveVideoResult RemoveVideo(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[RemoveVideo] {Utils.PrintDic(data)}");

                if (data.TryGetValue("video_id", out dynamic video_id) == false)
                    throw new Exception("param video_id not found");

                video_id = AESUtil.Instance.Decrypt256(video_id);

                // 작업 수행
                HttpClient client = new HttpClient();
                MultipartFormDataContent form = new MultipartFormDataContent();
                form.Add(new StringContent(video_id), $"\"video_id\"");

                // 결과 생성
                var response = client.PostAsync($"{Properties.Settings.Default.apiHost}:9093/face/REST/remove_video", form).Result;

                if (response.IsSuccessStatusCode)
                {
                    RemoveVideoResult result = response.Content.ReadAsAsync<RemoveVideoResult>().Result;
                    if (result.result_code == "S")
                    {
                        using (DBClient dbClient = new DBClient())
                        {
                            dbClient.UdtVideoUseYN(video_id, "N");
                            string path = dbClient.GetVideoPath(video_id);
                            if (!string.IsNullOrWhiteSpace(path) && File.Exists(path))
                            {
                                File.Delete(path);
                                logger.Debug($"[RemoveVideo] FileRemove Success path:[{path}]");
                            }
                            else
                            {
                                logger.Debug($"[RemoveVideo] FileRemove Fail path:[{path}]");

                                //return RemoveVideoResult.Fail($"[RemoveVideo] FileRemove Fail path:[{path}]"); //현재 파일이 없어도 db status 바꾸고 결과물은 Success
                            }
                            logger.Debug($"[RemoveVideo] {result}");
                        }

                        return result;
                    }
                    else
                    {
                        logger.Debug($"[RemoveVideo] {result}");

                        return result;
                    }
                }
                else
                {
                    RemoveVideoResult result = new RemoveVideoResult() { result_code = "F" };
                    logger.Debug($"[RemoveVideo] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[RemoveVideo] {ex.Message}");

                return RemoveVideoResult.Fail(ex.Message);
            }
        }

        public VanalListResult VanalList(string display, string page, string title, string reg_dt, string vanal_sts_cd)
        {
            try
            {
                allowCORS();

                // 입력 인자 정보 로깅
                logger.Info($"[VanalList] distplay[{display}] page[{page}] title[{title}] reg_dt[{reg_dt}] vanal_sts_cd[{vanal_sts_cd}]");

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    int total = dbClient.GetVanalListTotal(title, reg_dt, vanal_sts_cd);
                    var objects = new Collection<VanalListResult.VanalListObj>();
                    if (total > 0)
                    {
                        DataTable list = dbClient.GetVanalList(display, page, title, reg_dt, vanal_sts_cd);
                        if (list != null && list.Rows.Count > 0)
                        {
                            foreach (DataRow row in list.Rows)
                            {
                                string video_id = AESUtil.Instance.Encrypt256(DBNullCheck<string>(row["VIDEO_ID"]));
                                VanalListResult.VanalListObj obj = new VanalListResult.VanalListObj()
                                {
                                    video_id = video_id,                                    // 비디오 아이디
                                    url = $"/video/getMedia?video_id={video_id}",           // 비디오 URL
                                    title = DBNullCheck<string>(row["TITLE"]),              // 비디오 제목
                                    play_time = DBNullCheck<int>(row["PLAY_TIME"]),         // 비디오 길이 (초)
                                    status_cd = DBNullCheck<string>(row["VANAL_STS_CD"]),   // 분석 상태 코드 (R=분석 대기, P=분석 중, S=분석 완료)
                                    detect_cnt = DBNullCheck<int>(row["DETECT_CNT"]),       // 검출 건수
                                    match_cnt = DBNullCheck<int>(row["MATCH_CNT"]),         // 매칭 건수
                                    reg_dt = DBNullCheck<string>(row["REG_DT"]),            // 비디오 등록 일시
                                    reg_user = DBNullCheck<string>(row["REG_USER"])         // 등록 유저
                                };
                                obj.setting_status_txt(obj.status_cd, DBNullCheck<int>(row["JOB_PRCNT"]));

                                objects.Add(obj);
                            }
                        }
                    }

                    //결과 생성
                    var result = new VanalListResult()
                    {
                        page = Convert.ToInt32(page),
                        total_cnt = total,
                        videos = objects
                    };

                    logger.Debug($"[VanalList] {result}");
                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[VanalList] {ex.Message}");

                var result = new VanalListResult();

                //logger.Debug($"[VanalList] {result}");

                return result;
            }
        }

        public VanalRsltResult VanalRslt(string video_id, string display, string page)
        {
            try
            {
                allowCORS();

                // 입력 인자 정보 로깅
                logger.Info($"[VanalRslt] video_id[{video_id}] display[{display}] page[{page}]");
                video_id = AESUtil.Instance.Decrypt256(video_id);

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    int total = dbClient.GetVanalRsltListTotal(video_id);
                    logger.Debug($"GetVanalRsltListTotal({video_id})");

                    var objects = new Collection<VanalRsltResult.VanalRsltObj>();
                    if (total > 0)
                    {
                        DataTable list = dbClient.GetVanalRsltList(video_id, display, page);
                        logger.Debug($"GetVanalRsltList({video_id}, {display}, {page})");
                        if (list != null && list.Rows.Count > 0)
                        {
                            string apiHost = Properties.Settings.Default.apiHost;
                            foreach (DataRow row in list.Rows)
                            {
                                string crop_id = AESUtil.Instance.Encrypt256(DBNullCheck<string>(row["CROP_ID"]));
                                VanalRsltResult.VanalRsltObj obj = new VanalRsltResult.VanalRsltObj()
                                {
                                    crop_id = crop_id,                                      // 결과 이미지 아이디
                                    image_url = $"/video/image?id={crop_id}",               // 크롭 이미지 URL
                                    //snapshot_url = $"{apiHost}:9090/face/REST/image?id={DBNullCheck<string>(row["SNAPSHOT_ID"])}", // 스냅샷 이미지 URL
                                    detected_fr = DBNullCheck<double>(row["DETECTED_FR"]),  // 결과 이미지가 검출된 시각 (from, 초)
                                    detected_to = DBNullCheck<double>(row["DETECTED_TO"]),  // 결과 이미지가 검출된 시각 (to, 초)
                                    bounds = DBNullCheck<string>(row["BOUNDS"]),            // 스냅샷 이미지 기준 크롭 이미지의 박스 좌표 (x,y,width,height)
                                    score = DBNullCheck<float>(row["SCORE"]),               // 매칭 최고 점수
                                    match_cnt = DBNullCheck<int>(row["MATCH_CNT"])          // 매칭 건수
                                };

                                objects.Add(obj);
                            }
                        }
                    }

                    //결과 생성
                    var result = new VanalRsltResult()
                    {
                        page = Convert.ToInt32(page),
                        total_cnt = total,
                        rslt = objects
                    };

                    logger.Debug($"[VanalRslt] {result}");
                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[VanalRslt] {ex.Message}");

                var result = new VanalRsltResult();

                //logger.Debug($"[VanalRslt] {result}");

                return result;
            }
        }

        public VsearchResult Vsearch(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[Vsearch] {Utils.PrintDic(data)}");

                if (data.TryGetValue("image_id", out dynamic image_id) == false)
                    throw new Exception("param image_id not found");

                data["image_id"] = AESUtil.Instance.Decrypt256(image_id);

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    var vsearchReqId = dbClient.GetVsearchReqId();
                    data.Add("vsearch_req_id", vsearchReqId);
                    dbClient.InsertVsearchReq(data);

                    HttpClient client = new HttpClient();
                    MultipartFormDataContent form = new MultipartFormDataContent();
                    form.Add(new StringContent(vsearchReqId), $"\"vsearch_id\"");

                    var response = client.PostAsync($"{Properties.Settings.Default.apiHost}:9093/face/REST/search_video", form).Result;

                    SearchVideoResult responseRslt;
                    if (response.IsSuccessStatusCode)
                    {
                        responseRslt = response.Content.ReadAsAsync<SearchVideoResult>().Result;
                        logger.Debug($"[Vsearch] RES : {responseRslt}");
                    }
                    else
                    {
                        responseRslt = SearchVideoResult.Fail("response fail");
                        logger.Debug($"[Vsearch] RES : {responseRslt}");
                    }

                    //결과 생성
                    if (responseRslt.result_code == "S")
                    {
                        DataTable list = dbClient.GetVsearchRslt(vsearchReqId);
                        var result = new VsearchResult()
                        {
                            result_code = "S",
                            vsearch_id = AESUtil.Instance.Encrypt256(responseRslt.vsearch_id)
                        };

                        foreach (DataRow row in list.Rows)
                        {
                            string video_id = AESUtil.Instance.Encrypt256(DBNullCheck<string>(row["VIDEO_ID"]));
                            VsearchResult.VsearchResultObj obj = new VsearchResult.VsearchResultObj()
                            {
                                video_id = video_id,
                                title = DBNullCheck<string>(row["TITLE"]),
                                url = $"/video/getMedia?video_id={video_id}",
                                reg_dt = DBNullCheck<string>(row["REG_DT"]),
                                detect_cnt = DBNullCheck<int>(row["DETECT_CNT"])
                            };

                            result.rslt.Add(obj);
                        }

                        logger.Debug($"[Vsearch] {result}");
                        return result;
                    }
                    else
                    {
                        var result = VsearchResult.Fail(responseRslt.result_msg);
                        logger.Debug($"[Vsearch] {result}");

                        return result;
                    }
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Vsearch] {ex.Message}");
                var result = VsearchResult.Fail(ex.Message);

                return result;
            }
        }

        public VsearchDtlResult VsearchDtl(string vsearch_id, string video_id)
        {
            try
            {
                allowCORS();

                // 입력 인자 정보 로깅
                logger.Info($"[VsearchDtl] vsearch_id:[{vsearch_id}] video_id:[{video_id}]");

                vsearch_id = AESUtil.Instance.Decrypt256(vsearch_id);
                logger.Info($"[VsearchDtl] vsearch_id:[{vsearch_id}]");
                video_id = AESUtil.Instance.Decrypt256(video_id);
                logger.Info($"[VsearchDtl] video_id:[{video_id}]");

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    var objects = new Collection<VsearchDtlResult.VsearchDtlResultObj>();
                    DataTable list = dbClient.GetVsearchDtlRslt(vsearch_id, video_id);
                    logger.Debug($"GetVsearchDtlRslt({vsearch_id}, {video_id})");
                    if (list != null && list.Rows.Count > 0)
                    {
                        string apiHost = Properties.Settings.Default.apiHost;
                        foreach (DataRow row in list.Rows)
                        {
                            string crop_id = AESUtil.Instance.Encrypt256(DBNullCheck<string>(row["CROP_ID"]));
                            VsearchDtlResult.VsearchDtlResultObj obj = new VsearchDtlResult.VsearchDtlResultObj()
                            {
                                crop_id = crop_id,                         // 결과 이미지 아이디
                                image_url = $"/video/image?id={crop_id}",  // 크롭 이미지 URL
                                detected_fr = DBNullCheck<double>(row["DETECTED_FR"]),                                              // 결과 이미지가 검출된 시각 (from, 초)
                                detected_to = DBNullCheck<double>(row["DETECTED_TO"]),                                              // 결과 이미지가 검출된 시각 (to, 초)
                                score = DBNullCheck<float>(row["SCORE"]),                                                           // 매칭 최고 점수
                                bounds = DBNullCheck<string>(row["BOUNDS"])
                            };
                            objects.Add(obj);
                        }
                    }

                    //결과 생성
                    var result = new VsearchDtlResult()
                    {
                        matches = objects
                    };

                    logger.Debug($"[VsearchDtl] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[VsearchDtl] {ex.Message}");

                var result = new VsearchDtlResult();

                return result;
            }
        }

        public Stream GetImage(string face_id)
        {
            MemoryStream ms = null;
            try
            {
                allowCORS();


                // 입력 인자 정보 로깅
                logger.Info($"[GetImage] face_id[{face_id}]");

                if (string.IsNullOrWhiteSpace(face_id))
                    throw new ArgumentException("인수가 null 이거나 비어 있습니다.", "face_id");
                face_id = AESUtil.Instance.Decrypt256(face_id);

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    byte[] imgData = dbClient.GetImgData(face_id);

                    if (imgData == null)
                    {
                        logger.Error("[GetImage] Error : Image Data is Null (Check IMAGE_ID)");
                        return null;
                    }

                    //결과 생성
                    ms = new MemoryStream(imgData);
                    ms.Position = 0;

                    WebOperationContext.Current.OutgoingResponse.ContentType = "image/jpeg";
                    WebOperationContext.Current.OutgoingResponse.ContentLength = ms.Length;

                    logger.InfoFormat($"[GetImage] Success : face_id[{face_id}]");

                    return ms;
                }
            }
            catch (Exception ex)
            {
                logger.ErrorFormat($"[GetImage] Error : {ex.Message}");

                ms?.Dispose();
                ms = null;

                return ms;
            }
        }

        public IdentifyResult Identify(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[Identify] {Utils.PrintDic(data)}");

                if (data.TryGetValue("image_id", out dynamic image_id_dyn) == false)
                    throw new Exception("param image_id not found");

                //data["image_id"] = AESUtil.Instance.Decrypt256(image_id);
                string image_id = AESUtil.Instance.Decrypt256(image_id_dyn);

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    HttpClient client = new HttpClient();
                    MultipartFormDataContent form_frSvc = new MultipartFormDataContent();
                    form_frSvc.Add(new StringContent(image_id), $"\"image_id\"");

                    bool isOld = false;

                    if (image_id.StartsWith("C") || image_id.Length == 20)
                        isOld = true;

                    var response_frSvc = client.PostAsync($@"{(isOld?
                        Properties.Settings.Default.C94FrHelperAddress:
                        Properties.Settings.Default.C96FrHelperAddress)
                        }/face/REST/update_fir", form_frSvc).Result;

                    IdentifyResult responseRslt;
                    if (response_frSvc.IsSuccessStatusCode)
                    {
                        var result_FrSvc = response_frSvc.Content.ReadAsAsync<ApiResult>().Result;
                        logger.Debug($"[Identify] RES_FR : {result_FrSvc}");

                        if (result_FrSvc.result_code == "S")
                        {
                            var dbscan_id = dbClient.GetIdentifyReqId();
                            dbClient.InsertDBScanReq(dbscan_id, "G000003", image_id, "0.1", isOld? null : Settings.Default.CognitecVersion);
                            MultipartFormDataContent form_DBScanSvc = new MultipartFormDataContent();
                            form_DBScanSvc.Add(new StringContent(dbscan_id), $"\"dbscan_id\"");

                            var response_DBScanSvc = client.PostAsync($@"{(isOld?
                                Properties.Settings.Default.C94DBScanAddress:
                                Properties.Settings.Default.C96DBScanAddress)
                                }/face/REST/identify", form_DBScanSvc).Result;

                            if (response_DBScanSvc.IsSuccessStatusCode)
                            {
                                responseRslt = response_DBScanSvc.Content.ReadAsAsync<IdentifyResult>().Result;
                                logger.Debug($"[Identify] RES_DBSCAN : {responseRslt}");
                            }
                            else
                            {
                                responseRslt = IdentifyResult.Fail("response fail (DBSCAN)");
                                logger.Debug($"[Identify] RES_DBSCAN : {responseRslt}");
                            }
                        }
                        else
                        {
                            responseRslt = IdentifyResult.Fail(result_FrSvc.result_msg);
                            logger.Debug($"[Identify] RES_FR : {responseRslt}");
                        }
                    }
                    else
                    {
                        responseRslt = IdentifyResult.Fail("response fail (FR)");
                        logger.Debug($"[Identify] RES_FR : {responseRslt}");
                    }

                    //결과 생성
                    var result = responseRslt.result_code == "S" ? responseRslt : IdentifyResult.Fail(responseRslt.result_msg);
                    logger.Debug($"[Identify] {result}");

                    //암호화
                    foreach (var obj in result.matches)
                    {
                        var person_id = AESUtil.Instance.Encrypt256(obj.person_id);
                        obj.person_id = person_id;
                        obj.face_id = person_id;
                    }

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Identify] {ex.Message}");

                var result = IdentifyResult.Fail(ex.Message);

                return result;
            }
        }

        public PersonInfo GetPersonInfo(string person_id)   //미사용 중
        {
            try
            {
                allowCORS();

                // 입력 인자 정보 로깅
                logger.Info($"[GetPersonInfo] person_id:[{person_id}]");
                person_id = AESUtil.Instance.Decrypt256(person_id);

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    DataTable list = dbClient.GetPersonInfo(person_id);

                    //결과 생성
                    var result = new PersonInfo();
                    if (list != null && list.Rows.Count > 0)
                    {
                        DataRow row = list.Rows[0];
                        result.person_id = DBNullCheck<string>(row["TGTMNG_OBJ_ID"]);
                        result.face_id = DBNullCheck<string>(row["TGTMNG_OBJ_ID"]);
                        result.name = DBNullCheck<string>(row["EK_NM"]);
                        result.sex = DBNullCheck<string>(row["SEX_CD"]);
                        result.reg_emp_no = DBNullCheck<string>(row["REG_EMP_NO"]);
                    }

                    var encrypt_id = AESUtil.Instance.Encrypt256(result.person_id);
                    result.person_id = encrypt_id;
                    result.face_id = encrypt_id;

                    logger.Debug($"[GetPersonInfo] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[GetPersonInfo] {ex.Message}");

                var result = new PersonInfo();

                return result;
            }
        }

        public Groups GetGroups()
        {
            try
            {
                allowCORS();

                // 입력 인자 정보 로깅
                logger.Info($"[GetGroups]");

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    var objects = new Collection<Groups.GroupInfo>();
                    DataTable list = dbClient.GetGroups();
                    if (list != null && list.Rows.Count > 0)
                    {
                        foreach (DataRow row in list.Rows)
                        {
                            Groups.GroupInfo obj = new Groups.GroupInfo()
                            {
                                group_id = DBNullCheck<string>(row["TGT_MNGGRP_ID"]),
                                group_cd = DBNullCheck<string>(row["TGT_MNGGRP_CD"]),
                                status_cd = DBNullCheck<string>(row["MNGGRP_STS_CD"]),
                                name = DBNullCheck<string>(row["TGT_MNGGRP_NM"]),
                                count = DBNullCheck<int>(row["ENROLL_SUCSS_CNT"])
                            };

                            objects.Add(obj);
                        }
                    }

                    //결과 생성
                    var result = new Groups()
                    {
                        groups = objects
                    };

                    logger.Debug($"[GetGroups] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[GetGroups] {ex.Message}");

                var result = new Groups();

                return result;
            }
        }

        public PersonListResult GetPersonList(string group_cd)
        {
            try
            {
                allowCORS();

                // 입력 인자 정보 로깅
                logger.Info($"[GetPersonList] group_cd:[{group_cd}]");

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    var objects = new Collection<PersonInfo>();
                    DataTable list = dbClient.GetPersonList(group_cd);
                    if (list != null && list.Rows.Count > 0)
                    {
                        foreach (DataRow row in list.Rows)
                        {
                            PersonInfo obj = new PersonInfo()
                            {
                                person_id = DBNullCheck<string>(row["TGTMNG_OBJ_ID"]),
                                face_id = DBNullCheck<string>(row["TGTMNG_OBJ_ID"]),
                                name = DBNullCheck<string>(row["EK_NM"]),
                                sex = DBNullCheck<string>(row["SEX_CD"]),
                                reg_emp_no = DBNullCheck<string>(row["REG_EMP_NO"]),
                            };
                            objects.Add(obj);
                        }
                    }

                    //결과 생성
                    foreach (var obj in objects)
                    {
                        var person_id = AESUtil.Instance.Encrypt256(obj.person_id);
                        obj.person_id = person_id;
                        obj.face_id = person_id;
                    }

                    var result = new PersonListResult()
                    {
                        persons = objects
                    };

                    logger.Debug($"[GetPersonList] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[GetPersonList] {ex.Message}");

                var result = new PersonListResult();

                return result;
            }
        }

        public DetectResult Detect(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[Detect] {Utils.PrintDic(data)}");

                if (data.TryGetValue("image", out dynamic image) == false)
                    throw new Exception("param image not found");
                if (data.TryGetValue("min_eye_dist", out dynamic min_eye_dist) == false) min_eye_dist = "30";

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    var detect_id = dbClient.GetDetectReqId();

                    dbClient.InsertDetectReq(detect_id, image, min_eye_dist);

                    HttpClient client = new HttpClient();
                    MultipartFormDataContent form = new MultipartFormDataContent();
                    form.Add(new StringContent(detect_id), $"\"detect_id\"");

                    var response = client.PostAsync($"{Properties.Settings.Default.C96FrHelperAddress}/face/REST/detect", form).Result;

                    DetectResult responseRslt;
                    if (response.IsSuccessStatusCode)
                    {
                        responseRslt = response.Content.ReadAsAsync<DetectResult>().Result;
                        logger.Debug($"[Detect] RES : {responseRslt}");
                    }
                    else
                    {
                        responseRslt = DetectResult.Fail("response fail");
                        logger.Debug($"[Detect] RES : {responseRslt}");
                    }

                    //결과 생성
                    var result = responseRslt.result_code == "S" ? responseRslt : DetectResult.Fail(responseRslt.result_msg);

                    foreach (var obj in result.faces)
                    {
                        obj.face_id = AESUtil.Instance.Encrypt256(obj.face_id);
                    }

                    logger.Debug($"[Detect] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Detect] {ex.Message}");

                return DetectResult.Fail(ex.Message);
            }
        }

        public VerifyResult Verify(Stream stream)
        {
            try
            {
                allowCORS();
                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[Verify] {Utils.PrintDic(data)}");

                if (data.TryGetValue("face_id1", out dynamic face_id1) == false)
                    throw new Exception("param face_id1 not found");
                if (data.TryGetValue("face_id2", out dynamic face_id2) == false)
                    throw new Exception("param face_id2 not found");

                data["face_id1"] = AESUtil.Instance.Decrypt256(face_id1);
                data["face_id2"] = AESUtil.Instance.Decrypt256(face_id2);


                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    var verify_id = dbClient.GetVerifyId();

                    dbClient.InsertVerifyReq(verify_id, data["face_id1"], data["face_id2"], Settings.Default.CognitecVersion);

                    HttpClient client = new HttpClient();
                    MultipartFormDataContent form = new MultipartFormDataContent();
                    form.Add(new StringContent(verify_id), $"\"verify_id\"");

                    var response = client.PostAsync($"{Properties.Settings.Default.C96FrHelperAddress}/face/REST/verify", form).Result;

                    VerifyResult responseRslt;
                    if (response.IsSuccessStatusCode)
                    {
                        responseRslt = response.Content.ReadAsAsync<VerifyResult>().Result;
                        logger.Debug($"[Verify] RES : {responseRslt}");
                    }
                    else
                    {
                        responseRslt = VerifyResult.Fail("response fail");
                        logger.Debug($"[Verify] RES : {responseRslt}");
                    }

                    //결과 생성
                    var result = responseRslt.result_code == "S" ? responseRslt : VerifyResult.Fail(responseRslt.result_msg);
                    logger.Debug($"[Verify] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Verify] {ex.Message}");

                return VerifyResult.Fail(ex.Message);
            }
        }

        public CharacteristicsResult Characteristics(string face_id)
        {
            try
            {
                allowCORS();

                // 입력 인자 정보 로깅
                logger.Info($"[Characteristics] face_id:{face_id}");

                face_id = AESUtil.Instance.Decrypt256(face_id);

                // 작업 수행
                HttpClient client = new HttpClient();

                var response = client.GetAsync($"{Properties.Settings.Default.apiHost}:9094/face/REST/characteristics?face_id={face_id}").Result;

                CharacteristicsResult responseRslt;
                if (response.IsSuccessStatusCode)
                {
                    responseRslt = response.Content.ReadAsAsync<CharacteristicsResult>().Result;
                    logger.Debug($"[Characteristics] RES : {responseRslt}");
                }
                else
                {
                    responseRslt = CharacteristicsResult.Fail("response fail");
                    logger.Debug($"[Characteristics] RES : {responseRslt}");
                }

                //결과 생성
                var result = responseRslt.result_code == "S" ? responseRslt : CharacteristicsResult.Fail(responseRslt.result_msg);
                logger.Debug($"[CharacteristicsResult] {result}");

                return result;
            }
            catch (Exception ex)
            {
                logger.Error($"[CharacteristicsResult] {ex.Message}");

                return CharacteristicsResult.Fail(ex.Message);
            }
        }

        public AddPersonResult AddPerson(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[AddPerson] {Utils.PrintDic(data)}");

                if (data.TryGetValue("face_id", out dynamic face_id) == false)
                    throw new Exception("param face_id not found");
                if (data.TryGetValue("name", out dynamic name) == false)
                    throw new Exception("param name not found");
                if (data.TryGetValue("usr_id", out dynamic usr_id) == false) data.Add("usr_id", "admin");
                if (data.TryGetValue("sex", out dynamic sex) == false) data.Add("sex", "");
                if (data.TryGetValue("birth_ymd", out dynamic birth_ymd) == false) data.Add("birth_ymd", "");
                if (data.TryGetValue("etc", out dynamic etc) == false) data.Add("etc", "");
                if (data.TryGetValue("group_id", out dynamic group_id) == false) data.Add("group_id", "G000003");

                data["face_id"] = AESUtil.Instance.Decrypt256(face_id);

                // 작업 수행
                using (DBClient dbc = new DBClient())
                {
                    string tgtmng_obj_id = dbc.GetTargetId();
                    data.Add("tgtmng_obj_id", tgtmng_obj_id);

                    dbc.AddPerson(data);

                    HttpClient client = new HttpClient();
                    MultipartFormDataContent form = new MultipartFormDataContent();
                    form.Add(new StringContent(tgtmng_obj_id), "\"image_id\"");
                    form.Add(new StringContent("A"), "\"job_cd\"");

                    var response = client.PostAsync($"{Properties.Settings.Default.apiHost}:9092/face/REST/add_image", form).Result;

                    ApiResult responseRslt;
                    if (response.IsSuccessStatusCode)
                    {
                        responseRslt = response.Content.ReadAsAsync<ApiResult>().Result;
                        logger.Debug($"[AddPerson] RES : {responseRslt}");
                    }
                    else
                    {
                        responseRslt = ApiResult.Fail("response fail");
                        logger.Debug($"[AddPerson] RES : {responseRslt}");
                    }

                    //결과 생성
                    var result = responseRslt.result_code == "S" ? AddPersonResult.Success(AESUtil.Instance.Encrypt256(tgtmng_obj_id)) : AddPersonResult.Fail(responseRslt.result_msg);

                    logger.Debug($"[AddPerson] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[AddPerson] {ex.Message}");
                var result = AddPersonResult.Fail(ex.Message);

                return result;
            }
        }

        public ApiResult RemovePerson(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[RemovePerson] {Utils.PrintDic(data)}");

                if (data.TryGetValue("usr_id", out dynamic usr_id) == false)
                    throw new Exception("param usr_id not found");
                if (data.TryGetValue("person_id", out dynamic person_id) == false)
                    throw new Exception("param person_id not found");

                person_id = AESUtil.Instance.Decrypt256(person_id);

                // 작업 수행
                using (DBClient dbc = new DBClient())
                {
                    HttpClient client = new HttpClient();
                    MultipartFormDataContent form = new MultipartFormDataContent();
                    form.Add(new StringContent(person_id), "\"image_id\"");
                    form.Add(new StringContent("D"), "\"job_cd\"");

                    var response = client.PostAsync($"{Properties.Settings.Default.apiHost}:9092/face/REST/add_image", form).Result;

                    ApiResult responseRslt;
                    if (response.IsSuccessStatusCode)
                    {
                        responseRslt = response.Content.ReadAsAsync<ApiResult>().Result;
                        logger.Debug($"[RemovePerson] RES : {responseRslt}");

                        DataRow row = dbc.GetUserInfo(usr_id);
                        if (row == null) return ApiResult.Fail("User Not Found (Check usr_id)");

                        UserInfo user = new UserInfo()
                        {
                            usr_id = DBNullCheck<string>(row["USR_ID"]),
                            usr_nm = DBNullCheck<string>(row["USR_NM"]),
                            auth_id = DBNullCheck<string>(row["AUTH_ID"])
                        };

                        if (user.auth_id == "user")
                        {
                            dbc.RemovePerson(person_id, user.usr_id);
                        }
                        else if (user.auth_id == "admin")
                        {
                            dbc.RemovePerson(person_id);
                        }
                    }
                    else
                    {
                        responseRslt = ApiResult.Fail("response fail");
                        logger.Debug($"[RemovePerson] RES : {responseRslt}");
                    }

                    //결과 생성
                    var result = responseRslt.result_code == "S" ? ApiResult.Success : ApiResult.Fail(responseRslt.result_msg);

                    logger.Debug($"[RemovePerson] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[RemovePerson] {ex.Message}");
                var result = ApiResult.Fail(ex.Message);

                return result;
            }
        }

        public T DBNullCheck<T>(object data)
        {
            if (DBNull.Value == data)
            {
                if (typeof(T) == typeof(int))
                    return (T)Convert.ChangeType(0, typeof(int));
                else if (typeof(T) == typeof(string))
                    return (T)Convert.ChangeType("", typeof(string));
                else if (typeof(T) == typeof(double))
                    return (T)Convert.ChangeType(0, typeof(double));
                else if (typeof(T) == typeof(float))
                    return (T)Convert.ChangeType(0, typeof(float));
            }

            return (T)Convert.ChangeType(data, typeof(T));
        }

        public VerifyMultiResult VerifyMulti(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[VerifyMulti] {Utils.PrintDic(data)}");

                if (data.TryGetValue("face_id1", out dynamic face_id1) == false)
                    throw new Exception("param face_id1 not found");
                if (data.TryGetValue("face_id2", out dynamic face_id2) == false)
                    throw new Exception("param face_id2 not found");

                data["face_id1"] = AESUtil.Instance.Decrypt256(face_id1);
                data["face_id2"] = AESUtil.Instance.Decrypt256(face_id2);

                // 작업 수행
                VerifyMultiResult result = new VerifyMultiResult() { result_code = "S" };

                ManualResetEvent[] waitHandles = new ManualResetEvent[algorithmsUtils.Count];
                var names = algorithmsUtils.Keys.ToArray();
                //var utils = algorithmsUtils.Values.ToArray();

                for (int i = 0; i < waitHandles.Length; i++)
                {
                    var idx = i;
                    waitHandles[idx] = new ManualResetEvent(false);
                    ThreadPool.QueueUserWorkItem(delegate
                    {
                        string algothm_nm = names[idx];

                        try
                        {
                            Algorithms_ast algorithm = algorithmsUtils[algothm_nm];
                            string verify_id = algorithm.verify_id();

                            algorithm.InsertVerifyReq(verify_id, data["face_id1"], data["face_id2"]);

                            string url = algorithm.secondAddress + "/face/REST/verify";

                            Dictionary<string, string> formData = new Dictionary<string, string>();
                            formData.Add("verify_id", verify_id);

                            var response = PostRequest<VerifyResult>(url, formData);

                            if (response != null)
                            {
                                result.scores.Add(new VerifyMultiResult.ScoreInfo()
                                {
                                    algorithm = algothm_nm,
                                    score = response.score
                                });
                            }
                            else
                            {
                                result.scores.Add(new VerifyMultiResult.ScoreInfo()
                                {
                                    algorithm = algothm_nm,
                                    score = 0
                                });
                            }
                        }
                        catch (Exception ex)
                        {
                            logger.Error($"[VerifyMulti] Error algorithm[{algothm_nm}] message[{ex.Message}]");
                        }
                        finally
                        {
                            if (!result.scores.Any(x => x.algorithm == algothm_nm))
                            {
                                result.scores.Add(new VerifyMultiResult.ScoreInfo()
                                {
                                    algorithm = algothm_nm,
                                    score = 0
                                });
                            }

                            waitHandles[idx].Set();
                        }
                    });
                }

                WaitHandle.WaitAll(waitHandles);

                //result.scores = new List<VerifyMultiResult.ScoreInfo>(result.scores.OrderBy(x => x.algorithm).ToList());

                // kist 결과를 맨 뒤로 이동
                var kistScore = result.scores.Where(x => x.algorithm.Equals("Kist")).First();
                result.scores.Remove(kistScore);
                result.scores.Add(kistScore);

                //결과 생성
                logger.Debug($"[VerifyMulti] {result}");

                return result;
            }
            catch (Exception ex)
            {
                logger.Error($"[VerifyMulti] {ex.Message}");

                return new VerifyMultiResult() { result_code = "F", result_msg = ex.Message };
            }
        }

        public IdentifyMultiResult IdentifyMulti(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[IdentifyMulti] {Utils.PrintDic(data)}");

                if (data.TryGetValue("image_id", out dynamic image_id) == false)
                    throw new Exception("param image_id not found");

                //data["image_id"] = AESUtil.Instance.Decrypt256(image_id);
                image_id = AESUtil.Instance.Decrypt256(image_id);

                // 작업 수행
                IdentifyMultiResult result = new IdentifyMultiResult() { result_code = "S" };

                ManualResetEvent[] waitHandles = new ManualResetEvent[algorithmsUtils.Count];
                var keys = algorithmsUtils.Keys.ToArray();

                for (int i = 0; i < waitHandles.Length; i++)
                {
                    int idx = i;

                    waitHandles[idx] = new ManualResetEvent(false);

                    ThreadPool.QueueUserWorkItem(delegate
                    {
                        string algorithm_nm = keys[idx];
                        Algorithms_ast algorithm = algorithmsUtils[algorithm_nm];

                        try
                        {
                            ApiResult firUdtRslt = null;

                            if (algorithm_nm == "Cognitec" || algorithm_nm == "NewCognitec")
                            {
                                Dictionary<string, string> udtDictionary = new Dictionary<string, string>();
                                udtDictionary.Add("image_id", image_id);
                                string url = algorithm.secondAddress + "/face/REST/update_fir";

                                logger.Debug($"[updateFir REQ algorithm[{algorithm_nm}] {Utils.PrintDic(udtDictionary)}]");

                                firUdtRslt = PostRequest<ApiResult>(url, udtDictionary);

                                if (firUdtRslt == null)
                                {
                                    logger.Debug($"[updateFir request Fail algorithmList[{algorithm_nm}]]");
                                }
                                else if (firUdtRslt.result_code == "F")
                                {
                                    logger.Debug($"[updateFir Fail algorithmList[{algorithm_nm}] Message[{firUdtRslt.result_msg}]]");
                                }
                            }
                            else
                                firUdtRslt = ApiResult.Success;

                            if (firUdtRslt != null && firUdtRslt.result_code == "S")
                            {
                                Dictionary<string, string> dbscanDictionary = new Dictionary<string, string>();
                                var dbscan_id = algorithm.dbscan_id();
                                dbscanDictionary.Add("dbscan_id", dbscan_id);
                                algorithm.InsertDBScanReq(dbscan_id, "G000003", image_id, "0.1");

                                string dbscanUrl = algorithm.firstAddress + "/face/REST/identify";

                                logger.Debug($"[IdentifyMulti] REQ_DBSCAN algorithm[{algorithm_nm}] {Utils.PrintDic(dbscanDictionary)}");

                                var dbscanRslt = PostRequest<IdentifyResult>(dbscanUrl, dbscanDictionary);

                                if (dbscanRslt != null && dbscanRslt.result_code == "S")
                                {
                                    logger.Debug($"[IdentifyMulti] RES_DBSCAN Success algorithm[{algorithm_nm}]");

                                    var IdentifyInfo = new IdentifyMultiResult.IdentifyInfo() { algorithm = algorithm_nm };
                                    IdentifyInfo.matches = dbscanRslt.matches;

                                    foreach (var obj in IdentifyInfo.matches)
                                    {
                                        var person_id = AESUtil.Instance.Encrypt256(obj.person_id);
                                        obj.person_id = person_id;
                                        obj.face_id = person_id;
                                    }

                                    result.result.Add(IdentifyInfo);
                                }
                                else
                                {
                                    logger.Debug($"[IdentifyMulti] RES_DBSCAN Fail algorithm[{algorithm_nm}]");
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            logger.Error($"[IdentifyMulti] {ex.Message}");
                        }
                        finally
                        {
                            if (!result.result.Any(x => x.algorithm == algorithm_nm))
                                result.result.Add(new IdentifyMultiResult.IdentifyInfo() { algorithm = algorithm_nm });

                            waitHandles[idx].Set();
                        }
                    });
                }

                WaitHandle.WaitAll(waitHandles);

                result.result = new List<IdentifyMultiResult.IdentifyInfo>(result.result.OrderBy(x => x.algorithm).ToList());

                // kist 결과를 맨 뒤로 이동
                var kistScore = result.result.Where(x => x.algorithm.Equals("Kist")).First();
                result.result.Remove(kistScore);
                result.result.Add(kistScore);

                //결과 생성
                logger.Debug($"[IdentifyMulti] {result}");

                return result;
            }
            catch (Exception ex)
            {
                logger.Error($"[IdentifyMulti] {ex.Message}");

                var result = new IdentifyMultiResult() { result_code = "F", result_msg = ex.Message };

                return result;
            }
        }

        public ApiResult Test()
        {
            try
            {
                allowCORS();

                using (DBClient dbClient = new DBClient())
                {
                    DataTable dt = dbClient.GetTest();

                    Thread th = new Thread(new ThreadStart(delegate {
                        foreach (DataRow row in dt.Rows)
                        {
                            string obj_id = row["TGTMNG_OBJ_ID"] as string;
                            //byte[] image = row["IMG_DATA"] as byte[];
                            //
                            //string name = row["EK_NM"] == null ? "" : row["EK_NM"] as string;
                            //string sex = row["SEX_CD"] == null ? "" : row["SEX_CD"] as string;
                            //string birth_ymd = row["BIRTH_YMD"] == null ? "" : row["BIRTH_YMD"] as string;
                            //string etc = row["ETC_REF_DSC_01"] == null ? "" : row["ETC_REF_DSC_01"] as string;
                            //string info = PersonObject.Encode(name, sex, birth_ymd, null, null, etc);
                            //
                            //string api = $"http://{Settings.Default.SafrHost}:{Settings.Default.CoviPort}/people?merge=false&update=false&detect-mask=true&largest-only=true&max-occlusion=1&" +
                            //    $"detect-occlusion=true&min-cpq={Settings.Default.CenterPose}&min-fsq={Settings.Default.Sharpness}&min-fcq={Settings.Default.Contrast}&insert-profile=true";
                            //var safrClient = new RestClient(api) { Timeout = -1 };
                            //
                            //var request = new RestRequest(Method.POST);
                            //request.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
                            //request.AddHeader("X-RPC-DIRECTORY", "main");
                            //request.AddHeader("X-RPC-MIN-SIZE", "0");
                            //request.AddHeader("X-RPC-FACES-DETECT-THRESHOLD", "0");
                            //request.AddHeader("X-RPC-FACES-GROUPING-THRESHOLD", "0");
                            //request.AddHeader("X-RPC-EXTERNAL-ID", obj_id);
                            //request.AddHeader("X-RPC-PERSON-FIRST-NAME", info);
                            //request.AddHeader("Content-Type", "image/jpeg");
                            //request.AddParameter("image/jpeg", image, ParameterType.RequestBody);
                            //IRestResponse safrResponse = safrClient.Execute(request);
                            //
                            //if (safrResponse.StatusCode == System.Net.HttpStatusCode.Created)
                            //{
                            //    JObject jo = JObject.Parse(safrResponse.Content);
                            //    object safr_id = jo.SelectToken($"identifiedFaces[0].personId");
                            //
                            //    if (safr_id != null && !string.IsNullOrEmpty(safr_id.ToString()))
                            //    {
                            //        DBClient.Instance.SetSafrId(obj_id, safr_id.ToString());
                            //    }
                            //    else if (safr_id == null) 
                            //    {
                            //        using (MemoryStream ms = new MemoryStream(image))
                            //        using (MemoryStream ms2 = new MemoryStream())
                            //        {
                            //            Bitmap origin = new Bitmap(ms);
                            //            float ratio = 450f / origin.Width;
                            //            Bitmap resize = new Bitmap(origin, new Size((int)(origin.Width * ratio), (int)(origin.Height * ratio)));
                            //            resize.Save(ms2, System.Drawing.Imaging.ImageFormat.Jpeg);
                            //            byte[] image2 = ms2.ToArray();
                            //
                            //            var reClient = new RestClient(api) { Timeout = -1 };
                            //            var request2 = new RestRequest(Method.POST);
                            //            request2.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
                            //            request2.AddHeader("X-RPC-DIRECTORY", "main");
                            //            request2.AddHeader("X-RPC-MIN-SIZE", "114");
                            //            request2.AddHeader("X-RPC-FACES-DETECT-THRESHOLD", "0");
                            //            request2.AddHeader("X-RPC-FACES-GROUPING-THRESHOLD", "0");
                            //            request2.AddHeader("X-RPC-EXTERNAL-ID", obj_id);
                            //            request2.AddHeader("X-RPC-PERSON-FIRST-NAME", info);
                            //            request2.AddHeader("Content-Type", "image/jpeg");
                            //            request2.AddParameter("image/jpeg", image2, ParameterType.RequestBody);
                            //            IRestResponse safrResponse2 = reClient.Execute(request2);
                            //            if (safrResponse2.StatusCode == System.Net.HttpStatusCode.Created)
                            //            {
                            //                jo = JObject.Parse(safrResponse2.Content);
                            //                safr_id = jo.SelectToken($"identifiedFaces[0].personId");
                            //
                            //                if (safr_id != null && !string.IsNullOrEmpty(safr_id.ToString()))
                            //                    DBClient.Instance.SetSafrId(obj_id, safr_id.ToString());
                            //            }
                            //            origin.Dispose();
                            //            resize.Dispose();
                            //        }
                            //    }
                            //}

                            HttpClient client = new HttpClient();
                            MultipartFormDataContent form = new MultipartFormDataContent();
                            form.Add(new StringContent(obj_id), "\"image_id\"");
                            form.Add(new StringContent("A"), "\"job_cd\"");

                            var response = client.PostAsync($"{Properties.Settings.Default.KistApiAddress}/face/REST/add_image", form).Result;
                        }
                    }));
                    th.Start();

                    return ApiResult.Success;
                }
            }
            catch(Exception ex)
            {
                return ApiResult.Success;
            }
        }

        public T PostRequest<T>(string url, Dictionary<string, string> keyValues)
        {
            HttpClient client = new HttpClient();
            MultipartFormDataContent form = new MultipartFormDataContent();
            if(keyValues != null && keyValues.Count > 0)
            {
                foreach(var item in keyValues)
                    form.Add(new StringContent(item.Value), $"\"{item.Key}\"");
            }

            var response = client.PostAsync(url, form).Result;

            if (response.IsSuccessStatusCode)
            {
                logger.Debug($"RES Success url[{url}]");

                return response.Content.ReadAsAsync<T>().Result;
            }
            else
            {
                logger.Debug($"RES Fail url[{url}]");

                return default(T);
            }
        }

        public T GetRequest<T>(string url, Dictionary<string, string> keyValues)
        {
            HttpClient client = new HttpClient();

            if (keyValues != null && keyValues.Count > 0)
            {
                List<string> param = new List<string>();
                foreach(var item in keyValues)
                    param.Add($"{item.Key}={item.Value}");

                string paramStr = string.Join("&", param);
                url = string.Concat(url, "?", paramStr);
            }

            var response = client.GetAsync(url).Result;

            if (response.IsSuccessStatusCode)
            {
                logger.Debug($"RES Success url[{url}]");

                return response.Content.ReadAsAsync<T>().Result;
            }
            else
            {
                logger.Debug($"RES Fail url[{url}]");

                return default(T);
            }
        }

        public IdentifyMultiResult IdentifyMultiV2(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[IdentifyMulti] {Utils.PrintDic(data)}");

                if (data.TryGetValue("image_id", out dynamic image_id) == false)
                    throw new Exception("param image_id not found");
                if (data.TryGetValue("user_id", out dynamic user_id) == false)
                    throw new Exception("param user_id not found");

                //data["image_id"] = AESUtil.Instance.Decrypt256(image_id);
                image_id = AESUtil.Instance.Decrypt256(image_id);

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    UsrSettings user_settings = dbClient.GetUsrSettings(user_id, "MMI");
                    List<string> requireAlgorithms = GetAlgorithms_IdentifySetting(user_settings); // C, N, K ....
                    List<string> algorithmList = new List<string>();
                    for (int i = 0; i < requireAlgorithms.Count; i++)
                        algorithmList.Add(algorithmMatch[requireAlgorithms[i]]);

                    var keys = algorithmMatch.Keys.ToArray();
                    for (int i = 0; i < keys.Length; i++)
                        if (!requireAlgorithms.Contains(keys[i])) algorithmList.Add(algorithmMatch[keys[i]]);

                    IdentifyMultiResult result = new IdentifyMultiResult() { result_code = "S" };

                    //모드에 따른 필요 알고리즘들

                    ManualResetEvent[] waitHandles = new ManualResetEvent[algorithmList.Count];

                    for (int i = 0; i < algorithmList.Count; i++)
                    {
                        int idx = i;

                        waitHandles[idx] = new ManualResetEvent(false);

                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            string algorithm_nm = algorithmList[idx];
                            Algorithms_ast algorithm = algorithmsUtils[algorithm_nm];

                            try
                            {
                                ApiResult firUdtRslt = null;

                                if (algorithm_nm == "Cognitec" || algorithm_nm == "NewCognitec")
                                {
                                    Dictionary<string, string> udtDictionary = new Dictionary<string, string>();
                                    udtDictionary.Add("image_id", image_id);
                                    string url = algorithm.secondAddress + "/face/REST/update_fir";

                                    logger.Debug($"[updateFir REQ algorithm[{algorithm_nm}] {Utils.PrintDic(udtDictionary)}]");

                                    firUdtRslt = PostRequest<ApiResult>(url, udtDictionary);

                                    if (firUdtRslt == null)
                                    {
                                        logger.Debug($"[updateFir request Fail algorithmList[{algorithm_nm}]]");
                                    }
                                    else if (firUdtRslt.result_code == "F")
                                    {
                                        logger.Debug($"[updateFir Fail algorithmList[{algorithm_nm}] Message[{firUdtRslt.result_msg}]]");
                                    }
                                }
                                else
                                    firUdtRslt = ApiResult.Success;

                                if (firUdtRslt != null && firUdtRslt.result_code == "S") 
                                {
                                    Dictionary<string, string> dbscanDictionary = new Dictionary<string, string>();
                                    var dbscan_id = algorithm.dbscan_id();
                                    dbscanDictionary.Add("dbscan_id", dbscan_id);
                                    algorithm.InsertDBScanReq(dbscan_id, "G000003", image_id, "0.1");

                                    string dbscanUrl = algorithm.firstAddress + "/face/REST/identify";

                                    logger.Debug($"[IdentifyMulti] REQ_DBSCAN algorithm[{algorithm_nm}] {Utils.PrintDic(dbscanDictionary)}");

                                    var dbscanRslt = PostRequest<IdentifyResult>(dbscanUrl, dbscanDictionary);

                                    if (dbscanRslt != null && dbscanRslt.result_code == "S") 
                                    {
                                        logger.Debug($"[IdentifyMulti] RES_DBSCAN Success algorithm[{algorithm_nm}]");

                                        var IdentifyInfo = new IdentifyMultiResult.IdentifyInfo() { algorithm = algorithm_nm };
                                        IdentifyInfo.matches = dbscanRslt.matches;

                                        foreach (var obj in IdentifyInfo.matches)
                                        {
                                            var person_id = AESUtil.Instance.Encrypt256(obj.person_id);
                                            obj.person_id = person_id;
                                            obj.face_id = person_id;
                                        }

                                        result.result.Add(IdentifyInfo);
                                    }
                                    else
                                    {
                                        logger.Debug($"[IdentifyMulti] RES_DBSCAN Fail algorithm[{algorithm_nm}]");
                                    }
                                }
                            }
                            catch (Exception ex)
                            {
                                logger.Error($"[IdentifyMulti] {ex.Message}");
                            }
                            finally
                            {
                                if (!result.result.Any(x => x.algorithm == algorithm_nm))
                                    result.result.Add(new IdentifyMultiResult.IdentifyInfo() { algorithm = algorithm_nm });

                                waitHandles[idx].Set();
                            }
                        });
                    }

                    WaitHandle.WaitAll(waitHandles);

                    for (int i = requireAlgorithms.Count - 1; i > 0; i--) 
                    {
                        string algorithmsCd = requireAlgorithms[i];
                    
                        var score = result.result.Find(x => x.algorithm == algorithmMatch[algorithmsCd]);
                        result.result.Remove(score);
                        result.result.Insert(0, score);
                    }

                    Identify(result, user_settings);

                    //result.result = result.result.OrderByDescending(x => x.algorithm).ToList();

                    //결과 생성
                    logger.Debug($"[IdentifyMulti] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[IdentifyMulti] {ex.Message}");

                var result = new IdentifyMultiResult() { result_code = "F", result_msg = ex.Message };

                return result;
            }
        }

        public ApiResult AddPersonMulti(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[AddPersonMulti] {Utils.PrintDic(data)}");

                if (data.TryGetValue("face_id", out dynamic face_id) == false)
                    throw new Exception("param face_id not found");
                if (data.TryGetValue("name", out dynamic name) == false)
                    throw new Exception("param name not found");
                if (data.TryGetValue("usr_id", out dynamic usr_id) == false) data.Add("usr_id", "admin");
                if (data.TryGetValue("sex", out dynamic sex) == false) data.Add("sex", "");
                if (data.TryGetValue("birth_ymd", out dynamic birth_ymd) == false) data.Add("birth_ymd", "");
                if (data.TryGetValue("etc", out dynamic etc) == false) data.Add("etc", "");
                if (data.TryGetValue("group_id", out dynamic group_id) == false) data.Add("group_id", "G000003");

                data["face_id"] = AESUtil.Instance.Decrypt256(face_id);

                // 작업 수행
                using (DBClient dbc = new DBClient())
                {
                    string tgtmng_obj_id = dbc.GetTargetId();
                    data.Add("tgtmng_obj_id", tgtmng_obj_id);

                    dbc.AddPerson(data);

                    Dictionary<string, string> addDictionary = new Dictionary<string, string>();
                    addDictionary.Add("job_cd", "A");
                    addDictionary.Add("image_id", tgtmng_obj_id);

                    var firstAlgorithm = algorithmMatch["N"]; //현 cognitec 96
                    var url = algorithmsUtils[firstAlgorithm].firstAddress + "/face/REST/add_image";
                    var firstAddResponse = PostRequest<ApiResult>(url, addDictionary);
                    if (firstAddResponse != null && firstAddResponse.result_code == "S") 
                    {
                        logger.Debug($"[AddPersonMulti] firstAddPerson Success algorithm[{firstAlgorithm}]");

                        var restAlgorithms = algorithmsUtils.Where(x => x.Key != firstAlgorithm).ToArray();

                        ManualResetEvent[] waitHandles = new ManualResetEvent[restAlgorithms.Count()];

                        for (int i = 0; i < restAlgorithms.Count(); i++)
                        {
                            int idx = i;
                            waitHandles[idx] = new ManualResetEvent(false);

                            ThreadPool.QueueUserWorkItem(delegate 
                            {
                                var restAlgorithm = restAlgorithms[idx];
                                var restUrl = restAlgorithm.Value.firstAddress + "/face/REST/add_image";

                                try
                                {
                                    var restResponse = PostRequest<ApiResult>(restUrl, addDictionary);

                                    if (restResponse != null && restResponse.result_code == "S")
                                        logger.Debug($"[AddPersonMulti] RES Success algorithm[{restAlgorithm}]");
                                    else
                                        logger.Debug($"[AddPersonMulti] RES Fail algorithm[{restAlgorithm}]");
                                }
                                catch (Exception ex)
                                {
                                    logger.Error($"[AddPersonMulti] algorithm[{restAlgorithm}] Error[{ex.Message}]");
                                }
                                finally
                                {
                                    waitHandles[idx].Set();
                                }
                            });
                        }

                        WaitHandle.WaitAll(waitHandles);

                        logger.Debug($"[AddPersonMulti] {firstAddResponse}");

                        return firstAddResponse;
                    }
                    else
                    {
                        dbc.RemovePerson(tgtmng_obj_id);
                        logger.Debug($"[AddPersonMulti] firstAddPerson Fail algorithm[{algorithmMatch["N"]}]");

                        return firstAddResponse;
                    }
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[AddPersonMulti] {ex.Message}");
                var result = ApiResult.Fail(ex.Message);

                return result;
            }
        }

        public ApiResult RemovePersonMulti(Stream stream)
        {
            try
            {
                allowCORS();

                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[RemovePersonMulti] {Utils.PrintDic(data)}");

                if (data.TryGetValue("usr_id", out dynamic usr_id) == false)
                    throw new Exception("param usr_id not found");
                if (data.TryGetValue("person_id", out dynamic person_id) == false)
                    throw new Exception("param person_id not found");

                person_id = AESUtil.Instance.Decrypt256(person_id);

                // 작업 수행
                using (DBClient dbc = new DBClient())
                {
                    var restUrl = "/face/REST/add_image";

                    var url = algorithmsUtils[MainAlgorithm].firstAddress + restUrl;
                    Dictionary<string, string> dictionary = new Dictionary<string, string>();
                    dictionary.Add("job_cd", "D");
                    dictionary.Add("image_id", person_id);

                    var response = PostRequest<ApiResult>(url, dictionary);

                    if (response != null)
                    {
                        logger.Debug($"[RemovePersonMulti] MainAlgorithm[{MainAlgorithm}] RES : {response}");

                        if (response.result_code == "S")
                        {
                            DataRow row = dbc.GetUserInfo(usr_id);
                            if (row == null) return ApiResult.Fail("User Not Found (Check usr_id)");

                            UserInfo user = new UserInfo()
                            {
                                usr_id = DBNullCheck<string>(row["USR_ID"]),
                                usr_nm = DBNullCheck<string>(row["USR_NM"]),
                                auth_id = DBNullCheck<string>(row["AUTH_ID"])
                            };

                            if (user.auth_id == "user")
                            {
                                dbc.RemovePerson(person_id, user.usr_id);
                            }
                            else if (user.auth_id == "admin")
                            {
                                dbc.RemovePerson(person_id);
                            }

                            var restAlgorithms = algorithmsUtils.Where(x => x.Key != MainAlgorithm).ToArray();

                            ManualResetEvent[] waitHandles = new ManualResetEvent[restAlgorithms.Count()];
                            for (int i = 0; i < waitHandles.Length; i++)
                            {
                                int idx = i;
                                waitHandles[idx] = new ManualResetEvent(false);

                                ThreadPool.QueueUserWorkItem(delegate {
                                try
                                {
                                        var secondUrl = restAlgorithms[idx].Value.firstAddress + restUrl;
                                        var secondResponse = PostRequest<ApiResult>(secondUrl, dictionary);

                                        if (secondResponse != null)
                                        {
                                            if (secondResponse.result_code == "S") 
                                            {
                                                logger.Debug($"[RemovePersonMulti] Success algorithm[{restAlgorithms[idx].Key}]");
                                            }
                                            else
                                            {
                                                logger.Debug($"[RemovePersonMulti] Fail algorithm[{restAlgorithms[idx].Key}] message[{secondResponse.result_msg}]");
                                            }
                                        }
                                        else
                                        {
                                            logger.Debug($"[RemovePersonMulti] Res fail algorithm[{restAlgorithms[idx].Key}]");
                                        }
                                    }
                                    catch(Exception ex)
                                    {
                                        logger.Error($"[RemovePersonMulti] Error algorithm[{restAlgorithms[idx].Key}] Message[{ex.Message}]");
                                    }
                                    finally
                                    {
                                        waitHandles[idx].Set();
                                    }
                                });
                            }
                            WaitHandle.WaitAll(waitHandles);
                        }
                        else
                        {
                            logger.Debug($"[RemovePersonMulti] RES Fail MainAlgorithm[{MainAlgorithm}] Message[{response}]");
                        }

                        return response;
                    }
                    else
                    {
                        logger.Debug($"[RemovePersonMulti] REQ Fail MainAlgorithm[{MainAlgorithm}]");

                        return ApiResult.Fail($"[RemovePersonMulti] REQ Fail");
                    }
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[RemovePersonMulti] {ex.Message}");
                var result = ApiResult.Fail(ex.Message);

                return result;
            }
        }

        public VerifyMultiResult VerifyMultiV2(Stream stream)
        {
            try
            {
                allowCORS();
                // stream 파싱
                var data = Utils.ParseFormData(stream);

                // 입력 인자 정보 로깅
                logger.Info($"[VerifyMulti] {Utils.PrintDic(data)}");

                if (data.TryGetValue("face_id1", out dynamic face_id1) == false)
                    throw new Exception("param face_id1 not found");
                if (data.TryGetValue("face_id2", out dynamic face_id2) == false)
                    throw new Exception("param face_id2 not found");
                if (data.TryGetValue("user_id", out dynamic user_id) == false)
                    throw new Exception("param user_id not found");

                data["face_id1"] = AESUtil.Instance.Decrypt256(face_id1);
                data["face_id2"] = AESUtil.Instance.Decrypt256(face_id2);

                // 작업 수행
                using (DBClient dbClient = new DBClient())
                {
                    UsrSettings user_settings = dbClient.GetUsrSettings(user_id, "MMV");
                    List<string> requireAlgorithms = GetAlgorithms_VerifySetting(user_settings); // C, N, K ....
                    List<string> algorithmList = new List<string>();

                    for (int i = 0; i < requireAlgorithms.Count; i++)
                        algorithmList.Add(algorithmMatch[requireAlgorithms[i]]);

                    var keys = algorithmMatch.Keys.ToArray();
                    for (int i = 0; i < keys.Length; i++)
                        if (!requireAlgorithms.Contains(keys[i])) algorithmList.Add(algorithmMatch[keys[i]]);

                    VerifyMultiResult result = new VerifyMultiResult() { result_code = "S" };

                    ManualResetEvent[] waitHandles = new ManualResetEvent[algorithmList.Count()];
                    for (int i = 0; i < waitHandles.Length; i++)
                    {
                        var idx = i;
                        waitHandles[idx] = new ManualResetEvent(false);
                        ThreadPool.QueueUserWorkItem(delegate
                        {
                            string algothm_nm = algorithmList[idx];

                            try
                            {
                                Algorithms_ast algorithm = algorithmsUtils[algothm_nm];
                                string verify_id = algorithm.verify_id();

                                algorithm.InsertVerifyReq(verify_id, data["face_id1"], data["face_id2"]);

                                string url = algorithm.secondAddress + "/face/REST/verify";

                                Dictionary<string, string> formData = new Dictionary<string, string>();
                                formData.Add("verify_id", verify_id);

                                var response = PostRequest<VerifyResult>(url, formData);

                                if (response != null)
                                {
                                    result.scores.Add(new VerifyMultiResult.ScoreInfo()
                                    {
                                        algorithm = algothm_nm,
                                        score = response.score
                                    });
                                }
                                else
                                {
                                    result.scores.Add(new VerifyMultiResult.ScoreInfo()
                                    {
                                        algorithm = algothm_nm,
                                        score = 0
                                    });
                                }
                            }
                            catch(Exception ex)
                            {
                                logger.Error($"[VerifyMultiV2] Error algorithm[{algothm_nm}] message[{ex.Message}]");
                            }
                            finally
                            {
                                if (!result.scores.Any(x => x.algorithm == algothm_nm))
                                {
                                    result.scores.Add(new VerifyMultiResult.ScoreInfo()
                                    {
                                        algorithm = algothm_nm,
                                        score = 0
                                    });
                                }

                                waitHandles[idx].Set();
                            }
                        });
                    }

                    WaitHandle.WaitAll(waitHandles);

                    //for (int i = requireAlgorithms.Count - 1; i > 0; i--) 
                    //{
                    //    string algorithmsCd = requireAlgorithms[i];
                    //
                    //    var score = result.scores.Find(x => x.algorithm == algorithmMatch[algorithmsCd]);
                    //    result.scores.Remove(score);
                    //    result.scores.Insert(0, score);
                    //}

                    if (result.scores.First(x => x.algorithm == algorithmMatch[requireAlgorithms[0]]).score == 0)
                    {
                        result.result_code = "F";
                        result.result_msg = "first algorithm score error";

                        logger.Debug($"[VerifyMultiV2] score error {result}");

                        return result;
                    }

                    Verify(result, user_settings, algorithmList);

                    //result.scores = result.scores.OrderByDescending(x => x.algorithm).ToList();

                    //결과 생성
                    logger.Debug($"[VerifyMulti] {result}");

                    return result;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[VerifyMulti] {ex.Message}");

                return new VerifyMultiResult() { result_code = "F", result_msg = ex.Message };
            }
        }

        public List<string> GetAlgorithms_VerifySetting(UsrSettings settings)
        {
            List<string> list = new List<string>();

            string code = settings.jo["selected"].ToString();

            if (code == "mode1") 
            {
                list.Add(settings.jo["mode1"]["setting1"]["algorithm"].ToString());
                list.Add(settings.jo["mode1"]["setting3"]["algorithm"].ToString());
            }
            else if (code == "mode2")
            {
                list.Add(settings.jo["mode2"]["setting1"]["algorithm"].ToString());
                list.Add(settings.jo["mode2"]["setting2"]["algorithm"].ToString());
                list.Add(settings.jo["mode2"]["setting3"]["algorithm"].ToString());
            }
            else if (code == "mode3")
            {
                list.Add(settings.jo["mode3"]["setting1"]["algorithm"].ToString());
                list.Add(settings.jo["mode3"]["setting2"]["algorithm"].ToString());
                list.Add(settings.jo["mode3"]["setting3"]["algorithm"].ToString());
            }

            return list;
        }

        public List<string> GetAlgorithms_IdentifySetting(UsrSettings settings)
        {
            List<string> list = new List<string>();

            string code = settings.jo["selected"].ToString();

            if (code == "mode1")
            {
                list.Add(settings.jo["mode1"]["setting1"]["algorithm"].ToString());
                list.Add(settings.jo["mode1"]["setting2"]["algorithm"].ToString());
                list.Add(settings.jo["mode1"]["setting3"]["algorithm"].ToString());
            }

            return list;
        }

        public VerifyMultiResult Verify(VerifyMultiResult result, UsrSettings settings, List<string> requireAlgorithms)
        {
            var scores = result.scores;
            JObject jo = settings.jo;
            string mode = jo["selected"].ToString();
            List<VerifyMultiResult.ScoreInfo> scoreList = new List<VerifyMultiResult.ScoreInfo>(); //순서를 맞추기위함 (이전에 쓰레드 처리때문)

            for (int i = 0; i < requireAlgorithms.Count; i++) 
                scoreList.Add(scores.First(x => x.algorithm == requireAlgorithms[i]));

            bool accordCk = false; //일치 불일치 여부
            float avg = 0;

            if (mode == "mode1") 
            {
                float lower = Convert.ToSingle(jo["mode1"]["setting2"]["lower"]);
                float upper = Convert.ToSingle(jo["mode1"]["setting2"]["upper"]);

                bool firstTerms = false; // 첫번째 조건 판단 여부

                if (scoreList[0].score >= upper) 
                {
                    accordCk = true;
                    firstTerms = true;
                }
                else if (scoreList[0].score < lower)
                {
                    accordCk = false;
                    firstTerms = true;
                }
                else
                {
                    if (scoreList[1].score == 0)
                    {
                        result.result_code = "F";
                        result.result_msg = "second algorithm score error";
                        result.scores.Add(new VerifyMultiResult.ScoreInfo()
                        {
                            algorithm = "Avg",
                            score = 0
                        });

                        return result;
                    }
                    else
                    {
                        float avgThreshold = Convert.ToSingle(jo["mode1"]["setting4"]["avg"]);

                        if ((scoreList[0].score + scoreList[1].score) / 2 >= avgThreshold)
                            accordCk = true;
                        else
                            accordCk = false;
                    }
                }

                avg = firstTerms ? scoreList[0].score : (scoreList[0].score + scoreList[1].score) / 2;
            }
            else if (mode == "mode2") 
            {
                float avgThreshold = Convert.ToSingle(jo["mode2"]["setting4"]["avg"]);

                avg = (scoreList[0].score + scoreList[1].score + scoreList[2].score) / 3;

                if (avg >= avgThreshold)
                    accordCk = true;
            }
            else if (mode == "mode3") 
            {
                float avgThreshold1 = Convert.ToSingle(jo["mode3"]["setting4"]["avg"]);
                float avgThreshold2 = Convert.ToSingle(jo["mode3"]["setting5"]["avg"]);

                scoreList = scoreList.OrderByDescending(x => x.score).ToList();

                avg = (scoreList[0].score + scoreList[1].score) / 2;

                if (avg >= avgThreshold1)
                    accordCk = true;
                else
                {
                    avg = scoreList[0].score;
                    if (avg >= avgThreshold2)
                        accordCk = true;
                }
            }

            scoreList.Add(new VerifyMultiResult.ScoreInfo()
            {
                algorithm = "Avg",
                score = avg
            });

            result.scores = scoreList;

            result.result_msg = $"{(accordCk ? "일치" : "불일치")} : {GetScoreMesssage(result.scores)}";

            return result;
        }

        public string GetScoreMesssage(List<VerifyMultiResult.ScoreInfo> scores)
        {
            List<string> msgL = new List<string>();

            for (int i = 0; i < scores.Count; i++)
            {
                var item = scores[i];
                KeyValuePair<string, string> keyValue = algorithmMatch.FirstOrDefault(x => x.Value == item.algorithm);

                if (!keyValue.Equals(default(KeyValuePair<string, string>)))
                    msgL.Add($"{keyValue.Key}알고리즘({item.score})");
                else
                    msgL.Add($"최종점수({item.score})");
            }

            string result_scores = string.Join(",", msgL);

            return result_scores;
        }

        public IdentifyMultiResult Identify(IdentifyMultiResult result, UsrSettings settings)
        {
            var matches = result.result;
            JObject jo = settings.jo;
            string mode = jo["selected"].ToString();

            var firstAlgorithm = algorithmMatch.FirstOrDefault(x => x.Value == matches[0].algorithm).Key;
            var secondAlgorithm = algorithmMatch.FirstOrDefault(x => x.Value == matches[1].algorithm).Key;
            var thirdAlgorithm = algorithmMatch.FirstOrDefault(x => x.Value == matches[2].algorithm).Key;
            var rankCnt = Convert.ToInt32(jo["mode1"]["setting4"]["avg"]);

            for (int i = 0; i < matches.Count; i++)
                matches[i].matches = matches[i].matches.Take(rankCnt).ToList();

            var firstMatches = matches[0].matches;
            var secondMatches = matches[1].matches;
            var thirdMatches = matches[2].matches;

            var avgMatches = new List<IdentifyResult.MatchedFace>();
            for (int i = 0; i < firstMatches.Count; i++)
            {
                var clone = firstMatches[i].Clone();
                clone.algorithm = firstAlgorithm + "사";
                avgMatches.Add(clone);
            }

            if (mode == "mode1") 
            {
                for (int i = 0; i < secondMatches.Count; i++)
                {
                    var secondItem = secondMatches[i];
                    var finded = avgMatches.Find(x => x.face_id == secondItem.face_id);

                    if (finded != null)
                    {
                        finded.score += secondItem.score;
                        finded.algorithm += ", " + secondAlgorithm + "사";
                    }
                    else
                    {
                        var clone = secondItem.Clone();
                        clone.algorithm = secondAlgorithm + "사";
                        avgMatches.Add(clone);
                    }
                }

                for (int i = 0; i < thirdMatches.Count; i++)
                {
                    var thirdItem = thirdMatches[i];
                    var finded = avgMatches.Find(x => x.face_id == thirdItem.face_id);

                    if (finded != null)
                    {
                        finded.score += thirdItem.score;
                        finded.algorithm += ", " + thirdAlgorithm + "사";
                    }
                    else
                    {
                        var clone = thirdItem.Clone();
                        clone.algorithm = thirdAlgorithm + "사";
                        avgMatches.Add(clone);
                    }
                }
            }

            //string algorithmStr = firstAlgorithm + "사, " + secondAlgorithm + "사, " + thirdAlgorithm + "사";

            avgMatches = avgMatches.OrderByDescending(x => x.score).ToList();

            for (int i = 0; i < avgMatches.Count; i++)
            {
                //avgMatches[i].algorithm = algorithmStr;
                avgMatches[i].score /= 3;
            }

            matches.Add(new IdentifyMultiResult.IdentifyInfo()
            {
                algorithm = "Avg",
                matches = avgMatches
            });

            return result;
        }
    }
}
