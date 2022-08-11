using Cognitec.FRsdk;
using log4net;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VAnalSvc
{
    class AnalyzeResult
    {
        public SnapshotResult Snapshot;
        public CropResult[] Crop;
    }

    class CropResult
    {
        public string ObjectID;
        public string Bounds;
        public byte[] Crop;
        public byte[] Token;
        public byte[] Fir;
        public float EYE_DIST;
        public float FRONTAL;
    }

    class SnapshotResult
    {
        public byte[] Image;
        public double Timestamp;
        public string TimestampString => TimeSpan.FromSeconds(Timestamp).ToString(@"hh\:mm\:ss\.fff");
    }
}
