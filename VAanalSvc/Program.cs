using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using VAnalSvc;
using WcfSvcBase;

namespace VAnalSvc
{
    static class Program
    {
        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            IService service = new WcfService<VAnalServiceBehavior>();
            Svc.Run(service, "VAnalService");
        }
    }
}
