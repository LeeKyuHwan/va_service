using OpenCvSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace KistService
{
    public class FaceDetector
    {
        #region libfacedetector
        [DllImport("libfacedetector.dll")]
        internal static extern FaceDetector.FDRuntimeOptions FDDefaultRuntimeOptions();

        [DllImport("libfacedetector.dll")]
        internal static extern FaceDetector.FDConfiguration FDDefaultConfiguration(
          IntPtr faceDetector);

        [DllImport("libfacedetector.dll")]
        internal static extern IntPtr FDDetectFaces(
          IntPtr faceDetector,
          IntPtr picture,
          long width,
          long height,
          long lineSize,
          FaceDetector.FDColorSpace colorSpace,
          ref FaceDetector.FDConfiguration configuration,
          ref long numberOfFacesDetected,
          ref FaceDetector.FDStatus status);

        [DllImport("libfacedetector.dll")]
        internal static extern void FDDestroyFaces(IntPtr faces);

        [DllImport("libfacedetector.dll")]
        internal static extern IntPtr FDCreate(
          string modelPath,
          FaceDetector.FDModel environment,
          int streamId,
          ref FaceDetector.FDRuntimeOptions runtimeOptions);

        [DllImport("libfacedetector.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void FDDestroy(IntPtr faceDetector);
        #endregion

        #region Enum
        internal enum FDModel
        {
            FD_MTCNN,
            FD_RNCNN,
        }

        internal enum FDColorSpace
        {
            FD_GRAY,
            FD_RGBA,
            FD_RGB,
            FD_BGRA,
            FD_BGR,
        }

        internal enum FDInterpolation
        {
            FD_NEAREST_NEIGHBOR,
            FD_BILINEAR,
            FD_AREA,
            FD_BICUBIC,
            FD_LANCZOS4,
        }

        internal enum FDStatus
        {
            FD_STATUS_OK,
            FD_STATUS_ERROR,
            FD_STATUS_ERORR_OUT_OF_MEMORY,
            FD_GPU_UNSUPPORTED,
        }

        internal enum FDRuntimeEnvironment
        {
            FD_ENV_AUTO,
            FD_ENV_CPU,
            FD_ENV_GPU,
        }
        #endregion

        #region Struct
        internal struct FDPoint
        {
            internal double x;
            internal double y;
        }

        internal struct FDKeypoints
        {
            internal FaceDetector.FDPoint leftEyeCenter;
            internal FaceDetector.FDPoint rightEyeCenter;
            internal FaceDetector.FDPoint noseTip;
            internal FaceDetector.FDPoint leftMouthCorner;
            internal FaceDetector.FDPoint rightMouthCorner;
        }

        internal struct FDBoundingBox
        {
            internal double relativeOffsetX;
            internal double relativeOffsetY;
            internal double relativeWidth;
            internal double relativeHeight;
        }

        public struct FDFace
        {
            internal double confidance;
            internal double centerPoseQuality;
            internal double sharpnessQuality;
            internal FaceDetector.FDBoundingBox boundingBox;
            internal FaceDetector.FDKeypoints keypoints;
            internal double contrastQuality;
            internal double yaw;
            internal double roll;
            internal double pitch;
        }

        internal struct FDRuntimeOptions
        {
            internal int TFLiteNumberThreadsForBatches;
            internal FaceDetector.FDRuntimeEnvironment environment;
            internal IntPtr aesKey;
            internal long aesKeyLenght;
        }

        internal struct FDConfiguration
        {
            internal FaceDetector.FDInterpolation imagePyramidInterpolation;
            internal FaceDetector.FDInterpolation candidateCropInterpolaton;
            internal int fastResize;
            internal long minSize;
            internal double scaleFactor;
            internal float P_thres;
            internal float R_thres;
            internal float O_thres;
        }
        #endregion

        private FDConfiguration config;
        private IntPtr handle;

        public int Init()
        {
            try
            {
                string directoryName = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                FDRuntimeOptions runtimeOptions = FDDefaultRuntimeOptions();

                handle = FDCreate(directoryName, FDModel.FD_MTCNN, 0, ref runtimeOptions);
                config = FDDefaultConfiguration(handle);
                config.minSize = 30;
                config.O_thres = 0.1f;
                config.P_thres = 0.1f;
                config.R_thres = 0.1f;
            }
            catch
            {
                return -1;
            }

            return 0;
        }

        public void setMinSize(int size)
        {
            config.minSize = size;
        }

        public int FindFace(Mat image, out FDFace face)
        {
            long faceCount = 0;
            FDStatus status = FDStatus.FD_STATUS_OK;
            face = new FDFace() { boundingBox = new FDBoundingBox() };

            try
            {
                IntPtr facePtr = FDDetectFaces(handle, image.Data, image.Width, image.Height, image.Width * image.Channels(), FDColorSpace.FD_BGR, ref config, ref faceCount, ref status);

                if (status != FDStatus.FD_STATUS_OK || faceCount <= 0)
                    return 0;

                int fdSize = Marshal.SizeOf(new FDFace());

                for (int i = 0; i < faceCount; i++)
                {
                    FDFace f = (FDFace)Marshal.PtrToStructure(facePtr + fdSize * i, typeof(FDFace));
                    FDBoundingBox box = f.boundingBox;

                    if (face.boundingBox.relativeWidth < box.relativeWidth)
                        face = f;
                }

                FDDestroyFaces(facePtr);
            }
            catch
            {
                faceCount = 0;
            }

            return (int)faceCount;
        }

        public FDFace[] FindFaceAll(Mat image)
        {
            long faceCount = 0;
            FDStatus status = FDStatus.FD_STATUS_OK;
            List<FDFace> faces = new List<FDFace>();

            try
            {
                IntPtr facePtr = FDDetectFaces(handle, image.Data, image.Width, image.Height, image.Width * image.Channels(), FDColorSpace.FD_BGR, ref config, ref faceCount, ref status);

                if (status != FDStatus.FD_STATUS_OK || faceCount <= 0)
                    return faces.ToArray();

                int fdSize = Marshal.SizeOf(new FDFace());

                for (int i = 0; i < faceCount; i++)
                {
                    FDFace f = (FDFace)Marshal.PtrToStructure(facePtr + fdSize * i, typeof(FDFace));
                    faces.Add(f);
                }

                FDDestroyFaces(facePtr);
            }
            catch
            {
                faces.Clear();
            }

            return faces.ToArray();
        }

        public Mat Crop(Mat image, FDFace face)
        {
            try
            {
                var box = face.boundingBox;
                double w = box.relativeWidth * image.Width;
                double h = box.relativeHeight * image.Height;
                double half_w = w / 2.0d;
                double half_h = h / 2.0d;

                double x = box.relativeOffsetX * image.Width + half_w - half_h;
                double y = box.relativeOffsetY * image.Height;

                if (x < 0 || y < 0 || x + h > image.Width || y + h > image.Height)
                    return null;

                Mat roi = new Mat(image, new Rect((int)x, (int)y, (int)h, (int)h));
                Mat crop = new Mat();
                roi.CopyTo(crop);

                return crop.Resize(new OpenCvSharp.Size(224, 224));
            }
            catch
            {
                return null;
            }
        }

        public void Destroy()
        {
            FDDestroy(handle);
        }
    }
}
