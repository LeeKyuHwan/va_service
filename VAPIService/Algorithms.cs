using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VAPIService.Properties;

namespace VAPIService
{
    public abstract class Algorithms_ast
    {
        public abstract string dbscan_id();
        public abstract void InsertDBScanReq(string dbscan_id, string group_id, string image_id, string threshold);
        public abstract string verify_id();
        public abstract void InsertVerifyReq(string verify_id, string face_id1, string face_id2);
        public string firstAddress { get; set; }
        public string secondAddress { get; set; }
    }

    public class Cognitec_94 : Algorithms_ast
    {
        public Cognitec_94()
        {
            firstAddress = Settings.Default.C94DBScanAddress;
            secondAddress = Settings.Default.C94FrHelperAddress;
        }

        public override string dbscan_id() { using (DBClient dbClient = new DBClient()) return dbClient.GetIdentifyReqId(); }
        public override void InsertDBScanReq(string dbscan_id, string group_id, string image_id, string threshold)
        { using (DBClient dbClient = new DBClient()) dbClient.InsertDBScanReq(dbscan_id, group_id, image_id, threshold); }
        public override string verify_id() { using (DBClient dbClient = new DBClient()) return dbClient.GetVerifyId(); }
        public override void InsertVerifyReq(string verify_id, string face_id1, string face_id2)
        { using (DBClient dbClient = new DBClient()) dbClient.InsertVerifyReq(verify_id, face_id1, face_id2); }
    }

    public class Cognitec_96 : Algorithms_ast
    {
        public Cognitec_96()
        {
            firstAddress = Settings.Default.C96DBScanAddress;
            secondAddress = Settings.Default.C96FrHelperAddress;
        }

        public override string dbscan_id() { using (DBClient dbClient = new DBClient()) return dbClient.GetIdentifyReqId(); }
        public override void InsertDBScanReq(string dbscan_id, string group_id, string image_id, string threshold)
        { using (DBClient dbClient = new DBClient()) dbClient.InsertDBScanReq(dbscan_id, group_id, image_id, threshold, "C96"); }
        public override string verify_id() { using (DBClient dbClient = new DBClient()) return dbClient.GetVerifyId(); }
        public override void InsertVerifyReq(string verify_id, string face_id1, string face_id2)
        { using (DBClient dbClient = new DBClient()) dbClient.InsertVerifyReq(verify_id, face_id1, face_id2, "C96"); }
    }

    public class Safr : Algorithms_ast
    {
        public Safr()
        {
            firstAddress = Settings.Default.SafrApiAddress;
            secondAddress = Settings.Default.SafrApiAddress;
        }

        public override string dbscan_id() { using (DBClient dbClient = new DBClient()) return dbClient.GetSScanReqId(); } 
        public override void InsertDBScanReq(string dbscan_id, string group_id, string image_id, string threshold)
        { using (DBClient dbClient = new DBClient()) dbClient.InsertSScanReq(dbscan_id, group_id, image_id, threshold); }
        public override string verify_id() { using (DBClient dbClient = new DBClient()) return dbClient.GetSVerifyId(); }
        public override void InsertVerifyReq(string verify_id, string face_id1, string face_id2)
        { using (DBClient dbClient = new DBClient()) dbClient.InsertSVerifyReq(verify_id, face_id1, face_id2); }
    }

    public class FaceMe : Algorithms_ast
    {
        public FaceMe()
        {
            firstAddress = Settings.Default.FacemeApiAddress;
            secondAddress = Settings.Default.FacemeApiAddress;
        }

        public override string dbscan_id() { using (DBClient dbClient = new DBClient()) return dbClient.GetFScanReqId(); } 
        public override void InsertDBScanReq(string dbscan_id, string group_id, string image_id, string threshold)
        { using (DBClient dbClient = new DBClient()) dbClient.InsertFScanReq(dbscan_id, group_id, image_id, threshold); }
        public override string verify_id() { using (DBClient dbClient = new DBClient()) return dbClient.GetFVerifyId(); }
        public override void InsertVerifyReq(string verify_id, string face_id1, string face_id2)
        { using (DBClient dbClient = new DBClient()) dbClient.InsertFVerifyReq(verify_id, face_id1, face_id2); }
    }

    public class Kist : Algorithms_ast
    {
        public Kist()
        {
            firstAddress = Settings.Default.KistApiAddress;
            secondAddress = Settings.Default.KistApiAddress;
        }

        public override string dbscan_id() { using (DBClient dbClient = new DBClient()) return dbClient.GetKScanReqId(); }
        public override void InsertDBScanReq(string dbscan_id, string group_id, string image_id, string threshold)
        { using (DBClient dbClient = new DBClient()) dbClient.InsertKScanReq(dbscan_id, group_id, image_id, threshold); }
        public override string verify_id() { using (DBClient dbClient = new DBClient()) return dbClient.GetKVerifyId(); }
        public override void InsertVerifyReq(string verify_id, string face_id1, string face_id2)
        { using (DBClient dbClient = new DBClient()) dbClient.InsertKVerifyReq(verify_id, face_id1, face_id2); }
    }
}
