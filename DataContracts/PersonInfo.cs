using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class PersonInfo : DataContractBase<PersonInfo>
    {
        [DataMember(Name = "person_id")]
        public string person_id { get; set; }

        [DataMember(Name = "face_id")]
        public string face_id { get; set; }

        [DataMember(Name = "name")]
        public string name { get; set; }

        [DataMember(Name = "sex")]
        public string sex { get; set; }

        [DataMember(Name = "reg_emp_no")]
        public string reg_emp_no { get; set; }

        public override string ToString()
        {
            return serialize(this);
        }
    }
}
