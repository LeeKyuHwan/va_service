using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using WcfSvcBase;

namespace FaceMeService
{
    static class Program
    {
        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            var service = new WcfService<FaceMeServiceBehavior>();
            service.OnStarted += Service_OnStarted;

            Svc.Run(service, "FaceMeService");
        }

        private static void Service_OnStarted(object sender, EventArgs e)
        {
            var num = FrHelper.Instance.InitializeEngine();
            //FrHelper.Instance.test();
            if (num == 0) FrHelper.Instance.LoadGallery("B");
        }
    }
}
