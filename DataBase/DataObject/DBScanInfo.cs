using DataBase.Extension;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase.DataObject
{
    public class DBScanInfo
    {
        public readonly string DBSCAN_ID;
        public readonly byte[] FIR;
        public readonly float THRESHOLD;

        public DBScanInfo(DataRow row)
        {
            DBSCAN_ID = row.Field<string>("DBSCAN_ID");
            FIR = row["FIR"] as byte[];
            THRESHOLD = Convert.ToSingle(row["THRESHOLD"]);
        }
    }
}
