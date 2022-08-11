using log4net;
using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace WcfSvcBase
{
    public class WcfServices : IService
    {
        static ILog logger = LogManager.GetLogger(typeof(WcfServices));

        ServiceHost[] hosts;

        public event EventHandler OnStarted;
        public event EventHandler OnStopped;

        public WcfServices(Type[] contracts)
        {
            hosts = new ServiceHost[contracts.Length];

            for (int i = 0; i < contracts.Length; i++)
                hosts[i] = new ServiceHost(contracts[i]);
        }

        public void Start()
        {
            for (int i = 0; i < hosts.Length; i++)
            {
                hosts[i].Open();
                logger.InfoFormat($"[Start] start hosting {hosts[i].BaseAddresses[0]}");
            }

            OnStarted?.Invoke(this, EventArgs.Empty);
        }

        public void Stop()
        {
            for (int i = 0; i < hosts.Length; i++)
            {
                hosts[i].Close();
                logger.InfoFormat($"[Stop] {hosts[i].BaseAddresses[0]} closed");
            }

            OnStopped?.Invoke(this, EventArgs.Empty);
        }
    }
}
