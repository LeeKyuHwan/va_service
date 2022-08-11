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

namespace SafrService
{
    class DBClient : DBClientBase
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

        public void SetSafrId(string person_id, string safr_id)
        {
            string sql = @"
UPDATE TB_TGTMNG_OBJ_INFO 
   SET ETC_REF_DSC_05 = :ETC_REF_DSC_05
 WHERE TGTMNG_OBJ_ID = :TGTMNG_OBJ_ID";

            ExecuteNonQuery(sql, new OracleParameter("ETC_REF_DSC_05", safr_id), new OracleParameter("TGTMNG_OBJ_ID", person_id));
        }

        public DataTable LoadGallery(string group_cd)
        {
            string sql = @"
SELECT TGTMNG_OBJ_ID,
       ETC_REF_DSC_05
  FROM TB_TGTMNG_OBJ_INFO       
 WHERE TGT_MNGGRP_ID = (
                            SELECT TGT_MNGGRP_ID
                              FROM TB_TGT_MNGGRP_INFO
                             WHERE TGT_MNGGRP_CD = :TGT_MNGGRP_CD
                       )
   AND FIR_STS_CD = 'S'
   AND ETC_REF_DSC_05 IS NOT NULL
";

            return Fill(sql, CreateParam("TGT_MNGGRP_CD", group_cd));
        }

        public PersonObject GetPersonObject(string id)
        {
            string sql = @"
SELECT EK_NM, SEX_CD, BIRTH_YMD, ETC_REF_DSC_01
  FROM TB_TGTMNG_OBJ_INFO
 WHERE TGTMNG_OBJ_ID = :TGTMNG_OBJ_ID";

            var dt = Fill(sql, CreateParam("TGTMNG_OBJ_ID", id));
            
            if (dt.Rows.Count == 1)
            {
                var person = dt.Rows[0];
                return new PersonObject()
                {
                    Name = Convert.ToString(person["EK_NM"]),
                    Sex = Convert.ToString(person["SEX_CD"]),
                    Birth = Convert.ToString(person["BIRTH_YMD"]),
                    Etc = Convert.ToString(person["ETC_REF_DSC_01"])
                };
            }
            return null;
        }

        public DataTable test()
        {
            string sql = @"
SELECT TGTMNG_OBJ_ID
  FROM TB_TGTMNG_OBJ_INFO       
 WHERE TGT_MNGGRP_ID = (
                            SELECT TGT_MNGGRP_ID
                              FROM TB_TGT_MNGGRP_INFO
                             WHERE TGT_MNGGRP_CD = :TGT_MNGGRP_CD
                       )
   AND FIR_STS_CD = 'S'
   AND ETC_REF_DSC_05 IS NULL
";

            return Fill(sql, CreateParam("TGT_MNGGRP_CD", "B"));
        }

        //fir 대신 imageData 사용
        public DBScanInfo GetSScanInfo(string dbscan_id)
        {
            string sql = @"
SELECT SSCAN_ID DBSCAN_ID,
       FN_GET_IMAGE(IMAGE_ID) FIR,
       THRESHOLD
  FROM TB_SSCAN_REQ
 WHERE SSCAN_ID = :SSCAN_ID
";
            var dt = Fill(sql, CreateParam("SSCAN_ID", dbscan_id));
            return dt.Rows.Count == 1 ? new DBScanInfo(dt.Rows[0]) : null;
        }

        public void UpdateSafrId(string image_id, string safr_id)
        {
            string sql = @"
UPDATE TB_TGTMNG_OBJ_INFO
   SET ETC_REF_DSC_05 = :ETC_REF_DSC_05
 WHERE TGTMNG_OBJ_ID = :TGTMNG_OBJ_ID";

            ExecuteNonQuery(sql, CreateParam("ETC_REF_DSC_05", safr_id), CreateParam("TGTMNG_OBJ_ID", image_id));
        }

        public void InsertDBScanRslt(string dbscan_id, Match[] matches)
        {
            string sql = @"
INSERT INTO TB_SSCAN_RSLT (SSCAN_ID, TGTMNG_OBJ_ID, SCORE, RANK)
VALUES (:SSCAN_ID, :TGTMNG_OBJ_ID, :SCORE, :RANK)
";

            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                cmd.ArrayBindCount = matches.Length;

                string[] DBSCAN_ID = Enumerable.Repeat(dbscan_id, matches.Length).ToArray();
                string[] TGTMNG_OBJ_ID = matches.Select(x => x.id).ToArray();
                float[] SCORE = matches.Select(x => x.score).ToArray();
                int[] RANK = Enumerable.Range(1, matches.Length).ToArray();

                cmd.Parameters.Add("SSCAN_ID", OracleDbType.Varchar2, DBSCAN_ID, ParameterDirection.Input);
                cmd.Parameters.Add("TGTMNG_OBJ_ID", OracleDbType.Varchar2, TGTMNG_OBJ_ID, ParameterDirection.Input);
                cmd.Parameters.Add("SCORE", OracleDbType.Single, SCORE, ParameterDirection.Input);
                cmd.Parameters.Add("RANK", OracleDbType.Int32, RANK, ParameterDirection.Input);

                cmd.ExecuteNonQuery();
            }

            sql = @"
UPDATE TB_SSCAN_REQ
   SET MATCH_CNT = :MATCH_CNT
 WHERE SSCAN_ID = :SSCAN_ID
";

            ExecuteNonQuery(sql, CreateParam("MATCH_CNT", matches.Length),
                                 CreateParam("SSCAN_ID", dbscan_id));
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
       TB_SSCAN_RSLT B
 WHERE A.TGTMNG_OBJ_ID = B.TGTMNG_OBJ_ID
   AND B.SSCAN_ID = :SSCAN_ID
 ORDER BY B.RANK
";

            var dt = Fill(sql, CreateParam("SSCAN_ID", dbscan_id));
            return IdentifyResult.MatchedFace.FromTable(dt);
        }

        public DataRow GetVerifyReq(string verify_id)
        {
            string sql = @"
SELECT REQ_ID,
       FACE_ID1,
       FACE_ID2
  FROM TB_SVERIFY_REQ
 WHERE REQ_ID = :REQ_ID";

            DataTable dt = Fill(sql, new OracleParameter("REQ_ID", verify_id));

            return dt.Rows.Count == 1 ? dt.Rows[0] : null;
        }

        public void InsertVerifyRslt(string verify_id, float score)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_SVERIFY_RSLT (
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

        public List<string> GetPersonIdsMatchedSafrId(string safr_id)
        {
            string sql = @"
SELECT TGTMNG_OBJ_ID
  FROM TB_TGTMNG_OBJ_INFO
 WHERE ETC_REF_DSC_05 = :ETC_REF_DSC_05";
            DataTable dt = Fill(sql, new OracleParameter("ETC_REF_DSC_05", safr_id));

            List<string> list = new List<string>();

            foreach (DataRow row in dt.Rows)
            {
                list.Add(Convert.ToString(row["TGTMNG_OBJ_ID"]));
            }

            return list;
        }

        public void ResetSafrId()
        {
            string sql = @"
UPDATE TGTMNG_OBJ_ID
   SET ETC_REF_DSC_05 IS NULL
";
            ExecuteNonQuery(sql);
        }
    }
}
