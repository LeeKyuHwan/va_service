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

namespace FrService
{
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Single, InstanceContextMode = InstanceContextMode.PerCall, UseSynchronizationContext = false)]
    public class FrServiceBehavior : IFrServiceContract
    {
        static ILog logger = LogManager.GetLogger(typeof(FrServiceBehavior));

        private void allowCORS()
        {
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Origin", "*");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Credentials", "true");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Cache-Control", "no-cache, no-store");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Methods", "GET,POST");
            WebOperationContext.Current.OutgoingResponse.Headers.Add("Access-Control-Allow-Headers", "*");
        }

        public DetectResult Detect(Stream stream)
        {
            try
            {
                allowCORS();

                var data = Utils.ParseFormData(stream);

                logger.Info($"[Detect] {Utils.PrintDic(data)}");

                if (data.TryGetValue("detect_id", out dynamic detect_id) == false)
                    throw new Exception("param detect_id not found");

                var result = FrHelper.Instance.Detect(detect_id);

                logger.Debug($"[Detect] {result}");

                return result;
            }
            catch (Exception ex)
            {
                logger.Error($"[Detect] {ex.Message}");
                return DetectResult.Fail(ex.Message);
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

        public CharacteristicsResult Characteristics(string face_id)
        {
            try
            {
                allowCORS();

                logger.Info($"[Characteristics] face_id:{face_id}");

                var result = FrHelper.Instance.characteristics(face_id);

                logger.Debug($"[Characteristics] {result}");

                return result;
            }
            catch (Exception ex)
            {
                logger.Error($"[Characteristics] {ex.Message}");
                return CharacteristicsResult.Fail(ex.Message);
            }
        }

        public ApiResult UpdateFir(Stream stream)
        {
            try
            {
                allowCORS();

                var data = Utils.ParseFormData(stream);

                logger.Info($"[UpdateFir] {Utils.PrintDic(data)}");

                if (data.TryGetValue("image_id", out dynamic image_id) == false)
                    throw new Exception("param image_id not found");

                var result = FrHelper.Instance.UpdateFir(image_id);

                logger.Debug($"[UpdateFir] {result}");

                return result;
            }
            catch (Exception ex)
            {
                logger.Error($"[UpdateFir] {ex.Message}");
                return ApiResult.Fail(ex.Message);
            }
        }
    }
}
