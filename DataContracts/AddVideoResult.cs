using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    public class AddVideoResult : DataContractBase<AddVideoResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "video_id")]
        public string video_id { get; set; }

        public override string ToString()
        {
            return serialize(this);
        }

        public static AddVideoResult Success()
        {
            return new AddVideoResult() { result_code = "S" };
        }

        public static AddVideoResult Fail(string msg)
        {
            return new AddVideoResult() { result_code = "F", result_msg = msg };
        }
    }
}
