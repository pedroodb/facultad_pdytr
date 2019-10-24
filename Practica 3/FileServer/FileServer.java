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
      File file = new File(this.address + fileName);
      FileOutputStream fos = new FileOutputStream(file,true);
      fos.write(data,0,amount);
      fos.close();
      System.out.println("Se escribieron " + amount + " bytes en " + fileName);
      return amount;
    } catch (IOException e) {
      System.out.println("Error leyendo archivo " + fileName);
      return -1;
    }
  }

  public FileData read(String fileName, int offset, int amount) throws RemoteException{
    try {
      File file = new File(this.address + fileName);
      FileInputStream fis = new FileInputStream(file);
      fis.skip(offset);
      byte[] bytesArray = new byte[Math.min(amount,fis.available())]; 
      int amountRead = fis.read(bytesArray);
      boolean finished = fis.available() == 0;
      fis.close();
      System.out.println("Se leyeron " + amountRead + " bytes de " + fileName);
      return new FileData(bytesArray,finished,amountRead);
    } catch (IOException e) {
      System.out.println("Error leyendo archivo " + fileName);
      return null;
    }
  }

}