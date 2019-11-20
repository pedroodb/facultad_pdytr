package examples.Ej1;

import jade.util.leap.Serializable;

public class ContainerInfo implements Serializable{

    private String name;
    private long freeMemory;
    private long processingTime;

    public ContainerInfo(String name){
        this.name = name;
    }

    public String getName(){
        return this.name;
    }

    public long getFreeMemory(){
        return this.freeMemory;
    }

    public void setFreeMemory(long freeMemory){
        this.freeMemory = freeMemory;
    }

    public long getProcessingTime(){
        return this.processingTime;
    }

    public void setProcessingTime(long processingTime){
        this.processingTime = processingTime;
    }
    
}