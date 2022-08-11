using DataBase;
using DataBase.DataObject;
using DataContracts;
using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VAnalSvc
{
    partial class DBClient
    {
        public CropInfo[] GetRepresentations(string video_id)
        {
            string sql = @"
SELECT VIDEO_ID,
       CROP_ID,
       FIR
  FROM TB_VIDEO_CROP_RSLT
 WHERE VIDEO_ID = :VIDEO_ID
   AND REP_YN = 'Y'
 ORDER BY CROP_ID";

            var dt = Fill(sql, CreateParam("VIDEO_ID", video_id));
            return CropInfo.FromTable(dt);
        }

        public void CreateMatchRslt(CropInfo crop, DBScanResult result)
        {
            string sql = @"
INSERT INTO TB_VIDEO_MATCH_RSLT (CROP_ID, TGTMNG_OBJ_ID, SCORE, RANK)
VALUES (:CROP_ID, :TGTMNG_OBJ_ID, :SCORE, :RANK)
";

            string[] CROP_ID = Enumerable.Repeat(crop.CROP_ID, result.matches.Count).ToArray();
            string[] TGTMNG_OBJ_ID = result.matches.Select(x => x.person_id).ToArray();
            float[] SCORE = result.matches.Select(x => x.score).ToArray();
            int[] RANK = result.matches.Select(x => x.rank).ToArray();

            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                cmd.ArrayBindCount = CROP_ID.Length;

                cmd.Parameters.Add("CROP_ID", OracleDbType.Varchar2, CROP_ID, ParameterDirection.Input);
                cmd.Parameters.Add("TGTMNG_OBJ_ID", OracleDbType.Varchar2, TGTMNG_OBJ_ID, ParameterDirection.Input);
                cmd.Parameters.Add("SCORE", OracleDbType.Single, SCORE, ParameterDirection.Input);
                cmd.Parameters.Add("RANK", OracleDbType.Int32, RANK, ParameterDirection.Input);

                cmd.ExecuteNonQuery();
            }
        }
    }
}
