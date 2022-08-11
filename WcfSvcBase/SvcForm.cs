using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WcfSvcBase
{
    public partial class SvcForm : Form
    {
        IService service;

        public SvcForm(IService service)
        {
            InitializeComponent();

            Console.SetOut(new ControlWriter(tbLog));
            this.service = service;
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            service.Start();
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            service.Stop();
        }
    }
}
