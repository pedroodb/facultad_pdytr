#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "localhost:1883"
#define CLIENTID    "switch"
#define QOS         0

MQTTClient client;
int switch_status = 0;


int toggle() {
    return (switch_status = switch_status == 0 ? 1 : 0);
}

void report_status() {

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    char* payload = switch_status ? "1" : "0";
    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, "switch/status", &pubmsg, &token);
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {

    char* content = (char*)message->payload;

    // El mensaje contiene el comando (desde la consola se debe comparar agregando un \n, chequear si tambiend desde C)
    if (strcmp(content, "toggle") == 0) {
        printf("Se activó el interruptor, el estado es ahora %d\n", toggle());
    }
    else if (strcmp(content, "status") == 0) {
        printf("Reportando estado %d\n", switch_status);
        report_status();
    }
    else {
        printf("Se recibió un comando inválido\n");
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause) {
    printf("\nSe perdió la conexión, causa: %s\n", cause);
}

int main(int argc, char* argv[]) {

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, NULL);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Fallo en la conexión con codigo de error %d\n", rc);
        exit(EXIT_FAILURE);
    } else {
        printf("El sensor se encuentra en linea\n");
    }
    
    
    MQTTClient_subscribe(client, "switch/request", QOS);

    while (1) {}

    MQTTClient_unsubscribe(client, "switch/request");
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
