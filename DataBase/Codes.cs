using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase
{
    public static class Codes
    {
        public static class VANAL_STS_CD
        {
            /// <summary>분석 대기</summary>
            public const string R = "R";

            /// <summary>분석 중</summary>
            public const string P = "P";

            /// <summary>병합 중</summary>
            public const string M = "M";

            /// <summary>검색 중</summary>
            public const string I = "I";

            /// <summary>분석 완료</summary>
            public const string S = "S";
        }
    }
}
