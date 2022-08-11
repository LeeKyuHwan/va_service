using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class VsearchResult : DataContractBase<VsearchResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "vsearch_id")]
        public string vsearch_id { get; set; }

        [DataMember(Name = "rslt")]
        public Collection<VsearchResultObj> rslt = new Collection<VsearchResultObj>();

        public class VsearchResultObj
        {
            [DataMember(Name = "video_id")]
            public string video_id { get; set; }

            [DataMember(Name = "title")]
            public string title { get; set; }

            [DataMember(Name = "url")]
            public string url { get; set; }

            [DataMember(Name = "reg_dt")]
            public string reg_dt { get; set; }

            [DataMember(Name = "detect_cnt")]
            public int detect_cnt { get; set; }

            public VsearchResultObj()
            {
            }

            public VsearchResultObj(DataRow row)
            {
            }
        }

        public override string ToString()
        {
            return serialize(this);
        }

        public static VsearchResult Fail(string msg)
        {
            return new VsearchResult() { result_code = "F", result_msg = msg };
        }
    }
}
