using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void btnWrite_Click(object sender, EventArgs e)
        {
            XmlUtils.InitUtil();
            IntPtr root = XmlUtils.WriteXmlRoot(Encoding.UTF8.GetBytes("ArrayOfPrimParBase"));
            IntPtr attr1 = XmlUtils.AllocAttr(Encoding.UTF8.GetBytes("xmlns:xsd"), Encoding.UTF8.GetBytes("http://www.w3.org/2001/XMLSchema"));
            IntPtr attr2 = XmlUtils.AllocAttr(Encoding.UTF8.GetBytes("xmlns:xsi"), Encoding.UTF8.GetBytes("http://www.w3.org/2001/XMLSchema-instance"));
            XmlUtils.AppendAttr(root, attr1);
            XmlUtils.AppendAttr(root, attr2);

            IntPtr node = XmlUtils.AllocNode(Encoding.UTF8.GetBytes("PrimParBase"), null);
            XmlUtils.AppendNode(node, XmlUtils.AllocNode(Encoding.UTF8.GetBytes("info"), Encoding.UTF8.GetBytes("good节点内容")));
            XmlUtils.AppendNode(root, node);
            XmlUtils.WriteXml(Encoding.Default.GetBytes("d:\\看到.task"));
            XmlUtils.ReleaseUtil();

        }
    }
}
