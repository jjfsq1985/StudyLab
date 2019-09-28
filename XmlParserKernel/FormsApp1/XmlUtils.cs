using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FormsApp1
{
    public class XmlUtils
    {
        [DllImport("XmlParserKernel.dll")]
        public static extern bool InitUtil();

        [DllImport("XmlParserKernel.dll")]
        public static extern IntPtr XmlRoot(byte[] cRoot);


        [DllImport("XmlParserKernel.dll")]
        public static extern bool ReleaseUtil();

        [DllImport("XmlParserKernel.dll")]
        public static extern bool WriteXml(byte[] cPath);

        [DllImport("XmlParserKernel.dll")]
        public static extern IntPtr AllocNode(byte[] cName, byte[] cValue);

        [DllImport("XmlParserKernel.dll")]
        public static extern void AppendNode(IntPtr parent, IntPtr node);

        [DllImport("XmlParserKernel.dll")]
        public static extern IntPtr AllocAttr(byte[] cAttrName, byte[] cValue);

        [DllImport("XmlParserKernel.dll")]
        public static extern void AppendAttr(IntPtr node, IntPtr attr);
    }
}
