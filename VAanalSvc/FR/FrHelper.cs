using log4net;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VAnalSvc
{
    class FrHelper
    {
        static ILog logger = LogManager.GetLogger(nameof(FrHelper));

        Cognitec.FRsdk.Configuration cfg;

        Cognitec.FRsdk.FIRBuilder firBuilder;

        Cognitec.FRsdk.Face.Tracker faceTracker;
        Cognitec.FRsdk.Eyes.Finder eyesFinder;

        Cognitec.FRsdk.Enrollment.Processor enrollProc;
        Cognitec.FRsdk.ISO_19794_5.TokenFace.Creator tokenCreator;
        Cognitec.FRsdk.Portrait.Analyzer pc;
        Cognitec.FRsdk.FacialMatchingEngine fme;

        readonly FrOption option;

        public FrHelper()
        {
            cfg = new Cognitec.FRsdk.Configuration(@"C:\FVSDK_9_3_1\etc\frsdk.cfg");

            firBuilder = new Cognitec.FRsdk.FIRBuilder(cfg);
            faceTracker = new Cognitec.FRsdk.Face.Tracker(cfg);
            eyesFinder = new Cognitec.FRsdk.Eyes.Finder(cfg);
            enrollProc = new Cognitec.FRsdk.Enrollment.Processor(cfg);
            tokenCreator = new Cognitec.FRsdk.ISO_19794_5.TokenFace.Creator(cfg);
            pc = new Cognitec.FRsdk.Portrait.Analyzer(cfg);
            fme = new Cognitec.FRsdk.FacialMatchingEngine(cfg);
        }

        public FrHelper(FrOption option)
        {
            this.option = option;
            cfg = new Cognitec.FRsdk.Configuration(@"C:\FVSDK_9_3_1\etc\frsdk.cfg");
            cfg.setValue("FRSDK.FaceTracker.FaceFinderRange.MinEyeDistance", option.MinEyeRatio.ToString());
            cfg.setValue("FRSDK.FaceTracker.FaceFinderRange.MaxEyeDistance", option.MaxEyeRatio.ToString());

            firBuilder = new Cognitec.FRsdk.FIRBuilder(cfg);
            faceTracker = new Cognitec.FRsdk.Face.Tracker(cfg);
            eyesFinder = new Cognitec.FRsdk.Eyes.Finder(cfg);
            enrollProc = new Cognitec.FRsdk.Enrollment.Processor(cfg);
            tokenCreator = new Cognitec.FRsdk.ISO_19794_5.TokenFace.Creator(cfg);
            pc = new Cognitec.FRsdk.Portrait.Analyzer(cfg);
        }

        public Cognitec.FRsdk.FIR BuildFir(byte[] data)
        {
            using (MemoryStream ms = new MemoryStream(data))
                return firBuilder.build(ms);
        }

        byte[] ToBytes(Cognitec.FRsdk.Image image)
        {
            using (MemoryStream ms = new MemoryStream())
            {
                Cognitec.FRsdk.Jpeg.save(image, ms, 80);
                return ms.ToArray();
            }
        }

        byte[] ToBytes(Cognitec.FRsdk.FIR fir)
        {
            using (MemoryStream ms = new MemoryStream())
            {
                fir.serialize(ms);
                return ms.ToArray();
            }
        }

        EnrollmentFeedback Enroll(Cognitec.FRsdk.AnnotatedImage aimg)
        {
            EnrollmentFeedback feedback = new EnrollmentFeedback();
            enrollProc.process(new Cognitec.FRsdk.Sample[1] { new Cognitec.FRsdk.Sample(aimg) }, feedback);
            return feedback;
        }

        public AnalyzeResult Analyze(Cognitec.FRsdk.Image image, double timestamp)
        {
            var cropResults = new List<CropResult>();

            var faces = faceTracker.processFrame(image);
            for (int i = 0; i < faces.Length; i++)
            {
                var eyes = eyesFinder.find(image, faces[i].location);
                if (eyes.Length == 0)
                    continue;

                var aimg = new Cognitec.FRsdk.AnnotatedImage(image, eyes[0]);
                var feedback = Enroll(aimg);
                if (feedback.Fir == null)
                    continue;

                var crop = tokenCreator.extract(aimg);
                var token = tokenCreator.extractMinimal(aimg);
                var character = pc.analyze(aimg);

                var cropResult = new CropResult()
                {
                    ObjectID = faces[i].id,
                    Bounds = feedback.Bounds,
                    Fir = ToBytes(feedback.Fir),
                    Crop = ToBytes(crop.image),
                    Token = ToBytes(token.image),
                    EYE_DIST = character.eyeDistance(),
                    FRONTAL = character.deviationFromFrontalPose()
                };

                cropResults.Add(cropResult);
            }

            if (cropResults.Count == 0)
                return null;

            return new AnalyzeResult()
            {
                Snapshot = new SnapshotResult()
                {
                    Image = ToBytes(image),
                    Timestamp = timestamp
                },
                Crop = cropResults.ToArray()
            };
        }

        public float Compare(Cognitec.FRsdk.FIR firA, Cognitec.FRsdk.FIR firB)
        {
            return fme.compare(firA, firB).value;
        }
    }
}
