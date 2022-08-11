using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    public class PersonListResult : DataContractBase<PersonListResult>
    {
        [DataMember(Name = "persons")]
        public Collection<PersonInfo> persons = new Collection<PersonInfo>();

        public override string ToString()
        {
            return serialize(this);
        }
    }
}
