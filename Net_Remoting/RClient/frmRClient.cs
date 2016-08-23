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
		
		MyRemotableObject remoteObject;
		
		private System.ComponentModel.Container components = null;

		public frmRCleint()
		{

			InitializeComponent();

			//************************************* TCP *************************************//
			// using TCP protocol
			// running both client and server on same machines
			TcpChannel chan = new TcpChannel();
			ChannelServices.RegisterChannel(chan,false);
			// Create an instance of the remote object
			remoteObject = (MyRemotableObject) Activator.GetObject(typeof(MyRemotableObject),"tcp://localhost:8080/HelloWorld");
			// if remote object is on another machine the name of the machine should be used instead of localhost.
			//************************************* TCP *************************************//
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
            this.textBox1.Size = new System.Drawing.Size(656, 246);
            this.textBox1.TabIndex = 0;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // frmRCleint
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
            this.ClientSize = new System.Drawing.Size(656, 246);
            this.Controls.Add(this.textBox1);
            this.Name = "frmRCleint";
            this.Text = "RemoteClient";
            this.ResumeLayout(false);
            this.PerformLayout();

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
	}
}
