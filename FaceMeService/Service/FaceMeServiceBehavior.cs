using DataContracts;
using log4net;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Threading.Tasks;
using WcfSvcBase;

namespace FaceMeService
{
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Single, InstanceContextMode = InstanceContextMode.PerCall, UseSynchronizationContext = false)]
    public class FaceMeServiceBehavior : IFaceMeServiceContract
    {
        static ILog logger = LogManager.GetLogger(typeof(FaceMeServiceBehavior));

        private void allowCORS()
        {
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Origin", "*");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Credentials", "true");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Cache-Control", "no-cache, no-store");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Methods", "GET,POST");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Headers", "*");
        }

        public IdentifyResult Identify(Stream stream)
        {
            try
            {
                allowCORS();
            
                var data = Utils.ParseFormData(stream);
                logger.Info($"[Identify] {Utils.PrintDic(data)}");
            
                if (data.TryGetValue("dbscan_id", out dynamic dbscan_id) == false)
                    throw new Exception("param dbscan_id not found");
            
                var result = FrHelper.Instance.Identify(dbscan_id);
            
                logger.Debug($"[Identify] {result.matches.Count} matches");
            
                return result;
            }
            catch (Exception ex)
            {
                logger.Error($"[Identify] {ex.Message}");
                return IdentifyResult.Fail(ex.Message);
            }
        }

        public VerifyResult Verify(Stream stream)
        {
            try
            {
                allowCORS();
            
                var data = Utils.ParseFormData(stream);
            
                logger.Info($"[Verify] {Utils.PrintDic(data)}");
            
                if (data.TryGetValue("verify_id", out dynamic verify_id) == false)
                    throw new Exception("param verify_id not found");
            
                var result = FrHelper.Instance.Verify(verify_id);
            
                logger.Debug($"[Verify] {result}");
            
                return result;
            }
            catch (Exception ex)
            {
                logger.Error($"[Verify] {ex.Message}");
                return VerifyResult.Fail(ex.Message);
            }
        }

        public ApiResult AddImage(Stream stream)
        {
            try
            {
                allowCORS();
            
                var data = Utils.ParseFormData(stream);
                logger.Info($"[AddImage] {Utils.PrintDic(data)}");
            
                if (data.TryGetValue("image_id", out dynamic image_id) == false)
                    throw new Exception("param image_id not found");
            
                if (data.TryGetValue("job_cd", out dynamic job_cd) == false)
                    throw new Exception("param job_cd not found");
            
                switch (job_cd)
                {
                    case "A":
                        {
                            int count = FrHelper.Instance.Add(image_id);
                            logger.Debug($"[AddImage] total count : {count}");
                        }
                        break;
            
                    case "D":
                        {
                            int count = FrHelper.Instance.Remove(image_id);
                            logger.Debug($"[AddImage] total count : {count}");
                        }
                        break;
            
                    default:
                        throw new Exception($"invalid job_cd : {job_cd}");
                }
            
                return ApiResult.Success;
            }
            catch (Exception ex)
            {
                logger.Error($"[AddImage] {ex.Message}");
                return ApiResult.Fail(ex.Message);
            }
        }

    }
}
