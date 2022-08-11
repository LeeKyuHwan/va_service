using Cognitec.FRsdk;
using Cognitec.FRsdk.Enrollment;
using Cognitec.FRsdk.Eyes;
using log4net;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DBScanSvc
{
    class EnrollmentFeedback : Feedback, IDisposable
    {
        public Cognitec.FRsdk.FIR Fir { get; private set; }
        public Location EyeLoc { get; private set; }
        public string Bounds { get; private set; }
        public string FailReason { get; private set; }

        public void Dispose()
        {
            Fir?.Dispose();
        }

        public void end()
        {
        }

        public void eyesFound(Location eyeLoc)
        {
            EyeLoc = eyeLoc;
            int width = (int)(eyeLoc.right.x - eyeLoc.left.x);
            int x1 = (int)(eyeLoc.left.x - width);
            int x2 = (int)(eyeLoc.right.x + width);
            int y1 = (int)(eyeLoc.left.y - width * 1.5f);
            int y2 = (int)(eyeLoc.right.y + width * 2f);
            Bounds = $"{x1},{y1},{x2 - x1},{y2 - y1}";
        }

        public void eyesNotFound()
        {
            FailReason = "eyes not found";
        }

        public void failure()
        {
            FailReason = "face not found";
        }

        public void processingImage(Image img)
        {
        }

        public void start()
        {
        }

        public void success(FIR fir)
        {
            Fir = fir;
        }
    }
}
