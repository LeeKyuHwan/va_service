using DataBase;
using FrService.Properties;
using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FrService
{
    class DBClient : DBClientBase
    {
        static readonly string connStr = CreateConnectionStr("frs", "frs123!", "aiface.cx2zge1o00bx.ap-northeast-2.rds.amazonaws.com", 1521, "orcl");

        public DBClient() : base(connStr)
        {
        }

        public DataRow GetDetectReq(string detect_id)
        {
            string sql = @"
SELECT REQ_ID,
       REQ_IMAGE,
       MIN_EYE_DIST
  FROM TB_DETECT_REQ
 WHERE REQ_ID = :REQ_ID";

            DataTable dt = Fill(sql, new OracleParameter("REQ_ID", detect_id));

            return dt.Rows.Count == 1 ? dt.Rows[0] : null;
        }

        public string GetDetectRsltID()
        {
            string sql = @"
SELECT FN_GET_DETECT_RSLT_ID
  FROM DUAL";

            return Convert.ToString(ExecuteScalar(sql));
        }

        public void InsertDetectRslt(string req_id, string rslt_id, byte[] cropImg, string face_coord, string sex, int age) //byte[] fir, string face_coord, string sex, int age)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = $@"
INSERT INTO TB_DETECT_RSLT (
       REQ_ID,
       FACE_ID,
       CROP_IMG,
       FACE_COORD,
       SEX,
       AGE
       )
VALUES (
       :REQ_ID,
       :FACE_ID,
       :CROP_IMG,
       :FACE_COORD,
       :SEX,
       :AGE
       )";

            parameters.Add(new OracleParameter("REQ_ID", req_id));
            parameters.Add(new OracleParameter("FACE_ID", rslt_id));
            parameters.Add(new OracleParameter("CROP_IMG", cropImg));
            parameters.Add(new OracleParameter("FACE_COORD", face_coord));
            parameters.Add(new OracleParameter("SEX", sex));
            parameters.Add(new OracleParameter("AGE", age));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public void UpdateDetectReq(string req_id, string req_rslt, string fail_rsn = null)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
UPDATE TB_DETECT_REQ 
   SET REQ_RSLT = :REQ_RSLT,
       REQ_FAIL_RSN = :REQ_FAIL_RSN
 WHERE REQ_ID = :REQ_ID";

            parameters.Add(new OracleParameter("REQ_ID", req_id));
            parameters.Add(new OracleParameter("REQ_RSLT", req_rslt));
            parameters.Add(new OracleParameter("REQ_FAIL_RSN", fail_rsn));

            ExecuteNonQuery(sql, parameters.ToArray());
        }

        public DataRow GetVerifyReq(string verify_id)
        {
            string sql = @"
SELECT REQ_ID,
       FACE_ID1,
       FACE_ID2
  FROM TB_VERIFY_REQ
 WHERE REQ_ID = :REQ_ID";

            DataTable dt = Fill(sql, new OracleParameter("REQ_ID", verify_id));

            return dt.Rows.Count == 1 ? dt.Rows[0] : null;
        }

        public DataRow GetUnitInfo(string image_id)
        {
            string sql = @"
SELECT :ID ID,
       FN_GET_IMAGE(:ID) IMAGE,
       FN_GET_FIR(:ID) FIR 
  FROM DUAL";
            DataTable dt = Fill(sql, new OracleParameter("ID", image_id));

            return dt.Rows.Count == 1 ? dt.Rows[0] : null;
        }

        //UpdateFir로 사용중
//        public void UpdateUnitFir(string face_id, byte[] firData)
//        {
//            List<OracleParameter> parameters = new List<OracleParameter>();

//            string sql = @"
//UPDATE TB_DETECT_RSLT 
//   SET FIR = :FIR 
// WHERE FACE_ID = :FACE_ID";

//            parameters.Add(new OracleParameter("FACE_ID", face_id));
//            parameters.Add(new OracleParameter("FIR", firData));

//            ExecuteNonQuery(sql, parameters.ToArray());
//        }

        public void InsertVerifyRslt(string verify_id, float score)
        {
            List<OracleParameter> parameters = new List<OracleParameter>();

            string sql = @"
INSERT INTO TB_VERIFY_RSLT (
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

        //이미지 데이터
        public byte[] GetImgData(string image_id)
        {
            string sql = @"
SELECT FN_GET_IMAGE(:ID)
  FROM DUAL";

            return ExecuteScalar(sql, new OracleParameter("ID", image_id)) as byte[];
        }

        public OracleParameter[] UpdateFir(string image_id, byte[] fir)
        {
            OracleParameter[] iParams = new OracleParameter[] { CreateParam("I_IMAGE_ID", image_id), CreateParam("I_FIR", fir) };
            OracleParameter[] oParams = new OracleParameter[] { CreateOParam("O_RSLT_MSG_CD", 16), CreateOParam("O_RSLT_MSG", 1024) };
            Procecure("SP_UPDT_FIR", iParams, oParams);

            return oParams;
        }
    }
}
