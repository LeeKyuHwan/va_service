using DataContracts;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Threading.Tasks;

namespace VSearchSvc
{
    [ServiceContract]
    public interface IVSearchServiceContract
    {
        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "search_video", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        SearchVideoResult SearchVideo(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "add_video", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        AddVideoResult AddVideo(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "remove_video", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        RemoveVideoResult RemoveVideo(Stream stream);
    }
}
