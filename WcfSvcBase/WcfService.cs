using log4net;
using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace WcfSvcBase
{
    public class WcfService<T> : IService
    {
        static ILog logger = LogManager.GetLogger("WcfService");

        ServiceHost host;

        public event EventHandler OnStarted;
        public event EventHandler OnStopped;

        public WcfService()
        {
            host = new ServiceHost(typeof(T));
        }

        public void Start()
        {
            host.Open();
            logger.InfoFormat("[Start] start hosting {0}", host.BaseAddresses[0]);

            OnStarted?.Invoke(this, EventArgs.Empty);
        }

        public void Stop()
        {
            host.Close();
            logger.InfoFormat("[Stop] hosting closed");

            OnStopped?.Invoke(this, EventArgs.Empty);
        }
    }
}
