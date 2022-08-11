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

namespace VSearchSvc
{
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Single, InstanceContextMode = InstanceContextMode.PerCall, UseSynchronizationContext = false)]
    public class VSearchServiceBehavior : IVSearchServiceContract
    {
        static ILog logger = LogManager.GetLogger(typeof(VSearchServiceBehavior));

        private void allowCORS()
        {
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Origin", "*");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Credentials", "true");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Cache-Control", "no-cache, no-store");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Methods", "GET,POST");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Headers", "*");
        }

        public SearchVideoResult SearchVideo(Stream stream)
        {
            try
            {
                allowCORS();

                var data = Utils.ParseFormData(stream);
                logger.Info($"[SearchVideo] {Utils.PrintDic(data)}");

                if (data.TryGetValue("vsearch_id", out dynamic vsearch_id) == false)
                    throw new Exception("param vsearch_id not found");

                using (DBClient dbc = new DBClient())
                {
                    VSearchInfo info = dbc.GetVSearchInfo(vsearch_id);
                    if (info == null)
                        throw new Exception("vsearch req not found");

                    var matches = FrHelper.Instance.VSearch(info.VIDEO_ID, info.FIR, info.THRESHOLD);
                    logger.Debug($"[SearchVideo] {matches.Length} matches");

                    if (matches.Length > 0)
                        dbc.InsertVSearchResult(vsearch_id, matches);
                }

                var result = SearchVideoResult.Success(vsearch_id);

                logger.Debug($"[SearchVideo] {result}");

                return result;
            }
            catch (Exception ex)
            {
                logger.Error($"[SearchVideo] {ex.Message}");
                return SearchVideoResult.Fail(ex.Message);
            }
        }

        public AddVideoResult AddVideo(Stream stream)
        {
            try
            {
                allowCORS();

                var data = Utils.ParseFormData(stream);
                logger.Info($"[AddVideo] {Utils.PrintDic(data)}");

                if (data.TryGetValue("video_id", out dynamic video_id) == false)
                    throw new Exception("param video_id not found");

                FrHelper.Instance.AddVideo(video_id);

                return AddVideoResult.Success();
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

                var data = Utils.ParseFormData(stream);
                logger.Info($"[RemoveVideo] {Utils.PrintDic(data)}");

                if (data.TryGetValue("video_id", out dynamic video_id) == false)
                    throw new Exception("param video_id not found");

                FrHelper.Instance.RemoveVideo(video_id);

                return RemoveVideoResult.Success;
            }
            catch (Exception ex)
            {
                logger.Error($"[RemoveVideo] {ex.Message}");
                return RemoveVideoResult.Fail(ex.Message);
            }
        }
    }
}
