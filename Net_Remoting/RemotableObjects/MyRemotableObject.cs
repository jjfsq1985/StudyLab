using System;
using System.Windows.Forms;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;

namespace RemotableObjects
{

	public class MyRemotableObject : MarshalByRefObject
	{

		public MyRemotableObject()
		{
		
		}

		public void SetMsg(string message)
		{
			Cache.GetInstance().MessageString = message;
		}

	}
}
