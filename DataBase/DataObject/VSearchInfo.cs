using DataBase.Extension;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase.DataObject
{
    public class VSearchInfo
    {
        public readonly string VSEARCH_REQ_ID;
        public readonly byte[] FIR;
        public readonly float THRESHOLD;
        public readonly string[] VIDEO_ID;

        public VSearchInfo(DataRow row, string[] video_id)
        {
            VSEARCH_REQ_ID = row.Field<string>("VSEARCH_REQ_ID");
            FIR = (byte[])row["FIR"];
            THRESHOLD = Convert.ToSingle(row["THRESHOLD"]);
            VIDEO_ID = video_id;
        }
    }
}
