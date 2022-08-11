using DataBase;
using DataBase.DataObject;
using DataContracts;
using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KistService
{
    partial class DBClient : DBClientBase
    {
        static readonly string connStr = CreateConnectionStr("frs", "frs123!", "aiface.cx2zge1o00bx.ap-northeast-2.rds.amazonaws.com", 1521, "orcl");

        public DBClient() : base(connStr)
        {
        }

        public byte[] GetImage(string image_id)
        {
            string sql = @"SELECT FN_GET_IMAGE(:IMAGE_ID) FROM DUAL";
            return (byte[])ExecuteScalar(sql, CreateParam("IMAGE_ID", image_id));
        }

        public OracleParameter[] UpdateFir(string image_id, byte[] fir)
        {
            OracleParameter[] iParams = new OracleParameter[] { CreateParam("I_IMAGE_ID", image_id), CreateParam("I_FIR", fir) };
            OracleParameter[] oParams = new OracleParameter[] { CreateOParam("O_RSLT_MSG_CD", 16), CreateOParam("O_RSLT_MSG", 1024) };
            Procecure("SP_UPDT_FIR", iParams, oParams);

            return oParams;
        }

        public Item[] LoadGallery(string group_cd)
        {
            string sql = @"
SELECT TGTMNG_OBJ_ID,
       KIST_FIR_VAL
  FROM TB_TGTMNG_OBJ_INFO       
 WHERE TGT_MNGGRP_ID = (
                            SELECT TGT_MNGGRP_ID
                              FROM TB_TGT_MNGGRP_INFO
                             WHERE TGT_MNGGRP_CD = :TGT_MNGGRP_CD
                       )
   AND FIR_STS_CD = 'S'
   AND KIST_FIR_VAL IS NOT NULL
";

            DataTable dt = Fill(sql, CreateParam("TGT_MNGGRP_CD", group_cd));
            return Item.FromDataTable(dt);
        }

        //fir 대신 imageData 사용
        public DBScanInfo GetKScanInfo(string dbscan_id)
        {
            string sql = @"
SELECT KSCAN_ID DBSCAN_ID,
       FN_GET_IMAGE(IMAGE_ID) FIR,
       THRESHOLD
  FROM TB_KSCAN_REQ
 WHERE KSCAN_ID = :KSCAN_ID
";
            var dt = Fill(sql, CreateParam("KSCAN_ID", dbscan_id));
            return dt.Rows.Count == 1 ? new DBScanInfo(dt.Rows[0]) : null;
        }

        public void UpdateKistFir(string image_id, byte[] fir)
        {
            string sql = @"
UPDATE TB_TGTMNG_OBJ_INFO
   SET KIST_FIR_VAL = :KIST_FIR_VAL
 WHERE TGTMNG_OBJ_ID = :TGTMNG_OBJ_ID";

            ExecuteNonQuery(sql, CreateParam("KIST_FIR_VAL", fir), CreateParam("TGTMNG_OBJ_ID", image_id));
        }

        public void InsertDBScanRslt(string dbscan_id, Match[] matches)
        {
            string sql = @"
INSERT INTO TB_KSCAN_RSLT (KSCAN_ID, TGTMNG_OBJ_ID, SCORE, RANK)
VALUES (:KSCAN_ID, :TGTMNG_OBJ_ID, :SCORE, :RANK)
";
            if (matches.Count() != 0)
            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                cmd.ArrayBindCount = matches.Length;

                string[] DBSCAN_ID = Enumerable.Repeat(dbscan_id, matches.Length).ToArray();
                string[] TGTMNG_OBJ_ID = matches.Select(x => x.name).ToArray();
                float[] SCORE = matches.Select(x => x.score).ToArray();
                int[] RANK = Enumerable.Range(1, matches.Length).ToArray();

                cmd.Parameters.Add("KSCAN_ID", OracleDbType.Varchar2, DBSCAN_ID, ParameterDirection.Input);
                cmd.Parameters.Add("TGTMNG_OBJ_ID", OracleDbType.Varchar2, TGTMNG_OBJ_ID, ParameterDirection.Input);
                cmd.Parameters.Add("SCORE", OracleDbType.Single, SCORE, ParameterDirection.Input);
                cmd.Parameters.Add("RANK", OracleDbType.Int32, RANK, ParameterDirection.Input);

                cmd.ExecuteNonQuery();
            }

            sql = @"
UPDATE TB_KSCAN_REQ
   SET MATCH_CNT = :MATCH_CNT
 WHERE KSCAN_ID = :KSCAN_ID
";

            ExecuteNonQuery(sql, CreateParam("MATCH_CNT", matches.Length),
                                 CreateParam("KSCAN_ID", dbscan_id));
        }

        public List<IdentifyResult.MatchedFace> GetMatchedFaces(string dbscan_id)
        {
            string sql = @"
SELECT A.TGTMNG_OBJ_ID,
       A.EK_NM,
       A.SEX_CD,
       SUBSTR(A.BIRTH_YMD, 1, 4) || '-' || SUBSTR(A.BIRTH_YMD, 5, 2) || '-' || SUBSTR(A.BIRTH_YMD, 7, 2) AS BIRTH_YMD,
       B.SCORE
  FROM TB_TGTMNG_OBJ_INFO A,
       TB_KSCAN_RSLT B
 WHERE A.TGTMNG_OBJ_ID = B.TGTMNG_OBJ_ID
   AND B.KSCAN_ID = :KSCAN_ID
 ORDER BY B.RANK
";

            var dt = Fill(sql, CreateParam("KSCAN_ID", dbscan_id));
            return IdentifyResult.MatchedFace.FromTable(dt);
        }

        public DataRow GetVerifyReq(string verify_id)
        {
            string sql = @"
SELECT REQ_ID,
       FACE_ID1,
       FACE_ID2
  FROM TB_KVERIFY_REQ
 WHERE REQ_ID = :REQ_ID";

            DataTable dt = Fill(sql, new OracleParameter("REQ_ID", verify_id));

            return dt.Rows.Count == 1 ? dt.Rows[0] : null;
        }

        public void InsertVerifyRslt(string verify_id, float score)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_KVERIFY_RSLT (
       REQ_ID, 
       SCORE
       ) 
VALUES (
       :REQ_ID,
       :SCORE
       )";

            parameters.Add(new OracleParameter("REQ_ID", verify_id));
            parameters.Add(new OracleParameter("SCORE", score));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public DataTable LoadGalleryIds(string group_cd)
        {
            string sql = @"
SELECT TGTMNG_OBJ_ID, IMG_DATA
  FROM TB_TGTMNG_OBJ_INFO       
 WHERE TGT_MNGGRP_ID = (
                            SELECT TGT_MNGGRP_ID
                              FROM TB_TGT_MNGGRP_INFO
                             WHERE TGT_MNGGRP_CD = :TGT_MNGGRP_CD
                       )
   AND FIR_STS_CD = 'S'
";

            return Fill(sql, CreateParam("TGT_MNGGRP_CD", group_cd));
        }

        public void UpdateKistFirNull()
        {
            string sql = @"
UPDATE TB_TGTMNG_OBJ_INFO
   SET KIST_FIR_VAL IS NULL";

            ExecuteNonQuery(sql);
        }
    }
}
