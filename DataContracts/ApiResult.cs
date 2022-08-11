using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class ApiResult : DataContractBase<ApiResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        public override string ToString()
        {
            return serialize(this);
        }

        public static ApiResult Success = new ApiResult() { result_code = "S", result_msg = string.Empty };

        public static ApiResult Fail(string msg)
        {
            return new ApiResult() { result_code = "F", result_msg = msg };
        }
    }
}
