package examples.Ej1;

import jade.core.*;

public class Ej1 extends Agent{

    //Para calcular el tiempo que se toma en recorrer todos los contenedores.
    long startOfTotalTime;
    long endOfTotalTime;

    Location origin;

    ContainerInfo[] containersInfo;
    int indexContainerActual;

    protected void setup(){

        try {
            this.chequearYSetearContenedores();
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.print("Soy el agente " + getName() + " y visitare los contenedores: ");
        for (ContainerInfo container : containersInfo) {
            System.out.print(container.getName() + " ");
        }
        System.out.println("\n\n");

        indexContainerActual = 0;
        startOfTotalTime = System.currentTimeMillis();
        this.doMoveAgent();
    }

    protected void afterMove(){
        //Guarda la info del contenedor actual
        long startOfProcessingTime = System.currentTimeMillis();
        System.out.println("Soy el agente " + getName() + " y estoy en el contenedor " + containersInfo[indexContainerActual].getName());
        ContainerInfo current = containersInfo[indexContainerActual];
        current.setFreeMemory(java.lang.Runtime.getRuntime().freeMemory());
        long endOfProcessingTime = System.currentTimeMillis();
        current.setProcessingTime(endOfProcessingTime - startOfProcessingTime);

        //Chequea si es el ultimo
        if((containersInfo.length - 1) == indexContainerActual){
            endOfTotalTime = System.currentTimeMillis();
            System.out.println("He recorrido todos los contenedores especificados y me ha llevado un total de " + (endOfTotalTime - startOfTotalTime) + " milisegundos.");
            this.printInfoContainers();
        }else{
            indexContainerActual++;        
            this.doMoveAgent();
        }
    }

    private void chequearYSetearContenedores() throws Exception{
        Object[] args = getArguments();
        if(args.length == 0){
            throw new Exception("No se especificaron contenedores a visitar!");
        }
        containersInfo = new ContainerInfo[args.length + 1];
        for (int i = 0; i < args.length; i++) {
            containersInfo[i] = new ContainerInfo((String)args[i]);
        }
        containersInfo[args.length] = new ContainerInfo(here().getName());
    }

    private void printInfoContainers(){
        System.out.println("Esta es la informacion que obtuve de los contenedores: \n\n");
        for (ContainerInfo container : containersInfo) {
            System.out.println("Contenedor: " + container.getName());
            System.out.println("Memoria disponible: " + container.getFreeMemory() + " bytes");
            System.out.println("Tiempo de procesamiento: " + container.getProcessingTime() + "milisegundos\n");
        }
    }

    private void doMoveAgent(){
        try {
            ContainerID destino = new ContainerID(containersInfo[indexContainerActual].getName(), null);
            System.out.println("Migrando a destino: " + destino.getID());
            doMove(destino);
        } catch (Exception e) {
            System.out.println("NO SE PUDO MIGRAR AL AGENTE.");
        }
    }

}