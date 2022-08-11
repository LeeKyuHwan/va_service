using DataBase.Extension;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase.DataObject
{
    public class CropInfo
    {
        public readonly string VIDEO_ID;
        public readonly string CROP_ID;
        public readonly byte[] FIR;

        public CropInfo() { }

        public CropInfo(DataRow row)
        {
            VIDEO_ID = row.Field<string>("VIDEO_ID");
            CROP_ID = row.Field<string>("CROP_ID");
            FIR = (byte[])row["FIR"];
        }

        public static CropInfo[] FromTable(DataTable dt)
        {
            CropInfo[] infos = new CropInfo[dt.Rows.Count];

            for (int i = 0; i < dt.Rows.Count; i++)
                infos[i] = new CropInfo(dt.Rows[i]);

            return infos;
        }
    }
}
