using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class IdentifyResult : DataContractBase<IdentifyResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "matches")]
        public List<MatchedFace> matches = new List<MatchedFace>();

        public override string ToString()
        {
            return serialize(this);
        }

        public class MatchedFace
        {
            [DataMember(Name = "person_id")]
            public string person_id { get; set; }

            [DataMember(Name = "face_id")]
            public string face_id { get; set; }

            [DataMember(Name = "name")]
            public string name { get; set; }

            [DataMember(Name = "birth")]
            public string birth { get; set; }

            [DataMember(Name = "sex")]
            public string sex { get; set; }

            [DataMember(Name = "score")]
            public float score { get; set; }

            [DataMember(Name = "algorithm")]
            public string algorithm { get; set; }

            public static MatchedFace FromDataRow(DataRow row)
            {
                return new MatchedFace()
                {
                    person_id = row.Field<string>("TGTMNG_OBJ_ID"),
                    face_id = row.Field<string>("TGTMNG_OBJ_ID"),
                    name = row.Field<string>("EK_NM"),
                    birth = row.Field<string>("BIRTH_YMD"),
                    sex = row.Field<string>("SEX_CD"),
                    score = Convert.ToSingle(row["SCORE"])
                };
            }

            public static List<MatchedFace> FromTable(DataTable dt)
            {
                var list = dt.AsEnumerable().Select(x => FromDataRow(x)).ToList();
                return new List<MatchedFace>(list);
            }

            public MatchedFace Clone()
            {
                return new MatchedFace()
                {
                    person_id = this.person_id,
                    face_id = this.face_id,
                    name = this.name,
                    birth = this.birth,
                    sex = this.sex,
                    score = this.score,
                    algorithm = this.algorithm
                };
            }
        }

        public static IdentifyResult Success(List<MatchedFace> matches)
        {
            return new IdentifyResult() { result_code = "S", matches = matches };
        }

        public static IdentifyResult Fail(string msg)
        {
            return new IdentifyResult() { result_code = "F", result_msg = msg };
        }
    }
}
