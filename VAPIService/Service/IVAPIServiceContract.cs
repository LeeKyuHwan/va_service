using DataContracts;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Threading.Tasks;

namespace VAPIService
{
    [ServiceContract]
    public interface IVAPIServiceContract
    {
        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "add_video", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        AddVideoResult AddVideo(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "remove_video", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        RemoveVideoResult RemoveVideo(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "GET", UriTemplate = "vanal_list?display={display}&page={page}&title={title}&reg_dt={reg_dt}&vanal_sts_cd={vanal_sts_cd}", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        VanalListResult VanalList(string display, string page, string title, string reg_dt, string vanal_sts_cd);

        [OperationContract]
        [WebInvoke(Method = "GET", UriTemplate = "vanal_rslt?video_id={video_id}&display={display}&page={page}", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        VanalRsltResult VanalRslt(string video_id, string display, string page);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "vsearch", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        VsearchResult Vsearch(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "GET", UriTemplate = "vsearch_dtl?vsearch_id={vsearch_id}&video_id={video_id}", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        VsearchDtlResult VsearchDtl(string vsearch_id, string video_id);

        [OperationContract]
        [WebInvoke(Method = "GET", UriTemplate = "image?face_id={face_id}")]
        Stream GetImage(string face_id);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "identify", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        IdentifyResult Identify(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "GET", UriTemplate = "detail?person_id={person_id}", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        PersonInfo GetPersonInfo(string person_id);

        [OperationContract]
        [WebInvoke(Method = "*", UriTemplate = "groups", ResponseFormat = WebMessageFormat.Json)]
        Groups GetGroups();

        [OperationContract]
        [WebInvoke(Method = "GET", UriTemplate = "person_list?group_cd={group_cd}", ResponseFormat = WebMessageFormat.Json)]
        PersonListResult GetPersonList(string group_cd);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "detect", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        DetectResult Detect(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "verify", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        VerifyResult Verify(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "GET", UriTemplate = "characteristics?face_id={face_id}", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)] 
        CharacteristicsResult Characteristics(string face_id);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "add_person", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        AddPersonResult AddPerson(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "remove_person", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        ApiResult RemovePerson(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "add_person_multi", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        ApiResult AddPersonMulti(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "remove_person_multi", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        ApiResult RemovePersonMulti(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "verify_multi", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        VerifyMultiResult VerifyMulti(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "identify_multi", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        IdentifyMultiResult IdentifyMulti(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "test", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        ApiResult Test();

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "identify_multi_v2", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        IdentifyMultiResult IdentifyMultiV2(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "verify_multi_v2", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        VerifyMultiResult VerifyMultiV2(Stream stream);
    }
}
