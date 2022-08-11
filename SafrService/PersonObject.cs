using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SafrService
{
    class PersonObject
    {
        public string ID;
        public long LocalId;
        public float Temperature;
        public float Score;
        public bool Mask;
        public double Occlusion;

        public string Name;
        public string Sex;
        public string Birth;
        public string Grade;
        public string Room;
        public string Etc;

        public static string[] ParseInfo(string info)
        {
            byte[] data = Convert.FromBase64String(info);
            int offset = 0;

            List<string> infos = new List<string>();
            for (int i = 0; i < data.Length; i++)
            {
                if (data[i] == '\0')
                {
                    infos.Add(Encoding.UTF8.GetString(data, offset, i - offset));
                    offset = i + 1;
                }
            }

            return infos.ToArray();
        }

        public static string Encode(string name, string sex = "", string birth = "", string grade = "", string room = "", string etc = "")
        {
            if (!string.IsNullOrWhiteSpace(birth)) birth = DateTime.ParseExact(birth, "yyyyMMdd", null).ToString("yyyy-MM-dd");
            string infoStr = $"{name}\0{sex}\0{birth}\0{grade}\0{room}\0{etc}\0";
            return Convert.ToBase64String(Encoding.UTF8.GetBytes(infoStr));
        }


        public override string ToString()
        {
            return $"{ID} {Name} {Temperature} {Mask}";
        }

        //Name = infos.Length > 0 ? infos[0] : string.Empty;
        //Sex = infos.Length > 1 ? infos[1] : string.Empty;
        //Birth = infos.Length > 2 ? infos[2] : string.Empty;
        //Grade = infos.Length > 3 ? infos[3] : string.Empty;
        //Room = infos.Length > 4 ? infos[4] : string.Empty;
        //Etc = infos.Length > 5 ? infos[5] : string.Empty;
        //
        //if (string.IsNullOrEmpty(person.Person.ExternalId))
        //    Name = "Unknown";
    }
}
