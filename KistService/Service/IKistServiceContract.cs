using DataContracts;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Threading.Tasks;

namespace KistService
{
    [ServiceContract]
    public interface IKistServiceContract
    {
        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "identify", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        IdentifyResult Identify(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "verify", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        VerifyResult Verify(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "add_image", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        ApiResult AddImage(Stream stream);
    }
}
