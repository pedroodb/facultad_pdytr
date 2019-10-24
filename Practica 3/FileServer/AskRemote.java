/*
* AskRemote.java
* a) Looks up for the remote object
* b) "Makes" the RMI
*/
import java.rmi.Naming; /* lookup */
import java.rmi.RemoteException;
import java.rmi.registry.Registry; /* REGISTRY_PORT */
import java.util.Scanner;
import java.util.regex.Pattern;
import java.io.*;

public class AskRemote {

  public static void send(RemoteFSIface fs, String fname) {
    try {
      File file = new File(fname);
      FileInputStream is = new FileInputStream(file);
      byte[] fileData = new byte[2048];
      int written = 0;
      while (is.available() > 0) {
        int toSend = is.read(fileData);
        written += (int) fs.write(fname,10,fileData);
      }
      is.close();
      System.out.println("Se enviaron " + written + " bytes de " + fname);
    } catch (RemoteException e) {
      e.printStackTrace();
      System.out.print("Error en la escritura\n");
    } catch (IOException e) {
      e.printStackTrace();
      System.out.print("Error accediendo al archivo local\n");
    }
  }

  public static void main(String[] args) {
    /* Look for hostname and msg length in the command line */
    if (args.length != 1) {
      System.out.println("1 argument needed: (remote) hostname\n");
      System.exit(1);
    }
    try {
      String rname = "//" + args[0] + ":" + Registry.REGISTRY_PORT + "/remote";
      RemoteFSIface fs = (RemoteFSIface) Naming.lookup(rname);
      Scanner sc = new Scanner(System.in);
      cli: while (true) {
        String command = sc.next(Pattern.compile("\\S+"));
        String fileName;
        switch (command) {
          case "send":
            fileName = sc.next(Pattern.compile("\\S+"));
            send(fs,fileName);
            break;
          case "get":
            fileName = sc.next(Pattern.compile("\\S+"));
            break;
          case "exit":
            sc.close();
            break cli;
          default:
            System.out.print("Comando invalido");
            break;
        }
      }
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}