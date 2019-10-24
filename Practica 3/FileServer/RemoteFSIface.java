/* Needed for defining remote method/s */
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RemoteFSIface extends Remote {

  public int write(String fileName, int amount, byte[] data) throws RemoteException;

  public FileData read(String fileName, int offset, int amount) throws RemoteException;

}