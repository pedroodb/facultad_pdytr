/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     if (argc < 3) {
         fprintf(stderr,"Missing arguments\n");
         exit(1);
     }
     
     int msgsize = atoi(argv[2]);
     char *buffer = (char *) malloc(sizeof(char) * msgsize);
     struct sockaddr_in serv_addr, cli_addr;
     int n;
	 //CREA EL FILE DESCRIPTOR DEL SOCKET PARA LA CONEXION
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 //AF_INET - FAMILIA DEL PROTOCOLO - IPV4 PROTOCOLS INTERNET
	//SOCK_STREAM - TIPO DE SOCKET 
	
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     //ASIGNA EL PUERTO PASADO POR ARGUMENTO
	 //ASIGNA LA IP EN DONDE ESCUCHA (SU PROPIA IP)
	 portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
	 
	 //VINCULA EL FILE DESCRIPTOR CON LA DIRECCION Y EL PUERTO
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
			  
	 //SETEA LA CANTIDAD QUE PUEDEN ESPERAR MIENTRAS SE MANEJA UNA CONEXION		  
     listen(sockfd,5);
	 
	 // SE BLOQUEA A ESPERAR UNA CONEXION
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
				 
     //DEVUELVE UN NUEVO DESCRIPTOR POR EL CUAL SE VAN A REALIZAR LAS COMUNICACIONES
	 if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,msgsize);

	//LEE EL MENSAJE DEL CLIENTE
     n = read(newsockfd,buffer,msgsize);

     if (n < 0) error("ERROR reading from socket");

     //VERIFICACIÓN DE LLEGADA CORRECTA DE DATOS
     int xs = 0;
     int index = 0;
     while(index < msgsize){
       if(buffer[index] == 'X') xs++;
       index++;
     }

     //IMPRIME EL RESULTADO
     printf("Datos recibidos ");
     printf((xs != msgsize)? "con error": "correctamente" );
     printf("!\nCantidad de 'X' que se esperaba recibir: %d.\nSe recibieron: %d.\n", msgsize, xs);

	 //RESPONDE AL CLIENTE
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     free(buffer);
     return 0; 
}
