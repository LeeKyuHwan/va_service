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
    public class SearchVideoResult : DataContractBase<SearchVideoResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "vsearch_id")]
        public string vsearch_id { get; set; }

        public override string ToString()
        {
            return serialize(this);
        }

        public static SearchVideoResult Success(string vsearch_id)
        {
            return new SearchVideoResult() { result_code = "S", vsearch_id = vsearch_id };
        }

        public static SearchVideoResult Fail(string msg)
        {
            return new SearchVideoResult() { result_code = "F", result_msg = msg };
        }
    }
}
