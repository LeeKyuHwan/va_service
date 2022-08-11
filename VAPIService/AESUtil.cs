using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Web;

namespace VAPIService
{
    class AESUtil
    {
        static readonly string key = "sqisoft74307".PadRight(32, '0');
        static readonly string key128 = key.Substring(0, 16);
        static readonly string key256 = key.Substring(0, 32);

        RijndaelManaged rijndaelCipher;
        ICryptoTransform encryptor;
        ICryptoTransform decryptor;

        public static AESUtil Instance = new AESUtil();

        public AESUtil()
        {
            RijndaelManaged rijndaelCipher = new RijndaelManaged();
            rijndaelCipher.Mode = CipherMode.CBC;
            rijndaelCipher.Padding = PaddingMode.PKCS7;
            rijndaelCipher.KeySize = 256;
            rijndaelCipher.BlockSize = 128;
            rijndaelCipher.Key = Encoding.UTF8.GetBytes(key256);
            rijndaelCipher.IV = Encoding.UTF8.GetBytes(key128);

            this.rijndaelCipher = rijndaelCipher;

            encryptor = rijndaelCipher.CreateEncryptor(rijndaelCipher.Key, rijndaelCipher.IV);
            decryptor = rijndaelCipher.CreateDecryptor();
        }


        public string Encrypt256(string textToEncrypt)//, string key)
        {
            try
            {
                byte[] xBuff = null;

                using (var ms = new MemoryStream())
                {
                    using (var cs = new CryptoStream(ms, encryptor, CryptoStreamMode.Write))
                    {
                        byte[] xXml = Encoding.UTF8.GetBytes(textToEncrypt);

                        cs.Write(xXml, 0, xXml.Length);
                    }
                    xBuff = ms.ToArray();
                }
                //string encryptStr = Convert.ToBase64String(xBuff).Replace('+', ' ');
                //encryptStr = WebUtility.UrlEncode(encryptStr);
                string encryptStr = BytesToHex(xBuff, false);

                return encryptStr;
            }
            catch (Exception e)
            {
                return null;
            }
        }

        public string Decrypt256(string textToDecrypt) //, string key)
        {
            try
            {
                //textToDecrypt = WebUtility.UrlDecode(textToDecrypt);
                //textToDecrypt = textToDecrypt.Replace(' ', '+');
                byte[] textBytes = HexStringToByteArray(textToDecrypt);

                byte[] xBuff = null;
                using (var ms = new MemoryStream())
                {
                    using (var cs = new CryptoStream(ms, decryptor, CryptoStreamMode.Write))
                    {
                        byte[] xXml = textBytes; // Convert.FromBase64String(textToDecrypt);
                        cs.Write(xXml, 0, xXml.Length);
                    }

                    xBuff = ms.ToArray();
                }

                string decryptStr = Encoding.UTF8.GetString(xBuff);

                return decryptStr;
            }
            catch (Exception e)
            {
                return null;
            }
        }

        private static string BytesToHex(byte[] barray, bool toLowerCase = true)
        {
            byte addByte = 0x37;
            if (toLowerCase) addByte = 0x57;
            char[] c = new char[barray.Length * 2];
            byte b;
            for (int i = 0; i < barray.Length; ++i)
            {
                b = ((byte)(barray[i] >> 4));
                c[i * 2] = (char)(b > 9 ? b + addByte : b + 0x30);
                b = ((byte)(barray[i] & 0xF));
                c[i * 2 + 1] = (char)(b > 9 ? b + addByte : b + 0x30);
            }

            return new string(c);
        }

        private static byte[] HexStringToByteArray(string hexString)
        {
            //if (hexString.Length % 2 == 1)
            //    throw new Exception("The binary key cannot have an odd number of digits");
            //
            //byte[] arr = new byte[hexString.Length >> 1];
            //
            //for (int i = 0; i < hexString.Length >> 1; ++i)
            //{
            //    arr[i] = (byte)((GetHexVal(hexString[i << 1]) << 4) + (GetHexVal(hexString[(i << 1) + 1])));
            //}
            //
            //return arr;

            int hexStringLength = hexString.Length;
            byte[] b = new byte[hexStringLength / 2];
            for (int i = 0; i < hexStringLength; i += 2)
            {
                int topChar = (hexString[i] > 0x40 ? hexString[i] - 0x37 : hexString[i] - 0x30) << 4;
                int bottomChar = hexString[i + 1] > 0x40 ? hexString[i + 1] - 0x37 : hexString[i + 1] - 0x30;
                b[i / 2] = Convert.ToByte(topChar + bottomChar);
            }
            return b;
        }

        public static int GetHexVal(char hex)
        {
            int val = (int)hex;
            //For uppercase A-F letters:
            //return val - (val < 58 ? 48 : 55);
            //For lowercase a-f letters:
            //return val - (val < 58 ? 48 : 87);
            //Or the two combined, but a bit slower:
            return val - (val < 58 ? 48 : (val < 97 ? 55 : 87));
        }
    }
}
