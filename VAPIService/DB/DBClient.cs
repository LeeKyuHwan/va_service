using DataBase;
using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VAPIService
{
    class DBClient : DBClientBase
    {
        static readonly string connStr = CreateConnectionStr("frs", "frs123!", "aiface.cx2zge1o00bx.ap-northeast-2.rds.amazonaws.com", 1521, "orcl");

        public DBClient() : base(connStr)
        {
        }

        // 영상 분석 목록 조회
        public DataTable GetVanalList(string display, string page, string title, string reg_dt, string vanal_sts_cd)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql1 = @"
SELECT VIDEO_ID,
       VIDEO_URL,
       TITLE,
       PLAY_TIME,
       VANAL_STS_CD,
       DETECT_CNT,
       MATCH_CNT,
       REG_DT,
       REG_USER,
       JOB_PRCNT
  FROM TB_VIDEO_INFO
 WHERE 1 = 1
   AND USE_YN = 'Y'";
            if (!string.IsNullOrWhiteSpace(vanal_sts_cd) && vanal_sts_cd != "A")
            {
                sql1 += @"
   AND VANAL_STS_CD = :VANAL_STS_CD";
                parameters.Add(CreateParam("VANAL_STS_CD", vanal_sts_cd));
            }
            if (!string.IsNullOrWhiteSpace(title))
            {
                sql1 += @"
   AND TITLE LIKE :TITLE";
                parameters.Add(CreateParam("TITLE", $"%{title}%"));
            }
            if (!string.IsNullOrWhiteSpace(reg_dt))
            {
                sql1 += @"
   AND REG_DT >= TO_DATE(:REGDT_FROM, 'YYYYMMDD')";
                sql1 += @" 
   AND REG_DT <= TO_DATE(:REGDT_TO, 'YYYYMMDD') + 1";
                parameters.Add(CreateParam("REGDT_FROM", reg_dt.Split('-')[0]));
                parameters.Add(CreateParam("REGDT_TO", reg_dt.Split('-')[1]));
            }
            sql1 += @"
 ORDER BY REG_DT DESC";

            string sql2 = $@"
SELECT ROWNUM as RNUM,
       VIDEO_ID,
       VIDEO_URL,
       TITLE,
       PLAY_TIME,
       VANAL_STS_CD,
       DETECT_CNT,
       MATCH_CNT,
       REG_DT,
       REG_USER,
       JOB_PRCNT
  FROM ({sql1}
       )
 WHERE 1 = 1";
            int rowStart = 0;
            int rowEnd = 0;

            if (!string.IsNullOrWhiteSpace(display) && !string.IsNullOrWhiteSpace(page))
            {
                int displayInt = Convert.ToInt32(display);
                int pageInt = Convert.ToInt32(page);
                int start = (pageInt - 1) * displayInt + 1;
                int end = pageInt * displayInt;

                rowStart = start > 1 ? start : 1;
                rowEnd = end > 1 ? end : 1;
            }

            if (rowEnd > 0)
            {
                sql2 += @"
   AND ROWNUM <= :ROWEND";
                parameters.Add(CreateParam("ROWEND", rowEnd));
            }

            string sql3 = $@"
SELECT VIDEO_ID, 
       VIDEO_URL, 
       TITLE,
       PLAY_TIME,
       VANAL_STS_CD,
       DETECT_CNT,
       MATCH_CNT,
       TO_CHAR(REG_DT, 'YYYY-MM-DD') REG_DT,
       REG_USER,
       JOB_PRCNT
  FROM ({sql2}
       )
 WHERE 1 = 1";

            if (rowStart > 0)
            {
                sql3 += @" 
   AND RNUM >= :ROWSTART";
                parameters.Add(CreateParam("ROWSTART", rowStart));
            }

            return Fill(sql3, parameters.ToArray());
        }

        // 영상 분석 목록 총 조회 건수
        public int GetVanalListTotal(string title, string reg_dt, string vanal_sts_cd)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
SELECT COUNT(*)
  FROM TB_VIDEO_INFO
 WHERE 1 = 1
   AND USE_YN = 'Y'";
            if (!string.IsNullOrWhiteSpace(title))
            {
                sql += @"
   AND TITLE LIKE :TITLE";
                parameters.Add(CreateParam("TITLE", $"%{title}%"));
            }
            if (!string.IsNullOrWhiteSpace(reg_dt))
            {
                sql += @"
   AND REG_DT >= TO_DATE(:REGDT_FROM, 'YYYYMMDD')";
                sql += @" 
   AND REG_DT <= TO_DATE(:REGDT_TO, 'YYYYMMDD') + 1";
                parameters.Add(CreateParam("REGDT_FROM", reg_dt.Split('-')[0]));
                parameters.Add(CreateParam("REGDT_TO", reg_dt.Split('-')[1]));
            }
            if(!string.IsNullOrWhiteSpace(vanal_sts_cd) && vanal_sts_cd != "A")
            {
                sql += @"
   AND VANAL_STS_CD = :VANAL_STS_CD";
                parameters.Add(CreateParam("VANAL_STS_CD", vanal_sts_cd));
            }
            return Convert.ToInt32(ExecuteScalar(sql, parameters.ToArray()));
        }


        // 영상 분석 결과 - 검출 목록 조회
        public DataTable GetVanalRsltList(string video_id, string display, string page)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
