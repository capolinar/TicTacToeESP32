//headers go here
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "MQTTClient.h"

#define ADDRESS     "tcp://broker.emqx.io:1883"
#define USERNAME    "emqx"
#define PASSWORD    "public"
#define CLIENTID    "eqmx_test"
#define QOS         0
#define TOPIC       "emqx/c-test"
#define TIMEOUT     10000L

void publish(MQTTClient client, char *topic, char *payload) {
    MQTTClient_message message = MQTTClient_message_initializer;
    message.payload = payload;
    message.payloadlen = strlen(payload);
    message.qos = QOS;
    message.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &message, &token);
    MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Send `%s` to topic `%s` \n", payload, TOPIC);
}

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char *payload = message->payload;
    printf("Received `%s` from `%s` topic \n", payload, topicName);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

//setup for mqtt

//setup for tic tac toe game

//main

//code for mqtt

//copy and paste ticTacToe from class assignment

//most likey add mqtt integration

