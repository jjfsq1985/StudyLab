using System;

namespace RemotableObjects
{
    public delegate void ServerEventHandler(string strCmd);
	public interface IObserver
	{
		void Notify(string text);
	}

}
