#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "MQTTClient.h"

#define CLIENTID    "d:yj7gpz:Sensor:TEMPERATURE1"
#define QOS         0
#define ADDRESS     "yj7gpz.messaging.internetofthings.ibmcloud.com:1883"

MQTTClient client;

double get_temp() {
    // Aquí se debiera obtener la temperatura del sensor
    return (double)rand();
}

// Esta función obtiene la temperatura y la publica
void report_temp() {

    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    char* temp = malloc(4);
    gcvt(get_temp(),4,temp);
    char* payload = malloc(strlen(temp) + strlen("{\"temperature\":}") + 1);
    sprintf(payload, "{\"temperature\":%s}", temp);

    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    MQTTClient_publishMessage(client, "iot-2/evt/temperature/fmt/json", &pubmsg, NULL);

    free(payload);
    free(temp);
}

void connlost(void *context, char *cause) {
    printf("\nSe perdió la conexión, causa: %s\n", cause);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = "use-token-auth";
    conn_opts.password = "PDyTR2020";

    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, connlost, NULL, NULL);

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
