using DataContracts;
using FrService.Properties;
using log4net;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FrService
{
    class FrHelper
    {
        static ILog logger = LogManager.GetLogger(nameof(FrHelper));

        public static FrHelper Instance = new FrHelper();

        Cognitec.FRsdk.Configuration cfg;
        Cognitec.FRsdk.Face.Finder faceFinder;
        Cognitec.FRsdk.Eyes.Finder eyesFinder;
        Cognitec.FRsdk.Portrait.Analyzer portraitAnalyzer;
        Cognitec.FRsdk.Enrollment.Processor processor;
        Cognitec.FRsdk.ISO_19794_5.TokenFace.Creator tokenCreator;
        Cognitec.FRsdk.FIRBuilder firBuilder;
        Cognitec.FRsdk.FacialMatchingEngine facialMatchingEngine;
        Cognitec.FRsdk.ISO_19794_5.FullFrontal.Test test;

        private FrHelper()
        {
            cfg = new Cognitec.FRsdk.Configuration(@"C:\FVSDK_9_3_1\etc\frsdk.cfg");

            faceFinder = new Cognitec.FRsdk.Face.Finder(cfg);
            eyesFinder = new Cognitec.FRsdk.Eyes.Finder(cfg);
            portraitAnalyzer = new Cognitec.FRsdk.Portrait.Analyzer(cfg);
            processor = new Cognitec.FRsdk.Enrollment.Processor(cfg);
            tokenCreator = new Cognitec.FRsdk.ISO_19794_5.TokenFace.Creator(cfg);
            firBuilder = new Cognitec.FRsdk.FIRBuilder(cfg);
            facialMatchingEngine = new Cognitec.FRsdk.FacialMatchingEngine(cfg);
            test = new Cognitec.FRsdk.ISO_19794_5.FullFrontal.Test(cfg);
        }

        public DetectResult Detect(string detect_id)
        {
            using (DBClient dbc = new DBClient())
            {
                try
                {
                    DetectResult result = new DetectResult();

                    DataRow reqInfo = dbc.GetDetectReq(detect_id);
                    if (reqInfo == null) return DetectResult.Fail("REQ NOT FOUND");

                    bool faceFound = false;

                    Cognitec.FRsdk.Image image = loadImage(reqInfo["REQ_IMAGE"] as byte[]);
                    //ThresholdResult thresholdResult = dbc.GetThreshold(usr_id);

                    float minEyes = Convert.ToSingle(reqInfo["MIN_EYE_DIST"]) / image.width(); //Settings.Default.MinEyeDist / (float)image.width();
                    //float maxEyes = thresholdResult.max_eye_dist / 100; //0.8f;

                    //Cognitec.FRsdk.Face.Location[] faceLocs = faceFinder.find(image, minEyes, maxEyes, 0, 0, (int)image.width(), (int)image.height());
                    Cognitec.FRsdk.Face.Location[] faceLocs = faceFinder.find(image, minEyes, 0.8f, 0, 0, (int)image.width(), (int)image.height());

                    foreach (Cognitec.FRsdk.Face.Location faceLoc in faceLocs)
                    {
                        Cognitec.FRsdk.Eyes.Location[] eyesLocs = eyesFinder.find(image, faceLoc);

                        if (eyesLocs.Length == 0)
                            continue;

                        faceFound = true;
                        Cognitec.FRsdk.AnnotatedImage aimg = new Cognitec.FRsdk.AnnotatedImage(image, eyesLocs[0]);
                        Cognitec.FRsdk.Portrait.Characteristics pc = portraitAnalyzer.analyze(aimg);

                        int size = Convert.ToInt32(faceLoc.width);
                        int width = size * 3;
                        int x = Convert.ToInt32(faceLoc.pos.x - width * (1.0 / 2));
                        int height = Convert.ToInt32(width * (4.0 / 3));
                        int y = Convert.ToInt32(faceLoc.pos.y - height * (1.0 / 2));

                        string tp_box_coord = string.Format("{0},{1},{2},{3}", x, y, width, height);
                        string tp_face_id = dbc.GetDetectRsltID();
                        string tp_sex = pc.isMale() > Settings.Default.SexThreshold ? "M" : "F";
                        int tp_age = (int)pc.age();

                        byte[] cropImgData = GetAnoImgData(aimg);
                        //byte[] firData = GetFIRData(aimg);

                        result.faces.Add(new DetectResult.DetectedFace()
                        {
                            box_coord = tp_box_coord,
                            face_id = tp_face_id,
                            sex = tp_sex,
                            age = tp_age
                        });

                        dbc.InsertDetectRslt(detect_id, tp_face_id, cropImgData, tp_box_coord, tp_sex, tp_age); //firData, tp_box_coord, tp_sex, tp_age);
                    }

                    if (faceFound)
                    {
                        result.result_code = ResultCode.Success;
                        dbc.UpdateDetectReq(detect_id, ResultCode.Success);
                    }
                    else
                    {
                        result.result_code = ResultCode.Fail;
                        dbc.UpdateDetectReq(detect_id, ResultCode.Fail, "FaceNotFound");
                    }

                    return result;
                }
                catch (Exception e)
                {
                    DetectResult result = DetectResult.Fail(e.Message);
                    dbc.UpdateDetectReq(detect_id, ResultCode.Fail, e.Message);

                    return result;
                }
            }
        }

        public VerifyResult Verify(string verify_id)
        {
            using (DBClient dbc = new DBClient())
            {
                DataRow reqInfo = dbc.GetVerifyReq(verify_id);

                DataRow unitInfo1 = dbc.GetUnitInfo(reqInfo["FACE_ID1"] as string);
                DataRow unitInfo2 = dbc.GetUnitInfo(reqInfo["FACE_ID2"] as string);

                Unit unit1 = new Unit()
                {
                    face_id = unitInfo1["ID"] as string,
                    imgData = unitInfo1["IMAGE"] as byte[],
                    firData = unitInfo1["FIR"] as byte[]
                };
                Unit unit2 = new Unit()
                {
                    face_id = unitInfo2["ID"] as string,
                    imgData = unitInfo2["IMAGE"] as byte[],
                    firData = unitInfo2["FIR"] as byte[]
                };

                if (unit1 == null || unit2 == null)
                {
                    logger.Error("[Verify] Error : unit is Null (Check Face_ID)");
                    return VerifyResult.Fail("FACE_ID NOT FOUND");
                }

                bool unitCheck1 = false;
                bool unitCheck2 = false;

                if (unit1.firData == null) unitCheck1 = setUnitFir(unit1);
                else unitCheck1 = true;
                if (unit2.firData == null) unitCheck2 = setUnitFir(unit2);
                else unitCheck2 = true;
                logger.Debug($"[Verify] Unit1[{unitCheck1}], Unit2[{unitCheck2}]");

                if (!(unitCheck1 && unitCheck2))
                {
                    logger.Error("[Verify] Error : UnitSet is Fail");
                    return VerifyResult.Fail("FIR Setting Fail");
                }

                Cognitec.FRsdk.FIR fir1 = GetFIR(unit1.firData);
                Cognitec.FRsdk.FIR fir2 = GetFIR(unit2.firData);

                float tp_score = facialMatchingEngine.compare(fir1, fir2).value;

                fir1.Dispose();
                fir2.Dispose();

                dbc.InsertVerifyRslt(verify_id, tp_score);

                return VerifyResult.Success(tp_score);
            }
        }

        public CharacteristicsResult characteristics(string face_id)
        {
            byte[] imgData;

            using (DBClient dbc = new DBClient())
            {
               imgData = dbc.GetImgData(face_id);
            }

            if (imgData == null) return CharacteristicsResult.Fail("Image Data is Null (Check Face_ID)");

            Cognitec.FRsdk.Image image = GetCognitecImage(imgData);

            float minEyes = Settings.Default.MinEyeDist / (float)image.width();
            float maxEyes = 0.8f;

            Cognitec.FRsdk.Face.Location[] faceLoc = faceFinder.find(image, minEyes, maxEyes, 0, 0, (int)image.width(), (int)image.height());
            Cognitec.FRsdk.Eyes.Location[] eyesLocs = eyesFinder.find(image, faceLoc[0]);
            Cognitec.FRsdk.AnnotatedImage anoImg = new Cognitec.FRsdk.AnnotatedImage(image, eyesLocs[0]);
            Cognitec.FRsdk.Portrait.Characteristics pc = portraitAnalyzer.analyze(anoImg);
            Cognitec.FRsdk.ISO_19794_5.FullFrontal.Compliance compliance = test.assess(pc);

            return new CharacteristicsResult()
            {
                result_code = "S",
                eyes_open = compliance.eyesOpenBestPractice(),
                exposure = compliance.goodExposure(),
                frontal_face = compliance.isFrontalBestPractice(),
                frontal_eyes = compliance.eyesGazeFrontalBestPractice(),
                grayscale = compliance.goodGrayScaleProfile(),
                uniform_lighting = compliance.isLightingUniform(),
                resolution = compliance.resolutionBestPractice(),
                sharpness = compliance.isSharp(),
                mouse_closed = compliance.mouthClosedBestPractice(),
                glasses = pc.glasses() > 0 ? true : false,
                grade = "1"
            };

        }

        public ApiResult UpdateFir(string image_id)
        {
            try
            {
                using (DBClient dbc = new DBClient())
                {
                    DataRow unitInfo = dbc.GetUnitInfo(image_id);

                    Unit unit = new Unit()
                    {
                        face_id = unitInfo["ID"] as string,
                        imgData = unitInfo["IMAGE"] as byte[],
                        firData = unitInfo["FIR"] as byte[]
                    };

                    if (unit.imgData == null) return ApiResult.Fail("imgData is null (Check image_id)");

                    bool unitCheck = false;

                    if (unit.firData == null) unitCheck = setUnitFir(unit);
                    else unitCheck = true;

                    if (unitCheck) return ApiResult.Success;
                    else return ApiResult.Fail("fir set fail");
                }
            }
            catch (Exception ex)
            {
                return ApiResult.Fail(ex.Message);
            }
        }

        private bool setUnitFir(Unit unit)
        {
            Cognitec.FRsdk.Image image = loadImage(unit.imgData);

            Cognitec.FRsdk.Face.Location[] faceLocs = faceFinder.find(image, 0.1f, 0.4f, 0, 0, (int)image.width(), (int)image.height());

            if (faceLocs.Length == 0) return false;
            if (faceLocs.Length > 1) faceLocs = faceLocs.OrderByDescending(item => item.width).ToArray();

            Cognitec.FRsdk.Eyes.Location[] eyesLocs = eyesFinder.find(image, faceLocs[0]);

            if (eyesLocs.Length == 0) return false;

            Cognitec.FRsdk.AnnotatedImage aimg = new Cognitec.FRsdk.AnnotatedImage(image, eyesLocs[0]);
            byte[] firData = GetFIRData(aimg);
            unit.firData = firData;
            using (DBClient dbc = new DBClient())
            {
                dbc.UpdateFir(unit.face_id, firData);
            }

            return true;
        }

        private Cognitec.FRsdk.FIR GetFIR(byte[] firData)
        {
            using (MemoryStream ms = new MemoryStream(firData))
            {
                return firBuilder.build(ms);
            }
        }

        private byte[] GetFIRData(Cognitec.FRsdk.AnnotatedImage annotatedImage)
        {
            using (MemoryStream ms = new MemoryStream())
            {
                Cognitec.FRsdk.Sample[] enrollSample = { new Cognitec.FRsdk.Sample(annotatedImage) };
                EnrollmentFeedback feedback = new EnrollmentFeedback();
                processor.process(enrollSample, feedback);

                feedback.Fir.serialize(ms);
                feedback.Dispose();

                return ms.ToArray();
            }
        }

        private Cognitec.FRsdk.Image loadImage(byte[] imgData)
        {
            byte rotation = readExifRotation(imgData);

            Cognitec.FRsdk.Image img;
            using (MemoryStream ms = new MemoryStream(imgData))
                img = Cognitec.FRsdk.ImageUtils.load(ms);

            switch (rotation)
            {
                case 1:
                    // do nothing
                    break;
                case 8:
                    img = Cognitec.FRsdk.ImageUtils.rotateImage(img, Cognitec.FRsdk.ImageUtils.ImageRotation.ROTATECOUNTERCLOCKWISE);
                    break;
                case 3:
                    img = Cognitec.FRsdk.ImageUtils.rotateImage(img, Cognitec.FRsdk.ImageUtils.ImageRotation.ROTATEUPSIDEDOWN);
                    break;
                case 6:
                    img = Cognitec.FRsdk.ImageUtils.rotateImage(img, Cognitec.FRsdk.ImageUtils.ImageRotation.ROTATECLOCKWISE);
                    break;
                default:
                    break;
            }

            return img;
        }

        private byte readExifRotation(byte[] imgData)
        {
            using (MemoryStream ms = new MemoryStream(imgData))
            {
                System.Drawing.Image img = System.Drawing.Image.FromStream(ms);
                System.Drawing.Imaging.PropertyItem exifProperty = img.PropertyItems.FirstOrDefault(x => x.Id == 0x112);

                return exifProperty == null ? (byte)1 : exifProperty.Value[0];
            }
        }

        private byte[] GetAnoImgData(Cognitec.FRsdk.AnnotatedImage annotatedImage)
        {
            using (MemoryStream ms = new MemoryStream())
            {
                Cognitec.FRsdk.Jpeg.save(tokenCreator.extractMinimal(annotatedImage).image, ms, 100);

                return ms.ToArray();
            }
        }

        private Cognitec.FRsdk.Image GetCognitecImage(byte[] imgData)
        {
            using (MemoryStream ms = new MemoryStream(imgData))
            {
                return Cognitec.FRsdk.ImageUtils.load(ms);
            }
        }
    }
}
