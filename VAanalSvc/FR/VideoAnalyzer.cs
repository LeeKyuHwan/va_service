using DataBase.DataObject;
using log4net;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using VAnalSvc.Properties;

namespace VAnalSvc
{
    class VideoAnalyzer
    {
        static ILog logger = LogManager.GetLogger(nameof(VideoAnalyzer));

        public static VideoAnalyzer Instance = new VideoAnalyzer();
        private VideoAnalyzer() { }

        ConcurrentQueue<VideoInfo> queue = new ConcurrentQueue<VideoInfo>();
        AutoResetEvent idle = new AutoResetEvent(true);

        public void Enqueue(VideoInfo video)
        {
            queue.Enqueue(video);

            logger.Info($"[Enqueue] {queue.Count} job ready");

            if (idle.WaitOne(100))
            {
                ThreadPool.QueueUserWorkItem(delegate
                {
                    Start();
                });
            }
        }

        void Start()
        {
            using (var dbc = new DBClient())
            {
                var frameAnalyzer = new FrameAnalyzer(dbc);
                var repMaker = new RepImgMaker(dbc);
                var identifier = new Identifier(dbc);
                var notifier = new Notifier(dbc);

                while (queue.TryDequeue(out VideoInfo video))
                {
                    try
                    {
                        logger.Info($"[Start] starting job : VIDEO_ID[{video.VIDEO_ID}]");

                        //if (Settings.Default.Analyze)
                        //    frameAnalyzer.AnalyzeFrames(video);

                        if (Settings.Default.Representaion)
                            repMaker.SetRepresentation(video);

                        if (Settings.Default.Identify)
                            identifier.Identify(video);

                        dbc.UpdateVanalStatus_Cmplt(video.VIDEO_ID);

                        notifier.Notify(video);

                        logger.Info($"[Start] job completed : VIDEO_ID[{video.VIDEO_ID}]");
                    }
                    catch (Exception ex)
                    {
                        dbc.UpdateVanalStatus_Cmplt(video.VIDEO_ID, "F");

                        logger.Error($"[Start] {ex.Message}");
                    }
                }
            }

            logger.Info($"[Start] job queue is empty");

            idle.Set();
        }
    }
}
