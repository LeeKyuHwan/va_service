using DataContracts;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Threading.Tasks;

namespace FrService
{
    [ServiceContract]
    public interface IFrServiceContract
    {
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
        [WebInvoke(Method = "POST", UriTemplate = "update_fir", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        ApiResult UpdateFir(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "reset_fir", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        ApiResult ResetFir();
    }
}
