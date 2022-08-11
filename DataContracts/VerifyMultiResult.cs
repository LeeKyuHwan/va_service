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
    public class VerifyMultiResult : DataContractBase<VerifyMultiResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "scores")]
        public List<ScoreInfo> scores = new List<ScoreInfo>();

        public override string ToString()
        {
            return serialize(this);
        }

        public class ScoreInfo : DataContractBase<ScoreInfo>
        {
            [DataMember(Name = "algorithm")]
            public string algorithm { get; set; }

            [DataMember(Name = "score")]
            public float score { get; set; }
        }
    }
}
