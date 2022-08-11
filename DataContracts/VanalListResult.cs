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
    public class VanalListResult : DataContractBase<VanalListResult>
    {
        [DataMember(Name = "total_cnt")]
        public int total_cnt { get; set; }

        [DataMember(Name = "page")]
        public int page { get; set; }

        [DataMember(Name = "videos")]
        public Collection<VanalListObj> videos = new Collection<VanalListObj>();

        public class VanalListObj
        {
            [DataMember(Name = "video_id")]
            public string video_id { get; set; }

            [DataMember(Name = "url")]
            public string url { get; set; }

            [DataMember(Name = "title")]
            public string title { get; set; }

            [DataMember(Name = "play_time")]
            public int play_time { get; set; }

            [DataMember(Name = "status_cd")]
            public string status_cd { get; set; }

            [DataMember(Name = "status_txt")]
            public string status_txt { get; set; }

            [DataMember(Name = "detect_cnt")]
            public int detect_cnt { get; set; }

            [DataMember(Name = "match_cnt")]
            public int match_cnt { get; set; }

            [DataMember(Name = "reg_dt")]
            public string reg_dt { get; set; }

            [DataMember(Name = "reg_user")]
            public string reg_user { get; set; }

            public void setting_status_txt(string status_cd, int job_prcnt = 0)
            {
                switch (status_cd)
                {
                    case "R":
                        status_txt = ResultCode.Wait;
                        break;
                    case "P":
                        status_txt = $"{ResultCode.Do}({job_prcnt}%)";
                        break;
                    case "S":
                        status_txt = ResultCode.Complete;
                        break;
                    case "M":
                        status_txt = ResultCode.Merge;
                        break;
                    case "I":
                        status_txt = ResultCode.Identy;
                        break;
                    case "F":
                        status_txt = ResultCode.Fail;
                        break;
                    default:
                        status_txt = ResultCode.Wait;
                        break;
                }
            }
        }

        public override string ToString()
        {
            return serialize(this);
        }
    }

    public static class ResultCode
    {
        public const string Wait = "분석 대기";
        public const string Do = "분석 중";
        public const string Complete = "분석 완료";
        public const string Merge = "병합 중";
        public const string Identy = "검색 중";
        public const string Fail = "분석 실패";
    }
}
