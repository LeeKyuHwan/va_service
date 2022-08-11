using log4net;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VSearchSvc
{
    class FrHelper
    {
        static ILog logger = LogManager.GetLogger(nameof(FrHelper));

        public static FrHelper Instance = new FrHelper();

        Cognitec.FRsdk.Configuration cfg;
        Cognitec.FRsdk.FIRBuilder firBuilder;
        Cognitec.FRsdk.FacialMatchingEngine fme;
        Gallery gallery = new Gallery();

        private FrHelper()
        {
            cfg = new Cognitec.FRsdk.Configuration(@"C:\FVSDK_9_3_1\etc\frsdk.cfg");

            firBuilder = new Cognitec.FRsdk.FIRBuilder(cfg);
            fme = new Cognitec.FRsdk.FacialMatchingEngine(cfg);
        }

        public void LoadVideos()
        {
            logger.Info($"[LoadVideos]");

            using (DBClient dbc = new DBClient())
            {
                string[] video_id = dbc.GetVideos();
                for (int i = 0; i < video_id.Length; i++)
                {
                    Item[] items = dbc.GetVideo(video_id[i]);
                    if (items.Length > 0)
                    {
                        gallery.AddRange(video_id[i], items);
                        logger.Info($"[LoadVideos] VIDEO_ID[{video_id[i]}] : {items.Length}");
                    }
                }

                logger.Info($"[LoadVideos] {video_id.Length} video, {gallery.Count} fir loaded");
            }
        }

        public Cognitec.FRsdk.FIR BuildFir(byte[] data)
        {
            using (MemoryStream ms = new MemoryStream(data))
                return firBuilder.build(ms);
        }

        public float Compare(Cognitec.FRsdk.FIR fir, byte[] firData)
        {
            using (var fir2 = BuildFir(firData))
            {
                return fme.compare(fir, fir2).value;
            }
        }

        public VideoMatchResult[] VSearch(string[] video_id, byte[] firData, float threshold)
        {
            List<VideoMatchResult> results = new List<VideoMatchResult>();

            using (var fir = BuildFir(firData))
            {
                for (int i = 0; i < video_id.Length; i++)
                {
                    var matches = gallery.Matches(video_id[i], fir, threshold);
                    var result = new VideoMatchResult(video_id[i],
                                                      matches.Select(x => x.name).ToArray(),
                                                      matches.Select(x => x.score.value).ToArray());
                    results.Add(result);
                }
            }

            return results.ToArray();
        }

        public void AddVideo(string video_id)
        {
            using (DBClient dbc = new DBClient())
            {
                Item[] items = dbc.GetVideo(video_id);
                if (items.Length > 0)
                {
                    int count = gallery.AddRange(video_id, items);
                    logger.Info($"[AddVideo] {video_id} added, {gallery.Count} fir loaded");
                }
                else
                {
                    logger.Info($"[AddVideo] no firs to add : [{video_id}]");
                }
            }
        }

        public void RemoveVideo(string video_id)
        {
            int count = gallery.RemoveVideo(video_id);
            logger.Info($"[RemoveVideo] {video_id} removed, {count} fir loaded");
        }
    }
}
