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
    public class DetectResult : DataContractBase<DetectResult>
    {
        [DataMember(Name = "result_code")]
        public string result_code { get; set; }

        [DataMember(Name = "result_msg")]
        public string result_msg { get; set; }

        [DataMember(Name = "faces")]
        public Collection<DetectedFace> faces = new Collection<DetectedFace>();

        public override string ToString()
        {
            return serialize(this);
        }


        public class DetectedFace
        {
            [DataMember(Name = "box_code")]
            public string box_coord { get; set; }

            [DataMember(Name = "face_id")]
            public string face_id { get; set; }

            [DataMember(Name = "sex")]
            public string sex { get; set; }

            [DataMember(Name = "age")]
            public int age { get; set; }
        }

        public static DetectResult Success(Collection<DetectedFace> faces)
        {
            return new DetectResult() { result_code = "S", faces = faces };
        }

        public static DetectResult Fail(string msg)
        {
            return new DetectResult() { result_code = "F", result_msg = msg };
        }
    }
}
