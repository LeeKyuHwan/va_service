using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class PersonDeleteResult : DataContractBase<PersonDeleteResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "usr_id")]
        public string usr_id { get; set; }

        [DataMember(Name = "person_id")]
        public string person_id { get; set; }

        public override string ToString()
        {
            return serialize(this);
        }
    }
}
