using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Threading.Tasks;
using DataBase.DataObject;
using DataContracts;
using log4net;
using WcfSvcBase;

namespace VAnalSvc
{
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Single, InstanceContextMode = InstanceContextMode.PerCall, UseSynchronizationContext = false)]
    public class VAnalServiceBehavior : IVAnalServiceContract
    {
        static ILog logger = LogManager.GetLogger(typeof(VAnalServiceBehavior));

        private void allowCORS()
        {
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Origin", "*");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Credentials", "true");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Cache-Control", "no-cache, no-store");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Methods", "GET,POST");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Headers", "*");
        }

        public AddVideoResult AnalyzeVideo(Stream stream)
        {
            try
            {
                allowCORS();

                var data = Utils.ParseFormData(stream);
                logger.Info($"[AddVideo] {Utils.PrintDic(data)}");

                if (data.TryGetValue("video_id", out dynamic video_id) == false)
                    throw new Exception("param video_id not found");

                using (DBClient dbc = new DBClient())
                {
                    var video = dbc.GetVideoInfo(video_id);
                    if (video == null)
                        throw new Exception("video not found");

                    VideoAnalyzer.Instance.Enqueue(video);
                }

                var result = AddVideoResult.Success();
                result.video_id = video_id;

                logger.Debug($"[AddVideo] {result}");

                return result;
            }
            catch (Exception ex)
            {
                logger.Error($"[AddVideo] {ex.Message}");
                return AddVideoResult.Fail(ex.Message);
            }
        }
    }
}
