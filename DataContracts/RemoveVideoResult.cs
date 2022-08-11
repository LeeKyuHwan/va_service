using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class RemoveVideoResult : DataContractBase<RemoveVideoResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        public override string ToString()
        {
            return serialize(this);
        }

        public static RemoveVideoResult Success = new RemoveVideoResult() { result_code = "S", result_msg = string.Empty };

        public static RemoveVideoResult Fail(string msg)
        {
            return new RemoveVideoResult() { result_code = "F", result_msg = msg };
        }
    }
}
