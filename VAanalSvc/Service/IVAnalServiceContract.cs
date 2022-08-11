using DataContracts;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Threading.Tasks;

namespace VAnalSvc
{
    [ServiceContract]
    public interface IVAnalServiceContract
    {
        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "analyze_video", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        AddVideoResult AnalyzeVideo(Stream stream);
    }
}
