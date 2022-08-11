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
    public class VsearchDtlResult : DataContractBase<VsearchDtlResult>
    {
        [DataMember(Name = "matches")]
        public Collection<VsearchDtlResultObj> matches = new Collection<VsearchDtlResultObj>();

        public class VsearchDtlResultObj
        {
            [DataMember(Name = "crop_id")]
            public string crop_id { get; set; }

            [DataMember(Name = "image_url")]
            public string image_url { get; set; }

            [DataMember(Name = "detected_fr")]
            public double detected_fr { get; set; }

            [DataMember(Name = "detected_to")]
            public double detected_to { get; set; }

            [DataMember(Name = "score")]
            public float score { get; set; }

            [DataMember(Name = "bounds")]
            public string bounds { get; set; }
        }

        public override string ToString()
        {
            return serialize(this);
        }
    }
}
