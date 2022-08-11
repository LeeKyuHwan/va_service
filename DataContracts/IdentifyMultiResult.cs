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
    public class IdentifyMultiResult : DataContractBase<IdentifyMultiResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "result")]
        public List<IdentifyInfo> result = new List<IdentifyInfo>();

        public override string ToString()
        {
            return serialize(this);
        }

        public class IdentifyInfo : DataContractBase<IdentifyInfo>
        {
            [DataMember(Name = "algorithm")]
            public string algorithm { get; set; }

            [DataMember(Name = "matches")]
            public List<IdentifyResult.MatchedFace> matches = new List<IdentifyResult.MatchedFace>();
        }
    }
}
