using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WcfSvcBase
{
    public static class Svc
    {
        public static void Run(IService service, string name)
        {
            if (Environment.UserInteractive)
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new SvcForm(service) { Text = name });
            }
            else
            {
                ServiceBase[] ServicesToRun;
                ServicesToRun = new ServiceBase[]
                {
                    new Service(service)
                };
                ServiceBase.Run(ServicesToRun);
            }
        }
    }
}
