using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.ServiceModel.Web;
using System.Text;
using System.Threading.Tasks;

namespace WcfSvcBase
{
    public static class Utils
    {
        public static Dictionary<string, dynamic> ParseFormData(Stream stream)
        {
            var data = new Dictionary<string, dynamic>();

            IncomingWebRequestContext request = WebOperationContext.Current.IncomingRequest;
            WebHeaderCollection headers = request.Headers;
            string contentType = headers.Get("Content-Type");

            using (StreamContent sc = new StreamContent(stream))
            {
                sc.Headers.ContentType = MediaTypeHeaderValue.Parse(contentType);
                var provider = sc.ReadAsMultipartAsync().Result;

                foreach (var httpContent in provider.Contents)
                {
                    string name = httpContent.Headers.ContentDisposition.Name;
                    if (name[0] == '"')
                        name = name.Remove(0, 1);
                    if (name[name.Length - 1] == '"')
                        name = name.Remove(name.Length - 1, 1);

                    object value = null;

                    string filename = httpContent.Headers.ContentDisposition.FileName;
                    if (string.IsNullOrWhiteSpace(filename))
                    {
                        value = httpContent.ReadAsStringAsync().Result;
                    }
                    else
                    {
                        byte[] fileData;
                        using (Stream fs = httpContent.ReadAsStreamAsync().Result)
                        {
                            fileData = new byte[fs.Length];
                            fs.Read(fileData, 0, fileData.Length);
                        }

                        value = fileData;
                    }

                    data.Add(name, value);
                }

                return data;
            }
        }

        public static string PrintDic(Dictionary<string, dynamic> dic)
        {
            var keys = dic.Keys;

            StringBuilder sb = new StringBuilder();
            foreach (string key in keys)
            {
                var value = dic[key];
                if (value is byte[] data)
                    sb.Append($"{key}:[{data.Length}bytes] ");
                else
                    sb.Append($"{key}:[{dic[key]}] ");
            }

            return sb.ToString();
        }
        public static string PrintDic(Dictionary<string, string> dic)
        {
            var keys = dic.Keys;

            StringBuilder sb = new StringBuilder();
            foreach (string key in keys)
            {
                var value = dic[key];
                sb.Append($"{key}:[{dic[key]}] ");
            }

            return sb.ToString();
        }
    }
}
