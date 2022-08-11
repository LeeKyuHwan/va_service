using DataBase.Extension;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase.DataObject
{
    public class VideoInfo
    {
        public readonly string VIDEO_ID;
        public readonly string FILE_PATH;
        public readonly double PLAY_TIME;
        public readonly float MIN_EYE_RATIO;
        public readonly float MAX_EYE_RATIO;
        public readonly int SKIP_FRAME;
        public readonly float THRESHOLD;

        public VideoInfo() { }

        public VideoInfo(DataRow row)
        {
            VIDEO_ID = row.Field<string>("VIDEO_ID");
            FILE_PATH = row.Field<string>("FILE_PATH");
            PLAY_TIME = (double)row.Default<decimal>("PLAY_TIME", 0);
            MIN_EYE_RATIO = Convert.ToSingle(row["MIN_EYE_RATIO"]);
            MAX_EYE_RATIO = Convert.ToSingle(row["MAX_EYE_RATIO"]);
            SKIP_FRAME = Convert.ToInt32(row["SKIP_FRAME"]);
            THRESHOLD = Convert.ToSingle(row["THRESHOLD"]);
        }

        public static VideoInfo[] FromTable(DataTable dt)
        {
            VideoInfo[] infos = new VideoInfo[dt.Rows.Count];

            for (int i = 0; i < dt.Rows.Count; i++)
                infos[i] = new VideoInfo(dt.Rows[i]);

            return infos;
        }
    }
}
