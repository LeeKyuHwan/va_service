using DataContracts;
using log4net;
using OpenCvSharp;
using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace KistService
{
    public class FaceEngine : IDisposable
    {
        static ILog logger = LogManager.GetLogger(typeof(FaceEngine).Name);

        public static FaceEngine Instance = new FaceEngine();
        //FaceDetector detector = new FaceDetector();
        Gallery gallery = new Gallery();

        private FaceEngine()
        {
            var currentLocation = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            Directory.SetCurrentDirectory(currentLocation);

            //bool detecter_init = detector.Init() == 0;
            bool matcher_init = FaceMatcher.init();

            //if ((detecter_init && matcher_init) == false) logger.Error("[FaceEngine] Init Fail");
            if ((matcher_init) == false) logger.Error("[FaceEngine] Init Fail");
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

        public int Add(string image_id)
        {
            using (DBClient dbc = new DBClient())
            {
                var imageData = dbc.GetImage(image_id);
                var fir = Enroll(imageData);
                if (fir != null)
                {
                    dbc.UpdateKistFir(image_id, fir);
                    return gallery.Add(image_id, fir);
                }
                else
                {
                    //using (MemoryStream ms = new MemoryStream(imageData))
                    //using (MemoryStream ms2 = new MemoryStream())
                    //{
                    //    Bitmap origin = new Bitmap(ms);
                    //    float ratio = 600f / origin.Width;
                    //    Bitmap resize = new Bitmap(origin, new System.Drawing.Size((int)(origin.Width * ratio), (int)(origin.Height * ratio)));
                    //    resize.Save(ms2, ImageFormat.Bmp);
                    //    //detector.setMinSize(114);

                    //    fir = Enroll(ms2.ToArray());
                    //    if (fir != null)
                    //    {
                    //        dbc.UpdateKistFir(image_id, fir);
                    //        return gallery.Add(image_id, fir);
                    //    }

                    //    //detector.setMinSize(30);
                    //    origin.Dispose();
                    //    resize.Dispose();
                    //}

                    logger.Error($"[Add] fir is Null");
                    return gallery.Count;
                }
            }
        }

        public int Remove(string image_id)
        {
            return gallery.Remove(image_id);
        }

        public IdentifyResult Identify(string dbscan_id)
        {
            using (DBClient dbc = new DBClient())
            {
                var info = dbc.GetKScanInfo(dbscan_id);
                if (info == null)
                    return IdentifyResult.Fail($"req not found : {dbscan_id}");

                var fir = Enroll(info.FIR); // FIR 이 아닌 Image_data임

                var matches = gallery.Matches(fir, info.THRESHOLD);
                int maxMatches = Math.Min(100, matches.Length);

                dbc.InsertDBScanRslt(dbscan_id, matches.ToList().GetRange(0, maxMatches).ToArray());

                var result = dbc.GetMatchedFaces(dbscan_id);
                return IdentifyResult.Success(result);
            }
        }

        public VerifyResult Verify(string verify_id)
        {
            using (DBClient dbc = new DBClient())
            {
                DataRow reqInfo = dbc.GetVerifyReq(verify_id);

                var image_data1 = dbc.GetImage(reqInfo["FACE_ID1"] as string);
                var image_data2 = dbc.GetImage(reqInfo["FACE_ID2"] as string);

                if (image_data1 == null || image_data2 == null)
                {
                    logger.Error("[Verify] Error : image_data is Null (Check Face_ID)");
                    return VerifyResult.Fail("FACE_ID NOT FOUND");
                }

                var fir_data1 = Enroll(image_data1);
                if (fir_data1 == null) return VerifyResult.Fail("fir_data1 is Null");

                var fir_data2 = Enroll(image_data2);
                if (fir_data2 == null) return VerifyResult.Fail("fir_data2 is Null");

                float tp_score = Verify(fir_data1, fir_data2);

                dbc.InsertVerifyRslt(verify_id, tp_score);

                return VerifyResult.Success(tp_score);
            }
        }

        public unsafe byte[] Enroll(Mat image)
        {
            int faces = FaceMatcher.detect((byte*)image.Data.ToPointer(), image.Width, image.Height);
            if (faces == 0)
            {
                logger.Warn("[Enroll] face not found");
                return null;
            }

            byte[] fir = new byte[FaceMatcher.FEATURE_DIMENSION];
            fixed (byte* pFir = fir)
                FaceMatcher.featExtractFromIndex(0, pFir);

            return fir;
        }

        //public unsafe byte[] Enroll(Mat image, FaceDetector.FDFace face)
        //{
        //    using (Mat crop = detector.Crop(image, face))
        //    {
        //        if (crop == null)
        //        {
        //            logger.Error("[Enroll] crop failed");
        //            return null;
        //        }
        //
        //        byte[] fir = new byte[FaceMatcher.FEATURE_DIMENSION];
        //
        //        try
        //        {
        //            fixed (byte* pFir = fir)
        //                FaceMatcher.featExtractFromData((byte*)crop.Data.ToPointer(), crop.Cols, crop.Rows, pFir);
        //        }
        //        catch (Exception ex)
        //        {
        //            logger.Error($"[Enroll] enroll failed : {ex.Message}");
        //            return null;
        //        }
        //
        //        return fir;
        //    }
        //}

        public unsafe byte[] Enroll(string fileName)
        {
            try
            {
                using (Mat image = Cv2.ImRead(fileName))
                    return Enroll(image);
            }
            catch (Exception ex)
            {
                logger.Error($"[Enroll] enroll failed : {ex.Message}");
                return null;
            }
        }

        public unsafe byte[] Enroll(IntPtr bgrData, int width, int height)
        {
            try
            {
                using (Mat image = new Mat(height, width, MatType.CV_8UC3, bgrData))
                    return Enroll(image);
            }
            catch (Exception ex)
            {
                logger.Error($"[Enroll] enroll failed : {ex.Message}");
                return null;
            }
        }

        public unsafe byte[] Enroll(byte[] imgData)
        {
            byte[] fir = null;

            try
            {
                //using (MemoryStream ms = new MemoryStream(imgData))
                //{
                //    using (Bitmap bmp = new Bitmap(ms))
                //    {
                //        using (Mat image = OpenCvSharp.Extensions.BitmapConverter.ToMat(bmp))
                //            fir = Enroll(image);
                //    }
                //}
                fixed (byte* pImg = imgData)
                {
                    using (Mat image = Cv2.ImDecode(new Mat(1, imgData.Length, MatType.CV_8UC1, new IntPtr(pImg)), ImreadModes.Unchanged))
                    {
                        fir = Enroll(image);
                    }
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Enroll] enroll failed : {ex.Message}");
                return null;
            }

            return fir;
        }

        public unsafe float Verify(byte[] firA, byte[] firB)
        {
            if (firA == null || firB == null)
                return 0f;

            try
            {
                fixed (byte* pFirA = firA)
                {
                    fixed (byte* pFirB = firB)
                    {
                        return FaceMatcher.matching(pFirA, pFirB);
                    }
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Verify] verify failed : {ex.Message}");
                return 0f;
            }
        }

        public void Identify(byte[] fir, out string matched, out float maxScore)
        {
            matched = string.Empty;
            maxScore = 0;

            foreach (var person in gallery.Items)
            {
                float score = Verify(fir, person.FIR);
                if (score > maxScore)
                {
                    matched = person.Key;
                    maxScore = score;
                }
            }
        }

        public void Dispose()
        {
            //detector.Destroy();
            FaceMatcher.destroy();
        }

        public void test()
        {
            string dbscan_id = "K0000000456";

            using (DBClient dbc = new DBClient())
            {
                var info = dbc.GetKScanInfo(dbscan_id);
                if (info == null)
                    return;

                var fir = Enroll(info.FIR); // FIR 이 아닌 Image_data임

                var matches = gallery.Matches(fir, info.THRESHOLD);
                int maxMatches = Math.Min(100, matches.Length);

                dbc.InsertDBScanRslt(dbscan_id, matches.ToList().GetRange(0, maxMatches).ToArray());

                var result = dbc.GetMatchedFaces(dbscan_id);
                return;
            }
        }

        private void test_verify()
        {
            var firA = Enroll(File.ReadAllBytes(@"C:\images\a.jpg"));
            var firB = Enroll(File.ReadAllBytes(@"C:\images\a1.jpg"));

            var score = Verify(firA, firB);
            Debug.WriteLine(score);
        }

        public void FirReset()
        {
            using (DBClient dbc = new DBClient())
            {
                var item = dbc.LoadGalleryIds("B");
                dbc.UpdateKistFirNull();
                for (int i = 0; i < item.Rows.Count; i++) 
                {
                    var unit = item.Rows[i];

                    var id = Convert.ToString(unit["TGTMNG_OBJ_ID"]);
                    byte[] image = (byte[])unit["IMG_DATA"];
                    byte[] fir = Enroll(image);
                    logger.Debug($"FirReset Count[{i}]");
                    dbc.UpdateKistFir(id, fir);
                }
            }
        }
    }
}
