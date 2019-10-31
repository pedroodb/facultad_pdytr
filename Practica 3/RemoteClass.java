/*
* RemoteClass.java
* Just implements the RemoteMethod interface as an extension to
* UnicastRemoteObject
*
*/
/* Needed for implementing remote method/s */
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.concurrent.TimeUnit;
/* This class implements the interface with remote methods */
public class RemoteClass extends UnicastRemoteObject implements IfaceRemoteClass
{
protected RemoteClass() throws RemoteException
{
super();
}
/* Remote method implementation */
public byte[] sendThisBack(byte[] data) throws RemoteException
{
  System.out.println("Sleeping...");  //codigo agregado
  try {                               //codigo agregado
      TimeUnit.SECONDS.sleep(2);      //codigo agregado
  } catch (Exception e) {             //codigo agregado
      e.printStackTrace();            //codigo agregado
  }                                   //codigo agregado
  System.out.println("Data back to client");
  return data;
}
}