import java.io.Serializable;

public class FileData implements Serializable{

  private byte[] data;
  private boolean finished;
  private int amount;

  public FileData(byte[] data, boolean finished, int amount) {
    this.data = data;
    this.finished = finished;
    this.amount = amount;
  }

  public byte[] getData() {
    return data;
  }

  public boolean finished() {
    return finished;
  }

  public int getAmount() {
    return amount;
  }

}