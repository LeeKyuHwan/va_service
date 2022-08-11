using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class PersonEnrollResult : DataContractBase<PersonEnrollResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "usr_id")]
        public string usr_id { get; set; }

        [DataMember(Name = "target_id")]
        public string target_id { get; set; }

        [DataMember(Name = "face_id")]
        public string face_id { get; set; }

        [DataMember(Name = "en_name")]
        public string en_name { get; set; }

        [DataMember(Name = "en_sex")]
        public string en_sex { get; set; }

        [DataMember(Name = "en_bir_day")]
        public string en_bir_day { get; set; }

        [DataMember(Name = "en_etc")]
        public string en_etc { get; set; }

        [DataMember(Name = "en_grp_id")]
        public string en_grp_id { get; set; }

        public override string ToString()
        {
            return serialize(this);
        }

        public static PersonEnrollResult Success()
        {
            return new PersonEnrollResult() { result_code = "S" };
        }

        public static PersonEnrollResult Fail(string msg)
        {
            return new PersonEnrollResult() { result_code = "F", result_msg = msg };
        }
    }
}
