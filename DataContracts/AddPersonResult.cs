using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class AddPersonResult : DataContractBase<AddPersonResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "person_id")]
        public string person_id { get; set; }

        public override string ToString()
        {
            return serialize(this);
        }

        public static AddPersonResult Success(string person_id)
        {
            return new AddPersonResult() { result_code = "S", result_msg = string.Empty, person_id = person_id };
        }

        public static AddPersonResult Fail(string msg)
        {
            return new AddPersonResult() { result_code = "F", result_msg = msg };
        }
    }
}
