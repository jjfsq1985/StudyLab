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
	
		private System.ComponentModel.Container components = null;

		public frmRServer()
		{
			
			InitializeComponent();
			remotableObject = new MyRemotableObject();
			
			//************************************* TCP *************************************//
			// using TCP protocol
			TcpChannel channel = new TcpChannel(8080);
			ChannelServices.RegisterChannel(channel,false);
			RemotingConfiguration.RegisterWellKnownServiceType(typeof(MyRemotableObject),"HelloWorld",WellKnownObjectMode.Singleton);
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
			this.SuspendLayout();
			// 
			// textBox1
			// 
			this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.textBox1.Location = new System.Drawing.Point(0, 0);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.ReadOnly = true;
			this.textBox1.Size = new System.Drawing.Size(656, 246);
			this.textBox1.TabIndex = 1;
			this.textBox1.Text = "";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(656, 246);
			this.Controls.Add(this.textBox1);
			this.Name = "Form1";
			this.Text = "RemoteServer";
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
	}
}
