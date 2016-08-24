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

	public class frmRServer : System.Windows.Forms.Form, IObserver
	{
		private System.Windows.Forms.TextBox textBox1;
		private MyRemotableObject remotableObject;
        private TextBox textBox2;
        private SplitContainer splitContainer1;
	
		private System.ComponentModel.Container components = null;

		public frmRServer()
		{
			
			InitializeComponent();
			remotableObject = new MyRemotableObject();
			
			//************************************* TCP *************************************//
			// using TCP protocol
			//TcpChannel channel = new TcpChannel(8080);
            //双向信道
            BinaryServerFormatterSinkProvider serverProv = new BinaryServerFormatterSinkProvider();
            serverProv.TypeFilterLevel = System.Runtime.Serialization.Formatters.TypeFilterLevel.Full;
            BinaryClientFormatterSinkProvider clientProv = new BinaryClientFormatterSinkProvider();
            IDictionary props = new Hashtable();
            props["port"] = 8080;
            TcpChannel channel = new TcpChannel(props, clientProv, serverProv);

			ChannelServices.RegisterChannel(channel,false);
			//RemotingConfiguration.RegisterWellKnownServiceType(typeof(MyRemotableObject),"HelloWorld",WellKnownObjectMode.Singleton);
            ObjRef objRef = RemotingServices.Marshal(remotableObject, "HelloWorld");//server 向 client 广播消息
			//************************************* TCP *************************************//
			RemotableObjects.Cache.Attach(this);
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
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
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
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(656, 191);
            this.textBox1.TabIndex = 1;
            // 
            // textBox2
            // 
            this.textBox2.BackColor = System.Drawing.SystemColors.Window;
            this.textBox2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox2.Location = new System.Drawing.Point(0, 0);
            this.textBox2.Multiline = true;
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(656, 192);
            this.textBox2.TabIndex = 2;
            this.textBox2.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
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
            this.splitContainer1.Size = new System.Drawing.Size(656, 387);
            this.splitContainer1.SplitterDistance = 191;
            this.splitContainer1.TabIndex = 0;
            // 
            // frmRServer
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
            this.ClientSize = new System.Drawing.Size(656, 387);
            this.Controls.Add(this.splitContainer1);
            this.Name = "frmRServer";
            this.Text = "RemoteServer";
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
			Application.Run(new frmRServer());
		}

		#region IObserver Members

        public delegate void setText(string strText);

		public void Notify(string text)
		{
            //thread safe
            if (textBox1.InvokeRequired)
            {
                textBox1.Invoke(new setText(Notify), new object[] { text });
            }
            else
            {
                textBox1.Text = text;
            }
		}

		#endregion

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            remotableObject.PutCmd(textBox2.Text);
        }
	}
}
