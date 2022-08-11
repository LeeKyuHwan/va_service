using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public class CharacteristicsResult : DataContractBase<CharacteristicsResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "eyes_open")]
        public bool eyes_open { get; set; }

        [DataMember(Name = "exposure")]
        public bool exposure { get; set; }

        [DataMember(Name = "frontal_face")]
        public bool frontal_face { get; set; }

        [DataMember(Name = "frontal_eyes")]
        public bool frontal_eyes { get; set; }

        [DataMember(Name = "grayscale")]
        public bool grayscale { get; set; }

        [DataMember(Name = "uniform_lighting")]
        public bool uniform_lighting { get; set; }

        [DataMember(Name = "resolution")]
        public bool resolution { get; set; }

        [DataMember(Name = "sharpness")]
        public bool sharpness { get; set; }

        [DataMember(Name = "mouse_closed")]
        public bool mouse_closed { get; set; }

        [DataMember(Name = "glasses")]
        public bool glasses { get; set; }

        [DataMember(Name = "grade")]
        public string grade { get; set; }

        public override string ToString()
        {
            return serialize(this);
        }

        public static CharacteristicsResult Success()
        {
            return new CharacteristicsResult() { result_code = "S" };
        }

        public static CharacteristicsResult Fail(string msg)
        {
            return new CharacteristicsResult() { result_code = "F", result_msg = msg };
        }

    }
}
