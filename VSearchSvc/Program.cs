﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using WcfSvcBase;

namespace VSearchSvc
{
    static class Program
    {
        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            var service = new WcfService<VSearchServiceBehavior>();
            service.OnStarted += Service_OnStarted;

            Svc.Run(service, "VSearchService");
        }

        private static void Service_OnStarted(object sender, EventArgs e)
        {
            FrHelper.Instance.LoadVideos();
        }
    }
}
