using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace WcfSvcBase
{
    // 참고: "리팩터링" 메뉴에서 "이름 바꾸기" 명령을 사용하여 코드 및 config 파일에서 클래스 이름 "Service1"을 변경할 수 있습니다.
    public class Service1 : IService1
    {
        public void DoWork()
        {
        }
    }
}
