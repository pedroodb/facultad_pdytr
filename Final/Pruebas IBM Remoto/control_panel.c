#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define CLIENTID    "a:yj7gpz:kjetrawpdj"
#define ADDRESS     "yj7gpz.messaging.internetofthings.ibmcloud.com:1883"
#define QOS         0

MQTTClient client;
char* temperature;


int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    // Obtener el topico especifico en topic
    char* topic = strtok(topicName, "/");
    while (strcmp(topic, "evt") != 0) {topic = strtok(NULL, "/");}
    topic = strtok(NULL, "/");

    // Se parsea el contenido que llega en formato json
    char* cont = strtok((char*)message->payload, ":");
    cont = strtok(NULL, "}");

    if (strcmp(topic, "temperature") == 0) {
        temperature = malloc(1 + strlen (cont));
        strcpy(temperature, cont);
    }
    else if (strcmp(topic, "switch_status") == 0) {
        printf("La luz se encuentra %s\n", (strcmp(cont, "true") == 0) ? "encendida" : "apagada");
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
    MQTTClient_publishMessage(client, "iot-2/type/Switch/id/LIGHTSWITCH1/cmd/switch_request/fmt/json", &pubmsg, NULL);
}

void connlost(void *context, char *cause) {
    printf("\nSe perdió la conexión, causa: %s\n", cause);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = "a-yj7gpz-kjetrawpdj";
    conn_opts.password = "x0s?gW0-k4g4_eryaj";
    
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, NULL);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Fallo en la conexión con codigo de error %d\n", rc);
        exit(EXIT_FAILURE);
    }
    
    MQTTClient_subscribe(client, "iot-2/type/Sensor/id/TEMPERATURE1/evt/temperature/fmt/json", QOS);
    MQTTClient_subscribe(client, "iot-2/type/Switch/id/LIGHTSWITCH1/evt/switch_status/fmt/json", QOS);

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
                msg_to_switch("{request:\"toggle\"}");
                printf("Se modificó el estado de la luz\n");
                break;
            case '3':
                msg_to_switch("{request:\"status\"");
                break;
            default:
                break;
        }
    } while(ch!='Q' && ch != 'q');

    printf("Desconectando\n");
    MQTTClient_unsubscribe(client, "iot-2/type/Sensor/id/TEMPERATURE1/evt/temperature/fmt/json");
    MQTTClient_unsubscribe(client, "iot-2/type/Switch/id/LIGHTSWITCH1/evt/switch_status/fmt/json");
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
