#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "MQTTClient.h"

#define CLIENTID    "temperature"
#define QOS         0

char* address = "tcp://localhost:1883";
MQTTClient client;


double get_temp() {
    // Aquí se debiera obtener la temperatura del sensor
    return (double)rand();
}

// Esta función obtiene la temperatura y la publica
void report_temp() {

    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    char* buff;
    char* payload = gcvt(get_temp(),4,buff);
    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    MQTTClient_publishMessage(client, "temperature", &pubmsg, NULL);
}

int main(int argc, char* argv[]) {
    
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    if (argc >= 2) {
        address = argv[1];
    }

    MQTTClient_create(&client, address, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Fallo en la conexión con codigo de error %d\n", rc);
        exit(EXIT_FAILURE);
    } else {
        printf("El sensor se encuentra en linea\n");
    }

    // El sensor reportará la temperatura cada 1 segundo
    while(1) {
        sleep(1);
        report_temp();
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return rc;
}
