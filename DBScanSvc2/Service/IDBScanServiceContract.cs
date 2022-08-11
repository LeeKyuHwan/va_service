using DataContracts;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Threading.Tasks;

namespace DBScanSvc
{
    [ServiceContract]
    public interface IDBScanServiceContract
    {
        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "identify", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        IdentifyResult Identify(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "dbscan", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        DBScanResult DBScan(Stream stream);

        [OperationContract]
        [WebInvoke(Method = "POST", UriTemplate = "add_image", ResponseFormat = WebMessageFormat.Json, BodyStyle = WebMessageBodyStyle.Bare)]
        ApiResult AddImage(Stream stream);
    }
}
