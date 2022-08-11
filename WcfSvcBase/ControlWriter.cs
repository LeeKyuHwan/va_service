using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WcfSvcBase
{
    public class ControlWriter : TextWriter
    {
        private TextBox textbox;
        public ControlWriter(TextBox textbox)
        {
            this.textbox = textbox;
        }

        public override void Write(string value)
        {
            if (textbox.InvokeRequired)
                textbox.Invoke((Action)delegate { textbox.AppendText(value); });
            else
                textbox.AppendText(value);
        }

        public override Encoding Encoding
        {
            get { return Encoding.UTF8; }
        }
    }
}
