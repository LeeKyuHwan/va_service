using DataBase;
using DataBase.DataObject;
using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VSearchSvc
{
    class DBClient : DBClientBase
    {
        static readonly string connStr = CreateConnectionStr("frs", "frs123!", "aiface.cx2zge1o00bx.ap-northeast-2.rds.amazonaws.com", 1521, "orcl");

        public DBClient() : base(connStr)
        {
        }

        public string[] GetVideos()
        {
            string sql = @"
SELECT VIDEO_ID
  FROM TB_VIDEO_INFO
 WHERE USE_YN = 'Y'
   AND VANAL_STS_CD = 'S'
   AND DETECT_CNT > 0
";

            return QueryArray<string>(sql, "VIDEO_ID");
        }

        public Item[] GetVideo(string video_id)
        {
            string sql = @"
SELECT CROP_ID,
       FIR
  FROM TB_VIDEO_CROP_RSLT
 WHERE VIDEO_ID = :VIDEO_ID
   AND USE_YN = 'Y'
   AND FIR IS NOT NULL
   AND REP_YN = 'Y' 
";

            var dt = Fill(sql, CreateParam("VIDEO_ID", video_id));
            return Item.FromDataTable(dt);
        }

        public VSearchInfo GetVSearchInfo(string vsearch_id)
        {
            string sql = @"
SELECT A.VSEARCH_REQ_ID,
       (CASE WHEN A.IMAGE_ID LIKE 'C%' THEN (SELECT FIR FROM TB_VIDEO_CROP_RSLT WHERE CROP_ID = A.IMAGE_ID)
             ELSE (SELECT FIR FROM TB_DETECT_RSLT WHERE FACE_ID = A.IMAGE_ID)
        END) AS FIR,
       A.THRESHOLD
  FROM TB_VSEARCH_REQ A
 WHERE VSEARCH_REQ_ID = :VSEARCH_REQ_ID
";

            var dt = Fill(sql, CreateParam("VSEARCH_REQ_ID", vsearch_id));

            sql = @"
SELECT VIDEO_ID
  FROM TB_VIDEO_INFO A,
       TB_VSEARCH_REQ B
 WHERE B.VSEARCH_REQ_ID = :VSEARCH_REQ_ID 
   AND A.REG_DT BETWEEN B.VIDEO_DT_FR AND B.VIDEO_DT_TO  
   AND A.VANAL_STS_CD = 'S'
   AND A.USE_YN = 'Y'
   AND A.DETECT_CNT > 0
";

            string[] video_id = QueryArray<string>(sql, "VIDEO_ID", CreateParam("VSEARCH_REQ_ID", vsearch_id));

            return dt.Rows.Count == 1 ? new VSearchInfo(dt.Rows[0], video_id) : null;
        }

        public void InsertVSearchResult(string vsearch_id, VideoMatchResult[] results)
        {
            string sql = @"
INSERT INTO TB_VSEARCH_RSLT (VSEARCH_REQ_ID, VIDEO_ID, CROP_ID, SCORE, RANK)
VALUES (:VSEARCH_REQ_ID, :VIDEO_ID, :CROP_ID, :SCORE, :RANK)
";

            List<string> video_id = new List<string>();
            List<string> image_id = new List<string>();
            List<float> score = new List<float>();
            List<int> rank = new List<int>();

            for (int i = 0; i < results.Length; i++)
            {
                video_id.AddRange(Enumerable.Repeat(results[i].VIDEO_ID, results[i].IMAGE_ID.Length));
                image_id.AddRange(results[i].IMAGE_ID);
                score.AddRange(results[i].SCORE);
                rank.AddRange(Enumerable.Range(1, results[i].IMAGE_ID.Length));
            }

            string[] vsearch_req_id = Enumerable.Repeat(vsearch_id, video_id.Count).ToArray();

            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                cmd.ArrayBindCount = vsearch_req_id.Length;

                cmd.Parameters.Add("VSEARCH_REQ_ID", OracleDbType.Varchar2, vsearch_req_id, ParameterDirection.Input);
                cmd.Parameters.Add("VIDEO_ID", OracleDbType.Varchar2, video_id.ToArray(), ParameterDirection.Input);
                cmd.Parameters.Add("CROP_ID", OracleDbType.Varchar2, image_id.ToArray(), ParameterDirection.Input);
                cmd.Parameters.Add("SCORE", OracleDbType.Single, score.ToArray(), ParameterDirection.Input).DbType = DbType.Single;
                cmd.Parameters.Add("RANK", OracleDbType.Int32, rank.ToArray(), ParameterDirection.Input);

                cmd.ExecuteNonQuery();
            }
        }
    }
}
