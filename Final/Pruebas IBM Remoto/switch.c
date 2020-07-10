#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "MQTTClient.h"

#define CLIENTID    "d:yj7gpz:Switch:LIGHTSWITCH1"
#define ADDRESS     "yj7gpz.messaging.internetofthings.ibmcloud.com:1883"
#define QOS         0

MQTTClient client;
int switch_status = 0;
int finished = 0;


int toggle() {
    return (switch_status = switch_status == 0 ? 1 : 0);
}

void report_status() {

    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    char* payload = switch_status ? "{\"status\":true}" : "{\"status\":false}";
    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    MQTTClient_publishMessage(client, "iot-2/evt/switch_status/fmt/json", &pubmsg, NULL);
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {

    char* content = (char*)message->payload;

    // El mensaje contiene el comando a ejecutar
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
    exit(EXIT_FAILURE);
}

void handle_sigint(int sig) {    
    finished = 1; 
}

int main(int argc, char* argv[]) {

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = "use-token-auth";
    conn_opts.password = "PDyTR2020";

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
    
    signal(SIGTSTP, handle_sigint);

    MQTTClient_subscribe(client, "iot-2/cmd/switch_request/fmt/json", QOS);
    report_status();
    while (!finished) {}

    printf("\nDesconectando\n");
    MQTTClient_unsubscribe(client, "iot-2/cmd/switch_request/fmt/json");
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
