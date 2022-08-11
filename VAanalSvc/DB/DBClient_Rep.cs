using DataBase;
using DataBase.DataObject;
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
        public void UpdatePCropId(string video_id, DataTable dt)
        {
            string sql = @"
UPDATE TB_VIDEO_CROP_RSLT
   SET P_CROP_ID = :P_CROP_ID
 WHERE VIDEO_ID = :VIDEO_ID
   AND CROP_ID = :CROP_ID
";

            string[] video_ids = Enumerable.Repeat(video_id, dt.Rows.Count).ToArray();
            string[] p_crop_id = Pivot<string>(dt, "P_CROP_ID");
            string[] crop_id = Pivot<string>(dt, "CROP_ID");

            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                cmd.ArrayBindCount = video_ids.Length;

                cmd.Parameters.Add("VIDEO_ID", OracleDbType.Varchar2, video_ids, ParameterDirection.Input);
                cmd.Parameters.Add("P_CROP_ID", OracleDbType.Varchar2, p_crop_id, ParameterDirection.Input);
                cmd.Parameters.Add("CROP_ID", OracleDbType.Varchar2, crop_id, ParameterDirection.Input);

                cmd.ExecuteNonQuery();
            }
        }

        public void SetRepresentation(string video_id)
        {
            string sql = @"
WITH REP AS (
SELECT CONNECT_BY_ROOT CROP_ID R_CROP_ID,
       A.*
  FROM (
            SELECT A.CROP_ID,
                   A.P_CROP_ID,
                   A.FRONTAL
              FROM TB_VIDEO_CROP_RSLT A,
                   TB_VIDEO_SNAPSHOT B   
             WHERE A.VIDEO_ID = :VIDEO_ID
               AND A.VIDEO_ID = B.VIDEO_ID
               AND A.SNAPSHOT_ID = B.SNAPSHOT_ID    
             ORDER BY B.SNAPSHOT_TIME, B.SNAPSHOT_ID, A.CROP_ID
       ) A
  START WITH P_CROP_ID IS NULL
CONNECT BY PRIOR CROP_ID = P_CROP_ID
)
SELECT A.CROP_ID,
       B.REP_ID,
       CASE WHEN A.CROP_ID = B.REP_ID THEN 'Y'
            ELSE 'N'
       END REP_YN
  FROM REP A,
       (
            SELECT R_CROP_ID, 
                   MAX(CROP_ID) KEEP(DENSE_RANK FIRST ORDER BY FRONTAL DESC) REP_ID
              FROM REP A
             GROUP BY R_CROP_ID
       ) B
 WHERE A.R_CROP_ID = B.R_CROP_ID
";

            var dt = Fill(sql, CreateParam("VIDEO_ID", video_id));
            if (dt.Rows.Count == 0)
                return;

            var VIDEO_ID = Enumerable.Repeat(video_id, dt.Rows.Count).ToArray();
            var CROP_ID = Pivot<string>(dt, "CROP_ID");
            var REP_ID = Pivot<string>(dt, "REP_ID");
            var REP_YN = Pivot<string>(dt, "REP_YN");

            sql = @"
UPDATE TB_VIDEO_CROP_RSLT
   SET REP_IMG_ID = :REP_ID,
       REP_YN = :REP_YN
 WHERE VIDEO_ID = :VIDEO_ID
   AND CROP_ID = :CROP_ID 
";

            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                cmd.ArrayBindCount = VIDEO_ID.Length;

                cmd.Parameters.Add("REP_ID", OracleDbType.Varchar2, REP_ID, ParameterDirection.Input);
                cmd.Parameters.Add("REP_YN", OracleDbType.Varchar2, REP_YN, ParameterDirection.Input);
                cmd.Parameters.Add("VIDEO_ID", OracleDbType.Varchar2, VIDEO_ID, ParameterDirection.Input);
                cmd.Parameters.Add("CROP_ID", OracleDbType.Varchar2, CROP_ID, ParameterDirection.Input);

                cmd.ExecuteNonQuery();
            }
        }

        public DataTable GetCropResult(string video_id)
        {
            string sql = @"
SELECT /*+ RULE */
       A.SNAPSHOT_ID,
       A.CROP_ID,
       A.FIR,
       A.BOUNDS,
       A.REP_YN,
       A.P_CROP_ID,
       A.REP_IMG_ID,
       B.SNAPSHOT_TIME
  FROM TB_VIDEO_CROP_RSLT A,
       TB_VIDEO_SNAPSHOT B
 WHERE A.VIDEO_ID = :VIDEO_ID
   AND A.FIR_STS_CD = 'S'
   AND A.VIDEO_ID = B.VIDEO_ID
   AND A.SNAPSHOT_ID = B.SNAPSHOT_ID
 ORDER BY B.SNAPSHOT_TIME, SNAPSHOT_ID, CROP_ID
";

            return Fill(sql, CreateParam("VIDEO_ID", video_id));
        }

        public void SetRepresentation(string video_id, DataTable dt)
        {
            string sql = @"
UPDATE TB_VIDEO_CROP_RSLT
   SET REP_YN = :REP_YN,
       REP_IMG_ID = :REP_IMG_ID
 WHERE VIDEO_ID = :VIDEO_ID
   AND CROP_ID = :CROP_ID
";

            string[] VIDEO_ID = Enumerable.Repeat(video_id, dt.Rows.Count).ToArray();
            string[] REP_YN = Pivot<string>(dt, "REP_YN");
            string[] REP_IMG_ID = Pivot<string>(dt, "REP_IMG_ID");
            string[] CROP_ID = Pivot<string>(dt, "CROP_ID");

            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                cmd.ArrayBindCount = VIDEO_ID.Length;

                cmd.Parameters.Add("VIDEO_ID", VIDEO_ID);
                cmd.Parameters.Add("REP_YN", REP_YN);
                cmd.Parameters.Add("REP_IMG_ID", REP_IMG_ID);
                cmd.Parameters.Add("CROP_ID", CROP_ID);

                cmd.ExecuteNonQuery();
            }
        }
    }
}
