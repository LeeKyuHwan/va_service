using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VAPIService
{
    public class Mode1
    {
        Dictionary<string, string> setting1 = new Dictionary<string, string>();
        Dictionary<string, float> setting2 = new Dictionary<string, float>();
        Dictionary<string, string> setting3 = new Dictionary<string, string>();
        Dictionary<string, float> setting4 = new Dictionary<string, float>();

        public Mode1(JToken jt)
        {
            foreach(var item in jt["setting1"])
            {
                var obj = item.ToObject<JProperty>();
                setting1.Add(obj.Name, Convert.ToString(obj.Value.ToString()));
            }

            foreach (var item in jt["setting2"])
            {
                var obj = item.ToObject<JProperty>();
                setting2.Add(obj.Name, Convert.ToSingle(obj.Value.ToString()));
            }
            foreach (var item in jt["setting3"])
            {
                var obj = item.ToObject<JProperty>();
                setting3.Add(obj.Name, Convert.ToString(obj.Value.ToString()));
            }
            foreach (var item in jt["setting4"])
            {
                var obj = item.ToObject<JProperty>();
                setting4.Add(obj.Name, Convert.ToSingle(obj.Value.ToString()));
            }
        }
    }

    public class Mode2
    {
        public Mode2(JToken jt)
        {

        }
    }

    public class Mode3
    {

        public Mode3(JToken jt)
        {

        }
    }
}
