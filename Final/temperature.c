#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "MQTTClient.h"

#define ADDRESS     "localhost:1883"
#define CLIENTID    "temperature"
#define QOS         0

MQTTClient client;


double get_temp() {
    // Aquí se debiera obtener la temperatura del sensor
    return (double)rand();
}


// Esta función obtiene la temperatura y la publica
void report_temp() {

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    char* buff;
    char* payload = gcvt(get_temp(),4,buff);
    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, "temperature", &pubmsg, &token);
}


int main(int argc, char* argv[]) {
    
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

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
