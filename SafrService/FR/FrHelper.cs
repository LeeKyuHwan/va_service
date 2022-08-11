using DataContracts;
using log4net;
using Newtonsoft.Json.Linq;
using RestSharp;
using SafrService.Properties;
using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SafrService
{
    public class FrHelper
    {
        public static readonly FrHelper Instance = new FrHelper();
        private FrHelper() { }

        static ILog logger = LogManager.GetLogger(typeof(FrHelper));
        List<Person> gallery = new List<Person>();

        ReaderWriterLockSlim rwLock = new ReaderWriterLockSlim();

        public string Enroll(string image_id)
        {
            try
            {
                using (DBClient dbc = new DBClient())
                {
                    var image = dbc.GetImage(image_id);

                    PersonObject personObj = dbc.GetPersonObject(image_id);
                    string info = personObj != null ? PersonObject.Encode(personObj.Name, personObj.Sex, personObj.Birth, null, null, personObj.Etc) : "";

                    string api = $"http://{Settings.Default.SafrHost}:{Settings.Default.Covi}/people?merge=false&update=false&detect-mask=true&largest-only=true&" +
                        $"detect-occlusion=true&min-cpq={Settings.Default.CenterPose}&min-fsq={Settings.Default.Sharpness}&min-fcq={Settings.Default.Contrast}&insert-profile=true";
                    var safrClient = new RestClient(api) { Timeout = -1 };

                    var request = new RestRequest(Method.POST);
                    request.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
                    request.AddHeader("X-RPC-DIRECTORY", "main");
                    request.AddHeader("X-RPC-MIN-SIZE", "0");
                    request.AddHeader("X-RPC-FACES-DETECT-THRESHOLD", "0");
                    request.AddHeader("X-RPC-FACES-GROUPING-THRESHOLD", "0");
                    request.AddHeader("X-RPC-EXTERNAL-ID", image_id);
                    request.AddHeader("X-RPC-PERSON-FIRST-NAME", info);
                    request.AddHeader("Content-Type", "image/jpeg");
                    request.AddParameter("image/jpeg", image, ParameterType.RequestBody);
                    IRestResponse safrResponse = safrClient.Execute(request);

                    if (safrResponse.StatusCode == System.Net.HttpStatusCode.Created)
                    {
                        JObject jo = JObject.Parse(safrResponse.Content);
                        object safr_id = jo.SelectToken($"identifiedFaces[0].personId");

                        if (string.IsNullOrWhiteSpace(Convert.ToString(safr_id)))
                        {
                            using (MemoryStream ms = new MemoryStream(image))
                            using (MemoryStream ms2 = new MemoryStream())
                            {
                                Bitmap origin = new Bitmap(ms);
                                float ratio = 450f / origin.Width;
                                Bitmap resize = new Bitmap(origin, new Size((int)(origin.Width * ratio), (int)(origin.Height * ratio)));
                                resize.Save(ms2, System.Drawing.Imaging.ImageFormat.Jpeg);
                                byte[] image2 = ms2.ToArray();

                                var reClient = new RestClient(api) { Timeout = -1 };
                                var request2 = new RestRequest(Method.POST);
                                request2.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
                                request2.AddHeader("X-RPC-DIRECTORY", "main");
                                request2.AddHeader("X-RPC-MIN-SIZE", "114");
                                request2.AddHeader("X-RPC-FACES-DETECT-THRESHOLD", "0");
                                request2.AddHeader("X-RPC-FACES-GROUPING-THRESHOLD", "0");
                                request2.AddHeader("X-RPC-EXTERNAL-ID", image_id);
                                request2.AddHeader("X-RPC-PERSON-FIRST-NAME", info);
                                request2.AddHeader("Content-Type", "image/jpeg");
                                request2.AddParameter("image/jpeg", image2, ParameterType.RequestBody);
                                IRestResponse safrResponse2 = reClient.Execute(request2);
                                if (safrResponse2.StatusCode == System.Net.HttpStatusCode.Created)
                                {
                                    jo = JObject.Parse(safrResponse2.Content);
                                    safr_id = jo.SelectToken($"identifiedFaces[0].personId");
                                }
                                origin.Dispose();
                                resize.Dispose();
                            }
                        }

                        return safr_id.ToString();
                    }

                    return null;
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Enroll] {ex.Message}");
                return null;
            }
        }

        public float Verify(byte[] fir1, byte[] fir2)
        {
            // 1번 이미지 등록
            string api = $"http://{Settings.Default.SafrHost}:{Settings.Default.Covi}/people?merge=false&update=false&detect-mask=true&detect-occlusion=true&largest-only=true&" +
            $"min-cpq={Settings.Default.CenterPose}&min-fsq={Settings.Default.Sharpness}&min-fcq={Settings.Default.Contrast}&insert-profile=true";
            var safrClient = new RestClient(api) { Timeout = -1 };

            var request = new RestRequest(Method.POST);
            request.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
            request.AddHeader("X-RPC-DIRECTORY", "main");
            //request.AddHeader("X-RPC-MIN-SIZE", "0");
            request.AddHeader("X-RPC-FACES-DETECT-THRESHOLD", "0");
            request.AddHeader("X-RPC-FACES-GROUPING-THRESHOLD", "0");
            request.AddHeader("Content-Type", "image/jpeg");
            request.AddParameter("image/jpeg", fir1, ParameterType.RequestBody);
            IRestResponse safrResponse = safrClient.Execute(request);

            float score = 0;

            if (safrResponse.StatusCode == System.Net.HttpStatusCode.Created)
            {
                JObject jo = JObject.Parse(safrResponse.Content);
                object safr_id = jo.SelectToken($"identifiedFaces[0].personId");
                if (safr_id != null && !string.IsNullOrEmpty(safr_id.ToString()))
                {
                    // 등록 성공시 비교 api 보냄
                    api = $"http://{Settings.Default.SafrHost}:{Settings.Default.Covi}/verification/{safr_id}";
                    safrClient = new RestClient(api) { Timeout = -1 };
                    request = new RestRequest(Method.POST);
                    request.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
                    request.AddHeader("X-RPC-DIRECTORY", "main");
                    request.AddHeader("Content-Type", "image/jpeg");
                    request.AddParameter("image/jpeg", fir2, ParameterType.RequestBody);

                    safrResponse = safrClient.Execute(request);
                    if (safrResponse.StatusCode == System.Net.HttpStatusCode.OK)
                    {
                        jo = JObject.Parse(safrResponse.Content);
                        object safrScore = jo.SelectToken($"identifiedFaces[0].similar[0].similarityScore");
                        if (safrScore != null)
                        {
                            score = Convert.ToSingle(safrScore) * 0.633f;
                        }
                    }

                    // 등록된 1번 이미지 삭제(똑같은 사진 등록되어있으면 삭제 X)
                    int idx = gallery.FindIndex(x => x.safr_id.Equals(safr_id.ToString()));
                    if (idx < 0) 
                    {
                        api = $"http://{Settings.Default.SafrHost}:{Settings.Default.Covi}/people/{safr_id}?recursive=true";

                        safrClient = new RestClient(api) { Timeout = -1 };
                        request = new RestRequest(Method.DELETE);
                        request.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
                        request.AddHeader("X-RPC-DIRECTORY", "main");
                        IRestResponse SafrResponse = safrClient.Execute(request);
                    }
                }
            }
            return score = score > 1 ? 1 : score;
        }

        public void Add(string target_id, string safr_id)
        {
            gallery.Add(new Person()
            {
                target_id = target_id,
                safr_id = safr_id
            });
        }

        public Match[] Identify(byte[] fir, float threshold, int maxMatches = 100)
        {
            List<Match> matches = new List<Match>();

            string api = $"http://{Settings.Default.SafrHost}:{Settings.Default.Covi}/people?detect-mask=true&detect-occlusion=true&insert=false&update=false&merge=false&similar_limit=100&largest-only=true&" +
                            $"min-cpq={Settings.Default.CenterPose}&min-fsq={Settings.Default.Sharpness}&min-fcq={Settings.Default.Contrast}&insert-profile=true";

            var client = new RestClient(api) { Timeout = -1 };
            var request = new RestRequest(Method.POST);
            request.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
            request.AddHeader("X-RPC-DIRECTORY", "main");
            request.AddHeader("X-RPC-MIN-SIZE", "0");
            request.AddHeader("X-RPC-FACES-DETECT-THRESHOLD", "0");
            request.AddHeader("X-RPC-FACES-GROUPING-THRESHOLD", "0");
            request.AddHeader("Content-Type", "image/jpeg");
            request.AddParameter("image/jpeg", fir, ParameterType.RequestBody);

            IRestResponse response = client.Execute(request);
            if (response.StatusCode == System.Net.HttpStatusCode.Created)
            {
                JObject jo = JObject.Parse(response.Content);
                List<JToken> similars = jo.SelectToken($"identifiedFaces[0].similar")?.ToList();

                if (similars != null)
                {
                    foreach (var token in similars)
                    {
                        var info = token["firstName"];
                        if (info == null) continue;
                        var infos = PersonObject.ParseInfo(info.ToString());

                        var externalId = token["externalId"];
                        if (externalId == null) continue;
                        var person_id = externalId.ToString();

                        var score = Convert.ToSingle(token["similarityScore"].ToString());

                        if(score > threshold)
                            matches.Add(new Match
                            {
                                id = person_id,
                                score = score > 1 ? 1 : score
                            });
                    }
                }
            }

            return matches.OrderByDescending(x => x.score).Take(maxMatches).ToArray();
        }

        public void LoadGallery(string groupCd)
        {
            logger.Info($"[LoadGallery] Group [{groupCd}]");

            try
            {
                using (DBClient dbc = new DBClient())
                {
                    var items = dbc.LoadGallery(groupCd);

                    foreach (DataRow item in items.Rows)
                    {
                        Add(Convert.ToString(item["TGTMNG_OBJ_ID"]), Convert.ToString(item["ETC_REF_DSC_05"]));
                    }
                }
            }
            catch (Exception ex)
            {
                logger.Error($"LoadGalley [{ex.Message}]");
            }

            logger.Info($"GalleryCount : {gallery.Count}");
        }

        public int Add(string image_id)
        {
            using (DBClient dbc = new DBClient())
            {
                var safr_id = Enroll(image_id);
                if (!string.IsNullOrWhiteSpace(safr_id))
                {
                    dbc.SetSafrId(image_id, safr_id);
                    Add(image_id, safr_id);
                
                    return gallery.Count;
                }
                else
                {
                    logger.Error($"[Add] safr_id is Null");
                
                    return gallery.Count;
                }
            }
        }

        public int Remove(string image_id)
        {
            int count = 0;

            try
            {
                rwLock.EnterWriteLock();
                
                var person = gallery.Find(x => x.target_id.Equals(image_id));

                if (person != null) 
                {
                    gallery.Remove(person);
                    var safr_id = person.safr_id;

                    int duplicateCnt = gallery.Where(x => x.safr_id.Equals(safr_id)).Count();
                    if (duplicateCnt == 0)
                        RemovePerson_Safr(safr_id);

                    count = gallery.Count;
                    logger.Debug($"[Remove] total count : {count}");
                }
                else
                {
                    logger.Error($"[Remove] key not found : {image_id}");
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Remove] id[{image_id}] : {ex.Message}");
            }
            finally
            {
                rwLock.ExitWriteLock();
            }

            return count;
        }

        public IdentifyResult Identify(string dbscan_id)
        {
            using (DBClient dbc = new DBClient())
            {
                var info = dbc.GetSScanInfo(dbscan_id);
                if (info == null)
                    return IdentifyResult.Fail($"req not found : {dbscan_id}");

                var matches = Identify(info.FIR, info.THRESHOLD);
                int maxMatches = Math.Min(100, matches.Length);
                
                dbc.InsertDBScanRslt(dbscan_id, matches.ToList().GetRange(0, maxMatches).ToArray());

                var result = dbc.GetMatchedFaces(dbscan_id);

                return IdentifyResult.Success(result);
            }
        }

        public VerifyResult Verify(string verify_id)
        {
            using (DBClient dbc = new DBClient())
            {
                DataRow reqInfo = dbc.GetVerifyReq(verify_id);

                var image_data1 = dbc.GetImage(reqInfo["FACE_ID1"] as string);
                var image_data2 = dbc.GetImage(reqInfo["FACE_ID2"] as string);

                if (image_data1 == null || image_data2 == null)
                {
                    logger.Error("[Verify] Error : image_data is Null (Check Face_ID)");
                    return VerifyResult.Fail("FACE_ID NOT FOUND");
                }

                float tp_score = Verify(image_data1, image_data2);
                
                dbc.InsertVerifyRslt(verify_id, tp_score);

                return VerifyResult.Success(tp_score);
            }
        }

        public void ResetFir()
        {
            using (DBClient dbc = new DBClient())
            {
                RemovePeople_Safr();
                dbc.ResetSafrId();

                var items = dbc.test();

                foreach (DataRow item in items.Rows)
                {
                    var id = Enroll(item["TGTMNG_OBJ_ID"] as string);
                    
                    if (id != null)
                        dbc.UpdateSafrId(Convert.ToString(item["TGTMNG_OBJ_ID"]), id);
                    else
                        logger.Debug($"id[{Convert.ToString(item["TGTMNG_OBJ_ID"])}]");
                }
            }
        }

        public void RemovePerson_Safr(string safr_id)
        {
            string api = $"http://{Settings.Default.SafrHost}:{Settings.Default.Covi}/people/{safr_id}?recursive=true";

            var client = new RestClient(api) { Timeout = -1 };
            var request = new RestRequest(Method.DELETE);
            request.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
            request.AddHeader("X-RPC-DIRECTORY", "main");
            IRestResponse response = client.Execute(request);
            if (response.StatusCode == System.Net.HttpStatusCode.NoContent)
            {
                logger.Debug($"[RemovePerson_Safr] Delete Success safr_id[{safr_id}]");
            }
            else
            {
                logger.Error($"[RemovePerson_Safr] Delete Error safr_id[{safr_id}] Status[{response.StatusCode}] ErrorMessage[{response.ErrorMessage}] " +
                    $"Content[{response.Content}]");
            }
        }

        public void RemovePeople_Safr()
        {
            var request = new RestRequest(Method.GET);
            request.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
            request.AddHeader("X-RPC-DIRECTORY", "main");

            string api = $"http://{Settings.Default.SafrHost}:{Settings.Default.Covi}/people";

            var client = new RestClient(api) { Timeout = -1 };
            IRestResponse response = client.Execute(request);

            var jo = JObject.Parse(response.Content);

            if (response.StatusCode != System.Net.HttpStatusCode.OK)
                throw new Exception("response Fail");

            var personIdList = jo.SelectToken("people")
                .Select(x => x.SelectToken("personId"));

            var deleteRequest = new RestRequest(Method.DELETE);
            deleteRequest.AddHeader("X-RPC-AUTHORIZATION", Settings.Default.Auth);
            deleteRequest.AddHeader("X-RPC-DIRECTORY", "main");

            foreach (var personId in personIdList)
            {
                string deleteApi = $"http://{Settings.Default.SafrHost}:{Settings.Default.Covi}/people/{personId}?recursive=true";

                var deleteClient = new RestClient(deleteApi) { Timeout = -1 };
                IRestResponse deleteResponse = deleteClient.Execute(deleteRequest);

                if (deleteResponse.StatusCode != System.Net.HttpStatusCode.NoContent)
                    throw new Exception("deleteResponse Fail");
            }
        }
    }

    public class Match
    {
        public string id;
        public float score;
    }

    public class Person
    {
        public string target_id;
        public string safr_id;
    }
}
