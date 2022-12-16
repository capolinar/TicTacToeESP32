#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "emqx_test"
#define TOPIC       "esp32/test"
#define PAYLOAD     "This is from a C program!"
#define QOS         1
#define TIMEOUT     100000L


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


bool msgd = false;

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char *payload = message->payload;
    printf("Received `%s` from `%s` topic \n", payload, topicName);
	msgd = true;

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main(int argc, char *argv[]) {

	char *filename = "espStat.txt";//filename to update

	MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
  
    // MQTT Connection parameters
    conn_opts.keepAliveInterval = 2000;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
  
    // Publish message
    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/10000), PAYLOAD, TOPIC, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
  

    	printf("\nDaemon is currently running\n");
	
	if (argc <= 1) {
        printf("daemonize.exe COMMAND [ARGUMENTS]");
        return 1;
    }

    char* command = argv[1];
    char* arguments = NULL;

    if (argc > 2) {
        // We only need to concatenate arguments if there
        // actually are arguments.
        int idx;

        // Calculate the length of all arguments
        size_t argLength = 0;
        for (idx = 1; idx < argc; idx++) {
            argLength += strlen(argv[idx] + 2 + 1); // One for the space, two for the quotes.
        }
        argLength--; // Strip the last space.

        // Reserve some memory and NULL the new string.
        //The + 1 is the space for the last null character.
        arguments = (char*)malloc(sizeof(char) * (argLength + 1));
        arguments[0] = 0;

        // Now concatenate the arguments.
        for (idx = 1; idx < argc; idx++) {
            strcat(arguments, "\"");
            strcat(arguments, argv[idx]);
            strcat(arguments, "\"");
            if (idx < argc - 1) {
                strcat(arguments, " ");
            }
        }
    }

    STARTUPINFO startInfo;
    PROCESS_INFORMATION process;

    ZeroMemory(&startInfo, sizeof(startInfo));
    ZeroMemory(&process, sizeof(process));

    startInfo.cb = sizeof(startInfo);

    // Tell the system to use/honor the ShowWindow flags.
    startInfo.dwFlags = STARTF_USESHOWWINDOW;
    // Tell the system that the main window of the process should be hidden.
    startInfo.wShowWindow = SW_HIDE;

    if (!CreateProcessA(
            command,            // application name
            arguments,            // command line arguments
            NULL,                // process attributes
            NULL,                // thread attributes
            FALSE,                // inherit (file) handles
            // Detach the process from the current console.
            DETACHED_PROCESS,   // creation flags
            NULL,                // environment
            NULL,                // current directory
            &startInfo,            // startup info
            &process)            // process information
        ) {

        printf("Creation of the process failed, trying to fetch error message...\n");

        long lastError = GetLastError();
        char* message = NULL;

        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_ARGUMENT_ARRAY |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS, // Flags
            NULL,               // source
            lastError,          // message identifier
            0,                  // language id
            (LPSTR)&message,   // message buffer
            0,                  // size of the message
            NULL                // arguments
        );

        printf(message);

        return 1;
    }



	FILE* ptr;
    char str[50];
    ptr = fopen("espStat.txt", "w+");
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
 
 
    while (fgets(str, 50, ptr) != NULL) {
        if(msgd)
		{
			if(str == "Don't run daemon")
			{
				//should cancel script as is
			}
			else{
				fprintf(ptr,"Script can run");
			}
		}
    }

			
   			
   	fclose(ptr);



    return 0;
}