WITH DETECT AS (
SELECT REP_IMG_ID,
       MIN(B.CROP_ID) KEEP(DENSE_RANK FIRST ORDER BY SNAPSHOT_TIME) CROP_ID_FR,
       MAX(B.CROP_ID) KEEP(DENSE_RANK LAST ORDER BY SNAPSHOT_TIME) CROP_ID_TO,
       MIN(SNAPSHOT_TIME) DETECTED_FR,
       MAX(SNAPSHOT_TIME) DETECTED_TO,
       MIN(B.BOUNDS) KEEP(DENSE_RANK FIRST ORDER BY SNAPSHOT_TIME) BOUNDS
  FROM TB_VIDEO_INFO A,
       TB_VIDEO_CROP_RSLT B,
       TB_VIDEO_SNAPSHOT C
 WHERE A.VIDEO_ID = :VIDEO_ID
   AND A.VIDEO_ID = B.VIDEO_ID
   AND B.USE_YN = 'Y'
   AND B.VIDEO_ID = C.VIDEO_ID
   AND B.SNAPSHOT_ID = C.SNAPSHOT_ID
 GROUP BY REP_IMG_ID
)
SELECT *
  FROM (
            SELECT REP_IMG_ID AS CROP_ID,
                   DETECTED_FR,
                   DETECTED_TO,
                   BOUNDS,
                   (SELECT SCORE FROM TB_VIDEO_MATCH_RSLT WHERE CROP_ID = A.REP_IMG_ID AND RANK = 1) SCORE,
                   (SELECT COUNT(1) FROM TB_VIDEO_MATCH_RSLT WHERE CROP_ID = A.REP_IMG_ID) MATCH_CNT,
                   ROW_NUMBER() OVER (ORDER BY DETECTED_FR) RN       
              FROM DETECT A
       ) A
 WHERE 1 = 1";

            parameters.Add(CreateParam("VIDEO_ID", video_id));

            int rowStart = 0;
            int rowEnd = 0;

            if (!string.IsNullOrWhiteSpace(display) && !string.IsNullOrWhiteSpace(page))
            {
                int displayInt = Convert.ToInt32(display);
                int pageInt = Convert.ToInt32(page);
                int start = (pageInt - 1) * displayInt + 1;
                int end = pageInt * displayInt;

                rowStart = start > 1 ? start : 1;
                rowEnd = end > 1 ? end : 1;
            }

            if (rowEnd > 0)
            {
                sql += @"
   AND RN <= :ROWEND";
                parameters.Add(CreateParam("ROWEND", rowEnd));
            }

            if (rowStart > 0)
            {
                sql += @" 
   AND RN >= :ROWSTART";
                parameters.Add(CreateParam("ROWSTART", rowStart));
            }

            return Fill(sql, parameters.ToArray());
        }

        // 영상 분석 결과 - 검출 목록 총 조회 건수
        public int GetVanalRsltListTotal(string video_id)
        {
            string sql = @"
SELECT COUNT(*)
  FROM (
            SELECT /*+ RULE */ REP_IMG_ID
              FROM TB_VIDEO_INFO A,
                   TB_VIDEO_CROP_RSLT B
             WHERE A.VIDEO_ID = :VIDEO_ID
               AND A.VIDEO_ID = B.VIDEO_ID
               AND B.USE_YN = 'Y'
             GROUP BY REP_IMG_ID
 )";

            return Convert.ToInt32(ExecuteScalar(sql, CreateParam("VIDEO_ID", video_id)));
        }

        //이미지 데이터
        public byte[] GetImgData(string image_id)
        {
            string sql = @"
SELECT FN_GET_IMAGE(:ID)
  FROM DUAL";
            return ExecuteScalar(sql, new OracleParameter("ID", image_id)) as byte[];
        }

        public string GetVideoId()
        {
            string sql = @"
SELECT FN_GET_VIDEO_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void AddVideo(Dictionary<string, object> keyValues)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_VIDEO_INFO (
       VIDEO_ID,
       VIDEO_URL,
       FILE_PATH,
       TITLE,
       WIDTH,
       HEIGHT,
       PLAY_TIME,
       MIN_EYE_RATIO,
       MAX_EYE_RATIO,
       SKIP_FRAME,
       THRESHOLD,
       REG_USER
       )
VALUES (
       :VIDEO_ID,
       :VIDEO_URL,
       :FILE_PATH,
       :TITLE,
       :WIDTH,
       :HEIGHT,
       :PLAY_TIME,
       :MIN_EYE_RATIO,
       :MAX_EYE_RATIO,
       :SKIP_FRAME,
       :THRESHOLD,
       :REG_USER
       )";

            parameters.Add(CreateParam("VIDEO_ID", keyValues["video_id"].ToString()));
            parameters.Add(CreateParam("VIDEO_URL", keyValues["url"].ToString()));
            parameters.Add(CreateParam("FILE_PATH", keyValues["file_path"].ToString()));
            parameters.Add(CreateParam("TITLE", keyValues["title"].ToString()));
            parameters.Add(CreateParam("WIDTH", keyValues["width"].ToString()));
            parameters.Add(CreateParam("HEIGHT", keyValues["height"].ToString()));
            parameters.Add(CreateParam("PLAY_TIME", keyValues["play_time"].ToString()));
            double min_eye = Convert.ToDouble(keyValues["min_eye"]) / Convert.ToDouble(keyValues["width"]);
            parameters.Add(CreateParam("MIN_EYE_RATIO", min_eye.ToString("F3")));
            parameters.Add(CreateParam("MAX_EYE_RATIO", keyValues["max_eye"].ToString()));
            parameters.Add(CreateParam("SKIP_FRAME", keyValues["frame_skip"].ToString()));
            parameters.Add(CreateParam("THRESHOLD", keyValues["threshold"].ToString()));
            parameters.Add(CreateParam("REG_USER", keyValues["user"].ToString()));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public void UdtVideoUseYN(string video_id, string USE_YN)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
UPDATE TB_VIDEO_INFO
   SET USE_YN = :USE_YN
 WHERE VIDEO_ID = :VIDEO_ID";
            parameters.Add(new OracleParameter("VIDEO_ID", video_id));
            parameters.Add(new OracleParameter("USE_YN", USE_YN));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public string GetVideoPath(string video_id)
        {
            string sql = @"
SELECT FILE_PATH
  FROM TB_VIDEO_INFO
 WHERE VIDEO_ID = :VIDEO_ID";

            return ExecuteScalar(sql, new OracleParameter("VIDEO_ID", video_id)) as string;
        }

        public string GetVsearchReqId()
        {
            string sql = @"
SELECT FN_GET_VSEARCH_REQ_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void InsertVsearchReq(Dictionary<string, object> keyValues)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_VSEARCH_REQ (
       VSEARCH_REQ_ID,
       IMAGE_ID,
       VIDEO_DT_FR,
       VIDEO_DT_TO,
       THRESHOLD,
       REQ_USER_ID 
       )
VALUES (
       :VSEARCH_REQ_ID,
       :IMAGE_ID,
       TO_DATE(:VIDEO_DT_FR, 'YYYYMMDD'),
       TO_DATE(:VIDEO_DT_TO, 'YYYYMMDD'),
       :THRESHOLD,
       :REQ_USER_ID 
       )";
            string[] regDt = keyValues["reg_dt"].ToString().Split('-');
            string regDtFr = regDt[0];
            string regDtTo = regDt[1];

            parameters.Add(CreateParam("VSEARCH_REQ_ID", keyValues["vsearch_req_id"].ToString()));
            parameters.Add(CreateParam("IMAGE_ID", keyValues["image_id"].ToString()));
            parameters.Add(CreateParam("VIDEO_DT_FR", regDtFr));
            parameters.Add(CreateParam("VIDEO_DT_TO", regDtTo));
            parameters.Add(CreateParam("THRESHOLD", keyValues["threshold"].ToString()));
            parameters.Add(CreateParam("REQ_USER_ID", keyValues["user_id"].ToString()));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public DataTable GetVsearchRslt(string vsearchReqId)
        {
            string sql = @"
SELECT /*+ RULE */ 
       A.VIDEO_ID,
       A.TITLE,
       A.VIDEO_URL,
       TO_CHAR(A.REG_DT, 'YYYY-MM-DD') REG_DT,
       B.DETECT_CNT
  FROM TB_VIDEO_INFO A,
       (
            SELECT VIDEO_ID, COUNT(1) DETECT_CNT
              FROM TB_VSEARCH_RSLT
             WHERE VSEARCH_REQ_ID = :VSEARCH_REQ_ID
             GROUP BY VIDEO_ID
       ) B 
 WHERE A.VIDEO_ID = B.VIDEO_ID   
 ORDER BY A.REG_DT";

            return Fill(sql, new OracleParameter("VSEARCH_REQ_ID", vsearchReqId));
        }

        public DataTable GetVsearchDtlRslt(string vsearch_id, string video_id)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
SELECT REP_IMG_ID CROP_ID,
        (SELECT SCORE FROM TB_VSEARCH_RSLT WHERE VSEARCH_REQ_ID = :VSEARCH_REQ_ID AND CROP_ID = REP_IMG_ID) SCORE,
        MIN(B.CROP_ID) KEEP(DENSE_RANK FIRST ORDER BY SNAPSHOT_TIME) CROP_ID_FR,
        MAX(B.CROP_ID) KEEP(DENSE_RANK LAST ORDER BY SNAPSHOT_TIME) CROP_ID_TO,
        MIN(SNAPSHOT_TIME) DETECTED_FR,
        MAX(SNAPSHOT_TIME) DETECTED_TO,
        MIN(B.BOUNDS) KEEP(DENSE_RANK FIRST ORDER BY SNAPSHOT_TIME) BOUNDS
    FROM TB_VIDEO_SNAPSHOT A, 
        TB_VIDEO_CROP_RSLT B,
        TB_VSEARCH_RSLT C
WHERE A.VIDEO_ID = :VIDEO_ID
    AND A.SNAPSHOT_ID = B.SNAPSHOT_ID
    AND A.VIDEO_ID = B.VIDEO_ID
    AND B.CROP_ID = C.CROP_ID
    AND B.USE_YN='Y'       
    AND C.VSEARCH_REQ_ID = :VSEARCH_REQ_ID
GROUP BY B.REP_IMG_ID
";

            parameters.Add(new OracleParameter("VIDEO_ID", video_id));
            parameters.Add(new OracleParameter("VSEARCH_REQ_ID", vsearch_id));

            return Fill(sql, parameters.ToArray());

        }

        public string GetIdentifyReqId()
        {
            string sql = @"
SELECT FN_GET_DBSCAN_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public DataTable GetIdentifyRslt(string image_id)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
SELECT B.TGTMNG_OBJ_ID,
       CROP_ID,
       EK_NM,
       SEX_CD,
       TO_CHAR(TO_DATE(BIRTH_YMD, 'YYYYMMDD'), 'YYYY-MM-DD') BIRTH_YMD,
       SCORE
  FROM TB_VIDEO_MATCH_RSLT A,
       TB_TGTMNG_OBJ_INFO B
 WHERE A.CROP_ID = :IMAGE_ID
   AND A.TGTMNG_OBJ_ID = B.TGTMNG_OBJ_ID
 ORDER BY SCORE DESC";

            parameters.Add(CreateParam("IMAGE_ID", image_id));

            return Fill(sql, parameters.ToArray());
        }

        public DataTable GetPersonInfo(string TGTMNG_OBJ_ID)
        {
            string sql = @"
SELECT TGTMNG_OBJ_ID,
       EK_NM,
       SEX_CD,
       BIRTH_YMD,
       REG_EMP_NO
  FROM TB_TGTMNG_OBJ_INFO
 WHERE TGTMNG_OBJ_ID = :TGTMNG_OBJ_ID";

            return Fill(sql, new OracleParameter("TGTMNG_OBJ_ID", TGTMNG_OBJ_ID));
        }

        public DataTable GetGroups()
        {
            string sql = @"
SELECT TGT_MNGGRP_ID,
       TGT_MNGGRP_CD,
       MNGGRP_STS_CD,
       TGT_MNGGRP_NM,
       (SELECT COUNT(*) FROM TB_TGTMNG_OBJ_INFO A,
       TB_TGT_MNGGRP_INFO B
 WHERE A.TGT_MNGGRP_ID = B.TGT_MNGGRP_ID
   AND B.TGT_MNGGRP_CD = TGT_MNGGRP_CD) ENROLL_SUCSS_CNT
  FROM TB_TGT_MNGGRP_INFO
 WHERE MNGGRP_STS_CD = 'S'";

            return Fill(sql);
        }

        public DataTable GetPersonList(string group_cd)
        {
            string sql = @"
SELECT TGTMNG_OBJ_ID, 
       EK_NM, 
       SEX_CD,
       A.REG_EMP_NO
  FROM TB_TGTMNG_OBJ_INFO A,
       TB_TGT_MNGGRP_INFO B
 WHERE A.TGT_MNGGRP_ID = B.TGT_MNGGRP_ID
   AND B.TGT_MNGGRP_CD = :TGT_MNGGRP_CD
 ORDER BY EK_NM";

            return Fill(sql, new OracleParameter("TGT_MNGGRP_CD", group_cd));
        }

        public string GetDetectReqId()
        {
            string sql = @"
SELECT FN_GET_DETECT_REQ_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void InsertDetectReq(string detect_id, byte[] imgData, string min_eye_dist)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = $@"
INSERT INTO TB_DETECT_REQ (
       REQ_ID,
       REQ_IMAGE,
       MIN_EYE_DIST
       )
VALUES (
       :REQ_ID,
       :REQ_IMAGE,
       :MIN_EYE_DIST
       )";

            parameters.Add(new OracleParameter("REQ_ID", detect_id));
            parameters.Add(new OracleParameter("REQ_IMAGE", imgData));
            parameters.Add(new OracleParameter("MIN_EYE_DIST", min_eye_dist));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public string GetVerifyId()
        {
            string sql = @"
SELECT FN_GET_VERIFY_REQ_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void InsertVerifyReq(string verify_id, string face_id1, string face_id2, string cognitecVer = null)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = $@"
INSERT INTO TB_{(string.IsNullOrEmpty(cognitecVer) ? "" : cognitecVer + "_")}VERIFY_REQ (
       REQ_ID,
       FACE_ID1,
       FACE_ID2
       )
VALUES (
       :REQ_ID,
       :FACE_ID1,
       :FACE_ID2
       )";

            parameters.Add(new OracleParameter("REQ_ID", verify_id));
            parameters.Add(new OracleParameter("FACE_ID1", face_id1));
            parameters.Add(new OracleParameter("FACE_ID2", face_id2));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public string GetTargetId()
        {
            string sql = @"
SELECT FN_GET_TGTMNG_OBJ_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void AddPerson(Dictionary<string, dynamic> data)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = $@"
INSERT INTO TB_TGTMNG_OBJ_INFO (
       TGT_MNGGRP_ID,
       TGTMNG_OBJ_ID,
       EK_NM,
       BIRTH_YMD, 
       SEX_CD,
       ETC_REF_DSC_01,
       IMG_DATA,
       REG_EMP_NO
       )
VALUES (
       :TGT_MNGGRP_ID,
       :TGTMNG_OBJ_ID,
       :EK_NM,
       :BIRTH_YMD, 
       :SEX_CD,
       :ETC_REF_DSC_01,
       (SELECT CROP_IMG FROM TB_DETECT_RSLT WHERE FACE_ID = :FACE_ID),
       :REG_EMP_NO
       )";

            parameters.Add(new OracleParameter("TGT_MNGGRP_ID", data["group_id"]));
            parameters.Add(new OracleParameter("TGTMNG_OBJ_ID", data["tgtmng_obj_id"]));
            parameters.Add(new OracleParameter("EK_NM", data["name"]));
            parameters.Add(new OracleParameter("BIRTH_YMD", data["birth_ymd"]));
            parameters.Add(new OracleParameter("SEX_CD", data["sex"]));
            parameters.Add(new OracleParameter("ETC_REF_DSC_01", data["etc"]));
            parameters.Add(new OracleParameter("FACE_ID", data["face_id"]));
            parameters.Add(new OracleParameter("REG_EMP_NO", data["usr_id"]));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public void RemovePerson(string person_id, string usr_id = null)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
DELETE FROM TB_TGTMNG_OBJ_INFO 
 WHERE TGTMNG_OBJ_ID = :TGTMNG_OBJ_ID";

            parameters.Add(new OracleParameter("TGTMNG_OBJ_ID", person_id));
            if (!string.IsNullOrEmpty(usr_id))
            {
                sql += @"
   AND REG_EMP_NO = :REG_EMP_NO";
                parameters.Add(new OracleParameter("REG_EMP_NO", usr_id));
            }

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public DataRow GetUserInfo(string usr_id)
        {
            string sql = @"
SELECT USR_ID,
       USR_NM,
       AUTH_ID
  FROM USR_INFO
 WHERE USR_ID = :USR_ID";

            DataTable dt = Fill(sql, new OracleParameter("USR_ID", usr_id));

            return dt.Rows.Count == 1 ? dt.Rows[0] : null;
        }

        public void InsertDBScanReq(string dbscan_id, string group_id, string image_id, string threshold, string cognitecVer = null)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = $@"
INSERT INTO TB_{(string.IsNullOrEmpty(cognitecVer) ? "" : cognitecVer + "_")}DBSCAN_REQ (
       DBSCAN_ID,
       TGT_MNGGRP_ID,
       IMAGE_ID,
       THRESHOLD
       )
VALUES (
       :DBSCAN_ID,
       :TGT_MNGGRP_ID,
       :IMAGE_ID,
       :THRESHOLD
       )";
            parameters.Add(new OracleParameter("DBSCAN_ID", dbscan_id));
            parameters.Add(new OracleParameter("TGT_MNGGRP_ID", group_id));
            parameters.Add(new OracleParameter("IMAGE_ID", image_id));
            parameters.Add(new OracleParameter("THRESHOLD", threshold));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public void SetSafrId(string person_id, string safr_id)
        {
            string sql = @"
UPDATE TB_TGTMNG_OBJ_INFO 
   SET ETC_REF_DSC_05 = :ETC_REF_DSC_05
 WHERE TGTMNG_OBJ_ID = :TGTMNG_OBJ_ID";

            ExecuteNonQuery(sql, new OracleParameter("ETC_REF_DSC_05", safr_id), new OracleParameter("TGTMNG_OBJ_ID", person_id));
        }

        public List<string> GetPersonIdsMatchedSafrId(string safr_id)
        {
            string sql = @"
SELECT TGTMNG_OBJ_ID
  FROM TB_TGTMNG_OBJ_INFO
 WHERE ETC_REF_DSC_05 = :ETC_REF_DSC_05";
            DataTable dt = Fill(sql, new OracleParameter("ETC_REF_DSC_05", safr_id));

            List<string> list = new List<string>();

            foreach(DataRow row in dt.Rows)
            {
                list.Add(Convert.ToString(row["TGTMNG_OBJ_ID"]));
            }

            return list;
        }

        public string GetSafrId(string person_id)
        {
            string sql = @"
SELECT ETC_REF_DSC_05
  FROM TB_TGTMNG_OBJ_INFO
 WHERE TGTMNG_OBJ_ID = :TGTMNG_OBJ_ID";

            return ExecuteScalar(sql, new OracleParameter("TGTMNG_OBJ_ID", person_id)) as string;
        }

        public string GetKVerifyId()
        {
            string sql = @"
SELECT FN_GET_KVERIFY_REQ_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void InsertKVerifyReq(string verify_id, string face_id1, string face_id2)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_KVERIFY_REQ (
       REQ_ID,
       FACE_ID1,
       FACE_ID2
       )
VALUES (
       :REQ_ID,
       :FACE_ID1,
       :FACE_ID2
       )";

            parameters.Add(new OracleParameter("REQ_ID", verify_id));
            parameters.Add(new OracleParameter("FACE_ID1", face_id1));
            parameters.Add(new OracleParameter("FACE_ID2", face_id2));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public string GetKScanReqId()
        {
            string sql = @"
SELECT FN_GET_KSCAN_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void InsertKScanReq(string dbscan_id, string group_id, string image_id, string threshold)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_KSCAN_REQ (
       KSCAN_ID,
       TGT_MNGGRP_ID,
       IMAGE_ID,
       THRESHOLD
       )
VALUES (
       :KSCAN_ID,
       :TGT_MNGGRP_ID,
       :IMAGE_ID,
       :THRESHOLD
       )";
            parameters.Add(new OracleParameter("KSCAN_ID", dbscan_id));
            parameters.Add(new OracleParameter("TGT_MNGGRP_ID", group_id));
            parameters.Add(new OracleParameter("IMAGE_ID", image_id));
            parameters.Add(new OracleParameter("THRESHOLD", threshold));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public DataTable GetTest()
        {
            string sql = @"
SELECT TGTMNG_OBJ_ID, IMG_DATA, EK_NM, BIRTH_YMD, SEX_CD, ETC_REF_DSC_01
FROM TB_TGTMNG_OBJ_INFO
WHERE TGT_MNGGRP_ID = 'G000003'
and kist_fir_val is null";

            return Fill(sql);
        }

        public string GetFVerifyId()
        {
            string sql = @"
SELECT FN_GET_FVERIFY_REQ_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void InsertFVerifyReq(string verify_id, string face_id1, string face_id2)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_FVERIFY_REQ (
       REQ_ID,
       FACE_ID1,
       FACE_ID2
       )
VALUES (
       :REQ_ID,
       :FACE_ID1,
       :FACE_ID2
       )";

            parameters.Add(new OracleParameter("REQ_ID", verify_id));
            parameters.Add(new OracleParameter("FACE_ID1", face_id1));
            parameters.Add(new OracleParameter("FACE_ID2", face_id2));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public string GetFScanReqId()
        {
            string sql = @"
SELECT FN_GET_FSCAN_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void InsertFScanReq(string dbscan_id, string group_id, string image_id, string threshold)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_FSCAN_REQ (
       FSCAN_ID,
       TGT_MNGGRP_ID,
       IMAGE_ID,
       THRESHOLD
       )
VALUES (
       :FSCAN_ID,
       :TGT_MNGGRP_ID,
       :IMAGE_ID,
       :THRESHOLD
       )";
            parameters.Add(new OracleParameter("FSCAN_ID", dbscan_id));
            parameters.Add(new OracleParameter("TGT_MNGGRP_ID", group_id));
            parameters.Add(new OracleParameter("IMAGE_ID", image_id));
            parameters.Add(new OracleParameter("THRESHOLD", threshold));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public UsrSettings GetUsrSettings(string user_id, string setting_cd)
        {
            string sql = @"
SELECT USR_ID, SETTINGS_CD, SETTINGS_STR
  FROM USR_SETTINGS
 WHERE USR_ID = :USR_ID 
   AND SETTINGS_CD = :SETTINGS_CD";

            DataTable dt = Fill(sql, CreateParam("USR_ID", user_id), CreateParam("SETTINGS_CD", setting_cd));

            return dt.Rows.Count == 1 ? new UsrSettings(dt.Rows[0]) : null;
        }

        public string GetSScanReqId()
        {
            string sql = @"
SELECT FN_GET_SSCAN_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void InsertSScanReq(string dbscan_id, string group_id, string image_id, string threshold)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_SSCAN_REQ (
       SSCAN_ID,
       TGT_MNGGRP_ID,
       IMAGE_ID,
       THRESHOLD
       )
VALUES (
       :SSCAN_ID,
       :TGT_MNGGRP_ID,
       :IMAGE_ID,
       :THRESHOLD
       )";
            parameters.Add(new OracleParameter("SSCAN_ID", dbscan_id));
            parameters.Add(new OracleParameter("TGT_MNGGRP_ID", group_id));
            parameters.Add(new OracleParameter("IMAGE_ID", image_id));
            parameters.Add(new OracleParameter("THRESHOLD", threshold));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public string GetSVerifyId()
        {
            string sql = @"
SELECT FN_GET_SVERIFY_REQ_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void InsertSVerifyReq(string verify_id, string face_id1, string face_id2)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_SVERIFY_REQ (
       REQ_ID,
       FACE_ID1,
       FACE_ID2
       )
VALUES (
       :REQ_ID,
       :FACE_ID1,
       :FACE_ID2
       )";

            parameters.Add(new OracleParameter("REQ_ID", verify_id));
            parameters.Add(new OracleParameter("FACE_ID1", face_id1));
            parameters.Add(new OracleParameter("FACE_ID2", face_id2));

            ExecuteNonQuery(sql, parameters.ToArray());
        }
    }
}
