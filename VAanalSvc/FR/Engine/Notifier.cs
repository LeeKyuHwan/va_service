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
    class Notifier
    {
        static ILog logger = LogManager.GetLogger(nameof(Notifier));

        DBClient dbc;

        public Notifier(DBClient dbc)
        {
            this.dbc = dbc;
        }

        public AddVideoResult Notify(VideoInfo video)
        {
            logger.Debug($"[Notifier] start");

            try
            {
                var video_id = video.VIDEO_ID;
                var threshold = video.THRESHOLD.ToString();

                var http = new HttpClient();
                var form = new MultipartFormDataContent();
                form.Add(new StringContent(video_id), "video_id");

                var response = http.PostAsync("http://127.0.0.1:9093/face/REST/add_video", form).Result;

                if (response.IsSuccessStatusCode)
                {
                    return response.Content.ReadAsAsync<AddVideoResult>().Result;
                }
                else
                {
                    logger.Error($"[Notifier] api failed : {response.StatusCode}");
                    return AddVideoResult.Fail(response.StatusCode.ToString());
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Notifier] {ex.Message}");
                return AddVideoResult.Fail(ex.Message);
            }
        }
    }
}
