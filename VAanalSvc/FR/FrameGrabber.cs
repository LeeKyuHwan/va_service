using log4net;
using OpenCvSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace VAnalSvc
{
    class FrameGrabber
    {
        static ILog logger = LogManager.GetLogger(nameof(FrameGrabber));

        readonly double rangeFromoMs;
        readonly double rangeToMs;
        readonly VideoCapture video;

        public int Width { get; private set; }
        public int Height { get; private set; }

        List<double> percent = new List<double>();
        public double CurrentPositionMs { get { return video.PosMsec; } }
        public double CurrentPositionSec { get { return CurrentPositionMs / 1000d; } }
        public int ProgressPercent(double sec)
        {
            return percent.FindIndex(x => x >= sec * 1000);
        }

        Mat image = new Mat();
        IntPtr pBih = IntPtr.Zero;
        BITMAPINFOHEADER bih = new BITMAPINFOHEADER();

        public FrameGrabber(string url, double fromSec, double toSec)
        {
            video = new VideoCapture(url);

            rangeFromoMs = fromSec * 1000d;
            rangeToMs = toSec * 1000d;

            Width = video.FrameWidth;
            Height = video.FrameHeight;

            int level = 100;
            double frag = (rangeToMs - rangeFromoMs) / (double)level;
            for (int i = 0; i < level; i++)
                percent.Add(rangeFromoMs + frag * (double)i);

            MoveTo(fromSec);
        }

        public bool IsOpened()
        {
            return video.IsOpened();
        }

        public void MoveTo(double positionSec)
        {
            video.Set(VideoCaptureProperties.PosMsec, positionSec * 1000);
        }

        public Cognitec.FRsdk.Image GetNextFrame()
        {
            if (CurrentPositionMs >= rangeToMs)
                return null;

            if (video.Read(image) == false)
                return null;

            if (pBih == IntPtr.Zero)
            {
                pBih = Marshal.AllocCoTaskMem(Marshal.SizeOf(typeof(BITMAPINFOHEADER)));

                bih.biBitCount = (ushort)(image.Channels() * 8);
                bih.biHeight = -image.Height;
                bih.biWidth = image.Width;
                bih.biPlanes = 1;
                bih.biSizeImage = (uint)(((((bih.biWidth * bih.biBitCount) + 31) & ~31) >> 3) * Math.Abs(bih.biHeight));
                bih.biSize = (uint)Marshal.SizeOf(typeof(BITMAPINFOHEADER));

                Marshal.StructureToPtr(bih, pBih, false);
            }

            return Cognitec.FRsdk.Bmp.load(pBih, image.Data, string.Empty);
        }

        public double SkipFrame(int skipCount)
        {
            video.PosFrames += skipCount;

            return CurrentPositionSec;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct BITMAPINFOHEADER
    {
        public uint biSize;
        public int biWidth;
        public int biHeight;
        public ushort biPlanes;
        public ushort biBitCount;
        public uint biCompression;
        public uint biSizeImage;
        public int biXPelsPerMeter;
        public int biYPelsPerMeter;
        public uint biClrUsed;
        public uint biClrImportant;

        public void Init()
        {
            biSize = (uint)Marshal.SizeOf(this);
        }
    }
}
