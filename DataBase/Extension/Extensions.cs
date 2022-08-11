using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase.Extension
{
    public static class Extensions
    {
        public static T Default<T>(this DataRow row, string field, T defaultValue)
        {
            var value = row[field];
            if (value == null || value == DBNull.Value)
                return defaultValue;
            else
                return (T)row[field];
        }
    }
}
