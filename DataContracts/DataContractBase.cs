using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading.Tasks;

namespace DataContracts
{
    [DataContract]
    public abstract class DataContractBase<T>
    {
        protected static DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(T));

        protected string serialize(object data)
        {
            using (MemoryStream ms = new MemoryStream())
            {
                serializer.WriteObject(ms, data);
                return Encoding.UTF8.GetString(ms.ToArray());
            }
        }
    }
}
