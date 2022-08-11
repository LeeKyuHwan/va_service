using DataContracts;
using FaceMeService.Properties;
using log4net;
using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace FaceMeService
{
    public unsafe class FrHelper : IDisposable
    {
        [DllImport("LibFaceMeSDK.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int init();

        [DllImport("LibFaceMeSDK.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void destroy();

        [DllImport("LibFaceMeSDK.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int enroll(byte* imageData, int imageSize, byte* oFir);

        [DllImport("LibFaceMeSDK.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern float verify(byte* fir1, byte* fir2, int firSize);

        public static readonly FrHelper Instance = new FrHelper();
        private FrHelper() { }

        bool disposed = false;
        ~FrHelper()
        {
            Dispose(false);
        }

        public int InitializeEngine()
        {
            var num = init();
            if (num != 0) logger.Error($"Init Error Number[{num}]");

            return num;
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            if (!disposed)
            {
                if (disposing)
                    destroy();
            }
            disposed = true;
        }

        static ILog logger = LogManager.GetLogger(typeof(FrHelper));
        List<Person> gallery = new List<Person>();

        ReaderWriterLockSlim rwLock = new ReaderWriterLockSlim();

        public byte[] Enroll(byte[] bytes)
        {
            byte[] fir = new byte[8192];
            int firSize = 0;

            fixed (byte* pImage = bytes)
            {
                fixed (byte* pFir = fir)
                {
                    firSize = enroll(pImage, bytes.Length, pFir);
                }
            }

            if (firSize == 0)
                return null;

            byte[] firData = new byte[firSize];
            Buffer.BlockCopy(fir, 0, firData, 0, firSize);

            return firData;
        }

        public float Verify(byte[] fir1, byte[] fir2)
        {
            fixed (byte* pFir1 = fir1)
            {
                fixed (byte* pFir2 = fir2)
                {
                    return verify(pFir1, pFir2, fir1.Length);
                }
            }
        }

        public void Add(byte[] firData, string id)
        {
            gallery.Add(new Person()
            {
                id = id,
                fir = firData
            });
        }

        public Match[] Identify(byte[] fir, float threshold, int maxMatches = 100)
        {
            List<Match> matches = new List<Match>();

            fixed (byte* pFir1 = fir)
            {
                foreach (Person person in gallery)
                {
                    fixed (byte* pFir2 = person.fir)
                    {
                        float score = verify(pFir1, pFir2, fir.Length);
                        if (score >= threshold)
                        {
                            matches.Add(new Match()
                            {
                                id = person.id,
                                score = score
                            });
                        }
                    }
                }
            }

            return matches.OrderByDescending(x => x.score).Take(maxMatches).ToArray();
        }

        public void LoadGallery(string groupCd)
        {
            logger.Info($"[LoadGallery] Group [{groupCd}]");

            try
            {
                using (DBClient dbc = new DBClient())
                {
                    var items = dbc.LoadGallery(groupCd);

                    foreach (DataRow item in items.Rows)
                    {
                        Add(item["FACEME_FIR_VAL"] as byte[], Convert.ToString(item["TGTMNG_OBJ_ID"]));
                    }
                }
            }
            catch (Exception ex)
            {
                logger.Error($"LoadGalley [{ex.Message}]");
            }

            logger.Info($"GalleryCount : {gallery.Count}");
        }

        public int Add(string image_id)
        {
            using (DBClient dbc = new DBClient())
            {
                var imageData = dbc.GetImage(image_id);
                var fir = Enroll(imageData);
                if (fir != null)
                {
                    dbc.UpdateFir(image_id, fir);
                    Add(fir, image_id);

                    return gallery.Count;
                }
                else
                {
                    logger.Error($"[Add] fir is Null");

                    return gallery.Count;
                }
            }
        }

        public int Remove(string image_id)
        {
            int count = 0;
            
            try
            {
                rwLock.EnterWriteLock();

                int idx = gallery.FindIndex(x => x.id.Equals(image_id));
                if (idx >= 0)
                {
                    gallery.RemoveAt(idx);
                    count = gallery.Count;
                    logger.Debug($"[Remove] total count : {count}");
                }
                else
                {
                    logger.Error($"[Remove] key not found : {image_id}");
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Remove] id[{image_id}] : {ex.Message}");
            }
            finally
            {
                rwLock.ExitWriteLock();
            }

            return count;
        }

        public IdentifyResult Identify(string dbscan_id)
        {
            using (DBClient dbc = new DBClient())
            {
                var info = dbc.GetFScanInfo(dbscan_id);
                if (info == null)
                    return IdentifyResult.Fail($"req not found : {dbscan_id}");

                var fir = Enroll(info.FIR); // FIR 이 아닌 Image_data임
                if (fir == null) 
                    return IdentifyResult.Fail($"fir is null : {dbscan_id}");

                var matches = Identify(fir, info.THRESHOLD);
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

        public void ResetFir()
        {
            using (DBClient dbc = new DBClient())
            {
                var items = dbc.test();
        
                foreach (DataRow item in items.Rows)
                {
                    var firData = Enroll(item["IMG_DATA"] as byte[]);

                    if (firData != null)
                        dbc.UpdateFir(Convert.ToString(item["TGTMNG_OBJ_ID"]), firData);
                    else
                        logger.Debug($"id[{Convert.ToString(item["TGTMNG_OBJ_ID"])}]");
                }
            }
        }
    }

    public class Match
    {
        public string id;
        public float score;
    }

    public class Person
    {
        public string id;
        public byte[] fir;
    }
}
