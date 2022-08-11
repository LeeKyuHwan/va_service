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
    public class VanalRsltResult : DataContractBase<VanalRsltResult>
    {
        [DataMember(Name = "total_cnt")]
        public int total_cnt { get; set; }

        [DataMember(Name = "page")]
        public int page { get; set; }

        [DataMember(Name = "rslt")]
        public Collection<VanalRsltObj> rslt = new Collection<VanalRsltObj>();

        public class VanalRsltObj
        {
            [DataMember(Name = "crop_id")]
            public string crop_id { get; set; }

            [DataMember(Name = "image_url")]
            public string image_url { get; set; }

            //[DataMember(Name = "snapshot_url")]
            //public string snapshot_url { get; set; }

            [DataMember(Name = "detected_fr")]
            public double detected_fr { get; set; }

            [DataMember(Name = "detected_to")]
            public double detected_to { get; set; }

            [DataMember(Name = "bounds")]
            public string bounds { get; set; }

            [DataMember(Name = "score")]
            public float score { get; set; }

            [DataMember(Name = "match_cnt")]
            public int match_cnt { get; set; }
        }

        public override string ToString()
        {
            return serialize(this);
        }
    }
}
