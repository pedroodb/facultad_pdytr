#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define CLIENTID    "control_panel"
#define QOS         0

char* address = "tcp://localhost:1883";
MQTTClient client;

char* temperature;


int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    
    if (strcmp(topicName, "temperature") == 0) {
        temperature = malloc(1 + strlen ((char*)message->payload));
        strcpy(temperature, (char*)message->payload);
    }
    else if (strcmp(topicName, "switch/status") == 0) {
        printf("La luz se encuentra %s\n", atoi((char*)message->payload) ? "encendida" : "apagada");
    }
    else {
        printf("Se recibió un mensaje inválido\n");
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void msg_to_switch(char* payload) {

    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    MQTTClient_publishMessage(client, "switch/request", &pubmsg, NULL);
}

void connlost(void *context, char *cause) {
    printf("\nSe perdió la conexión, causa: %s\n", cause);
}

int main(int argc, char* argv[]) {

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    if (argc >= 2) {
        address = argv[1];
    }

    MQTTClient_create(&client, address, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, NULL);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Fallo en la conexión con codigo de error %d\n", rc);
        exit(EXIT_FAILURE);
    }
    
    MQTTClient_subscribe(client, "temperature", QOS);
    MQTTClient_subscribe(client, "switch/status", QOS);

    printf("Elija una opcion entre:\n");
    printf(" 1) Consultar la temperatura\n");
    printf(" 2) Prender/apagar la luz\n");
    printf(" 3) Consultar el estado de la luz\n");
    do {
        ch = getchar();
        switch (ch) {
            case '1':
                if (temperature) {
                    printf("La temperatura es %s\n", temperature);
                } else {
                    printf("No se pudo acceder al valor de la temperatura\n");
                }
                break;
            case '2':
                printf("Se modificó el estado de la luz\n");
                msg_to_switch("toggle");
                break;
            case '3':
                msg_to_switch("status");
                break;
            default:
                break;
        }
    } while(ch!='Q' && ch != 'q');

    MQTTClient_unsubscribe(client, "temperature");
    MQTTClient_unsubscribe(client, "switch/status");
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
