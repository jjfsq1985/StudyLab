using System;
using System.Windows.Forms;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;

namespace RemotableObjects
{
    public class EventWrapper : MarshalByRefObject
    {
        public event ServerEventHandler LocalEvent;
        public EventWrapper()
		{
		
		}

        public void Response(string command)
        {
            if (LocalEvent != null)
                LocalEvent(command);
        }

    }

	public class MyRemotableObject : MarshalByRefObject
	{
        public event ServerEventHandler ServerEvent;
		public MyRemotableObject()
		{
		
		}

		public void SetMsg(string message)
		{
			Cache.GetInstance().MessageString = message;
		}

        public void PutCmd(string command)
        {
            if (ServerEvent != null)
                ServerEvent(command);
        }
        public override object InitializeLifetimeService()
        {
            return null;
        }
	}
}
