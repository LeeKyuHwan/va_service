using OpenCvSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace KistService
{
    unsafe class FaceMatcher
    {
        internal static readonly int FEATURE_DIMENSION = 256 * 4;

        #region FaceMatcher
        [DllImport("libfacematcher.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool init();

        [DllImport("libfacematcher.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void destroy();

        [DllImport("libfacematcher.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int getFeatureDimension();

        [DllImport("libfacematcher.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int detect(byte* data, int width, int height);

        [DllImport("libfacematcher.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void getFaceInfo(int index, byte* pBox);

        [DllImport("libfacematcher.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int featExtractFromIndex(int index, byte* pFir);

        [DllImport("libfacematcher.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern float matching(byte* firA, byte* firB);
        #endregion
    }
}
