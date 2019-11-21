import jade.core.*;

import java.nio.file.Files;
import java.nio.file.Paths;

public class Ej2 extends Agent {

	Location origin;
	String destiny;
	String file;
	int result = 0;

	// Ejecutado por unica vez en la creacion
	public void setup() {
		this.origin = here();
		this.destiny = (String)getArguments()[0];
		this.file = (String)getArguments()[1];
		
		System.out.println("\n\nHola, agente con nombre local " + getLocalName());
		System.out.println("Se conectara a " + this.destiny + " para leer el archivo " + this.file);
		// Para migrar el agente
		try {
			ContainerID destino = new ContainerID(this.destiny, null);
			System.out.println("Migrando el agente a " + destino.getID() + "\n\n\n");
			doMove(destino);
		} catch (Exception e) {
			System.out.println("\n\n\nNo fue posible migrar el agente\n\n\n");
		}
	}

	// Ejecutado al llegar a un contenedor como resultado de una migracion
	protected void afterMove() {
		if(here().getID().equals(this.origin.getID())) {
			System.out.println("\n\n\nEl resultado es " + this.result + "\n\n\n");
		} else {
			System.out.println("\n\nHola, agente migrado con nombre local " + getLocalName());
			System.out.println("se tratara de acceder al archivo " + this.file);
			try {
				String[] numbers = new String(Files.readAllBytes(Paths.get(this.file))).split(",");
				for(String n : numbers){
					this.result += Integer.parseInt(n);
				}
			} catch (Exception e) {
				this.result = -1;
				e.printStackTrace();
			}
			doMove(this.origin);
		}
	}
}
