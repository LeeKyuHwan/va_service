using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class VerifyResult : DataContractBase<VerifyResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "score")]
        public float score { get; set; }

        public override string ToString()
        {
            return serialize(this);
        }

        public static VerifyResult Success(float score)
        {
            return new VerifyResult() { result_code = "S", score = score };
        }

        public static VerifyResult Fail(string msg)
        {
            return new VerifyResult() { result_code = "F", result_msg = msg };
        }
    }
}
