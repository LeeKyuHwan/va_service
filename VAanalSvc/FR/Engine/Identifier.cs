using DataBase;
using DataBase.DataObject;
using DataContracts;
using log4net;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace VAnalSvc
{
    class Identifier
    {
        static ILog logger = LogManager.GetLogger(nameof(Identifier));

        DBClient dbc;

        public Identifier(DBClient dbc)
        {
            this.dbc = dbc;
        }

        public void Identify(VideoInfo video)
        {
            logger.Debug($"[Identify] start");

            var video_id = video.VIDEO_ID;
            var threshold = video.THRESHOLD.ToString();

            dbc.UpdateVanalStatus(video_id, Codes.VANAL_STS_CD.I, 100);

            var crops = dbc.GetRepresentations(video_id);
            for (int i = 0; i < crops.Length; i++)
            {
                var crop = crops[i];
                var result = identify(crop.FIR, threshold);
                if (result.result_code.Equals("S") && result.matches.Count > 0)
                    dbc.CreateMatchRslt(crop, result);

                logger.Debug($"[Identify] {i + 1}/{crops.Length} : {result.result_code}");
            }

            logger.Debug($"[Identify] completed");
        }

        private DBScanResult identify(byte[] fir, string threshold)
        {
            try
            {
                var http = new HttpClient();
                var form = new MultipartFormDataContent();
                form.Add(new StreamContent(new MemoryStream(fir)), "fir", "fir");
                form.Add(new StringContent(threshold), "threshold");

                var response = http.PostAsync("http://127.0.0.1:9092/face/REST/dbscan", form).Result;

                if (response.IsSuccessStatusCode)
                {
                    return response.Content.ReadAsAsync<DBScanResult>().Result;
                }
                else
                {
                    logger.Error($"[identify] api failed : {response.StatusCode}");
                    return DBScanResult.Fail(response.StatusCode.ToString());
                }
            }
            catch (Exception ex)
            {
                return DBScanResult.Fail(ex.Message);
            }
        }
    }
}
