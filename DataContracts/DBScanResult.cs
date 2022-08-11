using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class DBScanResult : DataContractBase<DBScanResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "matches")]
        public Collection<MatchResult> matches = new Collection<MatchResult>();

        public override string ToString()
        {
            return serialize(this);
        }

        public class MatchResult
        {
            [DataMember(Name = "person_id")]
            public string person_id { get; set; }

            [DataMember(Name = "score")]
            public float score { get; set; }

            [DataMember(Name = "rank")]
            public int rank { get; set; }
        }

        public static DBScanResult Success(Collection<MatchResult> matches)
        {
            return new DBScanResult() { result_code = "S", matches = matches };
        }

        public static DBScanResult Fail(string msg)
        {
            return new DBScanResult() { result_code = "F", result_msg = msg };
        }
    }
}
