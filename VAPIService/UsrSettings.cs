using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VAPIService
{
    public class UsrSettings
    {
        public string usr_id { get; set; }
        public string settings_cd { get; set; }
        public string settings_str { get; set; }
        public JObject jo { get; set; }

        public UsrSettings(DataRow row)
        {
            usr_id = row["USR_ID"] as string;
            settings_cd = row["SETTINGS_CD"] as string;
            settings_str = row["SETTINGS_STR"] as string;
            jo = JObject.Parse(settings_str);
        }
    }
}
