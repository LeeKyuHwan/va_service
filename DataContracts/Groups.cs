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
    public class Groups : DataContractBase<Groups>
    {
        [DataMember(Name = "groups")]
        public Collection<GroupInfo> groups = new Collection<GroupInfo>();

        public class GroupInfo
        {
            [DataMember(Name = "group_id")]
            public string group_id { get; set; }

            [DataMember(Name = "group_cd")]
            public string group_cd { get; set; }

            [DataMember(Name = "status_cd")]
            public string status_cd { get; set; }

            [DataMember(Name = "name")]
            public string name { get; set; }

            [DataMember(Name = "count")]
            public int count { get; set; }

        }

        public override string ToString()
        {
            return serialize(this);
        }
    }
}
