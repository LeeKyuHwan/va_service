using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace WcfSvcBase
{
    public partial class Service : ServiceBase
    {
        IService service;

        public Service(IService service)
        {
            InitializeComponent();

            this.service = service;
        }

        protected override void OnStart(string[] args)
        {
            service.Start();
        }

        protected override void OnStop()
        {
            service.Stop();
        }
    }
}
