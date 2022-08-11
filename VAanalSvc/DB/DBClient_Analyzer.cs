using DataBase;
using DataBase.DataObject;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VAnalSvc
{
    partial class DBClient : DBClientBase
    {
        static readonly string connStr = CreateConnectionStr("frs", "frs123!", "aiface.cx2zge1o00bx.ap-northeast-2.rds.amazonaws.com", 1521, "orcl");

        public DBClient() : base(connStr)
        {
        }

        string GenerateCropID()
        {
            string sql = @"SELECT FN_GET_VIDEO_CROP_ID() FROM DUAL";
            return ExecuteScalar(sql).ToString();
        }

        string GenerateSnapshotID()
        {
            string sql = @"SELECT FN_GET_VIDEO_SNAPSHOT_ID() FROM DUAL";
            return ExecuteScalar(sql).ToString();
        }

        public VideoInfo GetVideoInfo(string video_id)
        {
            string sql = @"
SELECT *
  FROM TB_VIDEO_INFO
 WHERE VIDEO_ID = :VIDEO_ID
";

            DataTable dt = Fill(sql, CreateParam("VIDEO_ID", video_id));

            return dt.Rows.Count == 1 ? new VideoInfo(dt.Rows[0]) : null;
        }

        void InsertSnapshotResult(string video_id, string snapshot_id, SnapshotResult result)
        {
            string sql = @"
INSERT INTO TB_VIDEO_SNAPSHOT (VIDEO_ID, SNAPSHOT_ID, SNAPSHOT_TIME, SNAPSHOT_IMG)
VALUES (:VIDEO_ID, :SNAPSHOT_ID, :SNAPSHOT_TIME, :SNAPSHOT_IMG)
";

            ExecuteNonQuery(sql, CreateParam("VIDEO_ID", video_id),
                                 CreateParam("SNAPSHOT_ID", snapshot_id),
                                 CreateParam("SNAPSHOT_TIME", result.Timestamp),
                                 CreateParam("SNAPSHOT_IMG", result.Image));
        }

        void InsertCropResult(string video_id, string snapshot_id, string crop_id, CropResult result)
        {
            string sql = @"
INSERT INTO TB_VIDEO_CROP_RSLT (VIDEO_ID, SNAPSHOT_ID, CROP_ID, OBJECT_ID, CROP_IMG, TKN_IMG, FIR, FIR_STS_CD, BOUNDS, EYE_DIST, FRONTAL)
VALUES (:VIDEO_ID, :SNAPSHOT_ID, :CROP_ID, :OBJECT_ID, :CROP_IMG, :TKN_IMG, :FIR, :FIR_STS_CD, :BOUNDS, :EYE_DIST, :FRONTAL)
";

            ExecuteNonQuery(sql, CreateParam("VIDEO_ID", video_id),
                                 CreateParam("SNAPSHOT_ID", snapshot_id),
                                 CreateParam("CROP_ID", crop_id),
                                 CreateParam("OBJECT_ID", result.ObjectID),
                                 CreateParam("CROP_IMG", result.Crop),
                                 CreateParam("TKN_IMG", result.Token),
                                 CreateParam("FIR", result.Fir),
                                 CreateParam("FIR_STS_CD", "S"),
                                 CreateParam("BOUNDS", result.Bounds),
                                 CreateParam("EYE_DIST", result.EYE_DIST),
                                 CreateParam("FRONTAL", result.FRONTAL));
        }

        public void InsertAnalyzeResult(string video_id, AnalyzeResult result)
        {
            string snapshot_id = GenerateSnapshotID();
            InsertSnapshotResult(video_id, snapshot_id, result.Snapshot);

            for (int i = 0; i < result.Crop.Length; i++)
            {
                string crop_id = GenerateCropID();
                InsertCropResult(video_id, snapshot_id, crop_id, result.Crop[i]);
            }
        }

        public void UpdateVanalStatus_Start(string video_id)
        {
            string sql = @"
UPDATE TB_VIDEO_INFO
   SET VANAL_STS_CD = :VANAL_STS_CD,
       JOB_PRCNT = :JOB_PRCNT,
       JOB_START_DT = SYSDATE
 WHERE VIDEO_ID = :VIDEO_ID
";
            ExecuteNonQuery(sql, CreateParam("VIDEO_ID", video_id),
                                 CreateParam("JOB_PRCNT", 0),
                                 CreateParam("VANAL_STS_CD", "P"));
        }

        public void UpdateVanalStatus(string video_id, string vanal_sts_cd, int job_prcnt)
        {
            string sql = @"
UPDATE TB_VIDEO_INFO
   SET VANAL_STS_CD = :VANAL_STS_CD,
       JOB_PRCNT = :JOB_PRCNT
 WHERE VIDEO_ID = :VIDEO_ID
";
            ExecuteNonQuery(sql, CreateParam("VIDEO_ID", video_id),
                                 CreateParam("JOB_PRCNT", job_prcnt),
                                 CreateParam("VANAL_STS_CD", vanal_sts_cd));
        }

        public void UpdateVanalStatus_Cmplt(string video_id, string vanal_sts_cd = "S")
        {
            string sql = @"
UPDATE TB_VIDEO_INFO
   SET VANAL_STS_CD = :VANAL_STS_CD,
       JOB_PRCNT = :JOB_PRCNT,
       JOB_CMPLT_DT = SYSDATE,
       DETECT_CNT = (
                        SELECT COUNT(1)
                          FROM TB_VIDEO_CROP_RSLT
                         WHERE USE_YN = 'Y'
                           AND REP_YN = 'Y'
                           AND VIDEO_ID = :VIDEO_ID
                    ),
       MATCH_CNT = (
                        SELECT COUNT(1)
                          FROM TB_VIDEO_CROP_RSLT A,
                               TB_VIDEO_MATCH_RSLT B
                         WHERE A.CROP_ID = B.CROP_ID
                           AND A.USE_YN = 'Y'
                           AND A.REP_YN = 'Y'
                           AND A.VIDEO_ID = :VIDEO_ID
                           AND B.SCORE >= (SELECT THRESHOLD FROM TB_VIDEO_INFO WHERE VIDEO_ID = :VIDEO_ID)
                           AND B.RANK = 1
                   )
 WHERE VIDEO_ID = :VIDEO_ID
";
            ExecuteNonQuery(sql, CreateParam("VIDEO_ID", video_id),
                                 CreateParam("JOB_PRCNT", 100),
                                 CreateParam("VANAL_STS_CD", vanal_sts_cd));
        }
    }
}
