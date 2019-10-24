/* Needed for implementing remote method/s */
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
/* This class implements the interface with remote methods */

public class FileServer extends UnicastRemoteObject implements RemoteFSIface {

  private String address = "FS/"; 
  
  protected FileServer() throws RemoteException {
    super();
    File root = new File(this.address);
    if(!root.exists()){
      root.mkdir();
    }
  }

  public int write(String fileName, int amount, byte[] data) throws RemoteException{
    try {
      FileOutputStream fos = new FileOutputStream(this.address + fileName);
      fos.write(data,0,amount);
      fos.close();
      System.out.println(amount + " bytes written into " + fileName);
      return amount;
    } catch (IOException e) {
      e.printStackTrace();
      return -1;
    }
  }

  public byte[] read(String fileName, int offset, int amount) throws RemoteException{
    try {
      File file = new File(this.address + fileName);
      byte[] bytesArray = new byte[amount]; 
      FileInputStream stream = new FileInputStream(file);
      stream.skip(offset);
      stream.read(bytesArray);
      stream.close();
      return bytesArray;
    } catch (IOException e) {
      e.printStackTrace();
      return null;
    }
  }

}