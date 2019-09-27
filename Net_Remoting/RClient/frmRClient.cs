using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;

namespace RemotableObjects
{


    public class frmRCleint : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox textBox1;

        private MyRemotableObject remoteObject;
        private EventWrapper wrapper;

        private SplitContainer splitContainer1;
        private TextBox textBox2;
		
		private System.ComponentModel.Container components = null;

		public frmRCleint()
		{

			InitializeComponent();

			//************************************* TCP *************************************//
			// using TCP protocol
			// running both client and server on same machines
			//TcpChannel chan = new TcpChannel();
            //双向信道
            BinaryServerFormatterSinkProvider serverProv = new BinaryServerFormatterSinkProvider();
            serverProv.TypeFilterLevel = System.Runtime.Serialization.Formatters.TypeFilterLevel.Full;
            BinaryClientFormatterSinkProvider clientProv = new BinaryClientFormatterSinkProvider();
            IDictionary props = new Hashtable();
            props["port"] = 0;//接收端口(随机)
            TcpChannel chan = new TcpChannel(props, clientProv, serverProv);

			ChannelServices.RegisterChannel(chan,false);
			// Create an instance of the remote object
			remoteObject = (MyRemotableObject) Activator.GetObject(typeof(MyRemotableObject),"tcp://localhost:8080/HelloWorld");
			// if remote object is on another machine the name of the machine should be used instead of localhost.
			//************************************* TCP *************************************//
            wrapper = new EventWrapper();//事件包装，必须
            wrapper.LocalEvent += new ServerEventHandler(OnServerEvent);
            remoteObject.ServerEvent += new ServerEventHandler(wrapper.Response);
        }

		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Location = new System.Drawing.Point(0, 0);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(669, 160);
            this.textBox1.TabIndex = 0;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.textBox1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.textBox2);
            this.splitContainer1.Size = new System.Drawing.Size(669, 334);
            this.splitContainer1.SplitterDistance = 160;
            this.splitContainer1.TabIndex = 1;
            // 
            // textBox2
            // 
            this.textBox2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox2.Location = new System.Drawing.Point(0, 0);
            this.textBox2.Multiline = true;
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(669, 170);
            this.textBox2.TabIndex = 0;
            // 
            // frmRCleint
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
            this.ClientSize = new System.Drawing.Size(669, 334);
            this.Controls.Add(this.splitContainer1);
            this.Name = "frmRCleint";
            this.Text = "RemoteClient";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

		}
		#endregion


		[STAThread]
		static void Main() 
		{
			Application.Run(new frmRCleint());
		}

		private void textBox1_TextChanged(object sender, System.EventArgs e)
		{
            remoteObject.SetMsg(textBox1.Text);
		}

        public void OnServerEvent(string strCmd)
        {
            GetCommand(strCmd);
        }

        public delegate void setText(string strText);

        void GetCommand(string strCmd)
        {
            //thread safe
            if (textBox2.InvokeRequired)
            {
                textBox2.Invoke(new setText(GetCommand), new object[] { strCmd });
            }
            else
            {
                textBox2.Text = strCmd;
            }
        }
        
	}
}
