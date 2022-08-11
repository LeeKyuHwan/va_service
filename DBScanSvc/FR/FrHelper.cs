using DataContracts;
using log4net;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DBScanSvc
{
    class FrHelper
    {
        static ILog logger = LogManager.GetLogger(nameof(FrHelper));
        public static FrHelper Instance = new FrHelper();

        Cognitec.FRsdk.Configuration cfg;
        Cognitec.FRsdk.FIRBuilder firBuilder;
        Cognitec.FRsdk.FacialMatchingEngine fme;
        Cognitec.FRsdk.Enrollment.Processor enrollProc;
        Gallery gallery = new Gallery();

        private FrHelper()
        {
            cfg = new Cognitec.FRsdk.Configuration(Properties.Settings.Default.config);

            firBuilder = new Cognitec.FRsdk.FIRBuilder(cfg);
            fme = new Cognitec.FRsdk.FacialMatchingEngine(cfg);
            enrollProc = new Cognitec.FRsdk.Enrollment.Processor(cfg);
        }

        public void LoadGallery(string groupCd)
        {
            logger.Info($"[LoadGallery] Group [{groupCd}]");

            using (DBClient dbc = new DBClient())
            {
                var items = dbc.LoadGallery(groupCd);
                gallery.AddRange(items);
            }
        }

        Cognitec.FRsdk.Image FromBytes(byte[] data)
        {
            using (MemoryStream ms = new MemoryStream(data))
                return Cognitec.FRsdk.ImageUtils.load(ms);
        }

        byte[] ToBytes(Cognitec.FRsdk.FIR fir)
        {
            using (MemoryStream ms = new MemoryStream())
            {
                fir.serialize(ms);
                return ms.ToArray();
            }
        }

        EnrollmentFeedback Enroll(Cognitec.FRsdk.Image img)
        {
            EnrollmentFeedback feedback = new EnrollmentFeedback();
            enrollProc.process(new Cognitec.FRsdk.Sample[1] { new Cognitec.FRsdk.Sample(img) }, feedback);
            return feedback;
        }

        public int Add(string image_id)
        {
            using (DBClient dbc = new DBClient())
            {
                var imageData = dbc.GetImage(image_id);
                var image = FromBytes(imageData);
                using (var feedback = Enroll(image))
                {
                    if (feedback.Fir != null)
                    {
                        var firData = ToBytes(feedback.Fir);
                        var result = dbc.UpdateFir(image_id, firData);
                        return gallery.Add(image_id, firData);
                    }
                    else
                    {
                        logger.Error($"[Add] {feedback.FailReason}");
                        return gallery.Count;
                    }
                }
            }
        }

        public int Remove(string image_id)
        {
            return gallery.Remove(image_id);
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

        public IdentifyResult Identify(string dbscan_id)
        {
            using (DBClient dbc = new DBClient())
            {
                var info = dbc.GetDBScanInfo(dbscan_id);
                if (info == null)
                    return IdentifyResult.Fail($"req not found : {dbscan_id}");

                using (var fir = BuildFir(info.FIR))
                {
                    var matches = gallery.Matches(fir, info.THRESHOLD);
                    int maxMatches = Math.Min(100, matches.Length);
                    
                    dbc.InsertDBScanRslt(dbscan_id, matches.ToList().GetRange(0, maxMatches).ToArray());
                }

                var result = dbc.GetMatchedFaces(dbscan_id);
                return IdentifyResult.Success(result);
            }
        }

        public DBScanResult DBScan(byte[] firData, float threshold)
        {
            DBScanResult result = DBScanResult.Success(new System.Collections.ObjectModel.Collection<DBScanResult.MatchResult>());
            
            using (var fir = BuildFir(firData))
            {
                var matches = gallery.Matches(fir, threshold);
                int maxMatches = Math.Min(20, matches.Length);

                for (int i = 0; i < maxMatches; i++)
                {
                    var match = new DBScanResult.MatchResult()
                    {
                        person_id = matches[i].name,
                        score = matches[i].score.value,
                        rank = i + 1
                    };

                    result.matches.Add(match);
                }
            }

            return result;
        }
    }
}
