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
char square[10] = { '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }; //the tictactoe board
//function to print board, used after every move made my a player
void printBoard()
{
    //system("cls");
    printf("\n\n\tTic Tac Toe\n\n");

    printf("Player 1 is X-----Player 2 is O \n\n\n");


    printf("+-----------+\n");
    printf("| %c | %c | %c |\n", square[1], square[2], square[3]);
    printf("+-----------+\n");
    printf("| %c | %c | %c |\n", square[4], square[5], square[6]);
    printf("+-----------+\n");
    printf("| %c | %c | %c |\n", square[7], square[8], square[9]);
    printf("+-----------+\n");
}

//main

int main(int argc, char *argv[]) {
    int rc;
    MQTTClient client;

    MQTTClient_create(&client, ADDRESS, CLIENTID, 0, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;
    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    } else {
        printf("Connected to MQTT Broker!\n");
    }
    // subscribe topic
    MQTTClient_subscribe(client, TOPIC, QOS);
    char payload[16];
	
    for (int i = 0; i < 100; i += 1) {
        // publish message to broker
        //snprintf(payload, 16, "message-%d", i);
        //publish(client, TOPIC, payload);
		printf("\n Iteration %d\n",i);
		char* x;
		
		scanf("%s", x);
			
		publish(client, TOPIC, x);
		
        sleep(1);
    }


	
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
    return rc;
}

//code for mqtt

//copy and paste ticTacToe from class assignment

//most likey add mqtt integration

