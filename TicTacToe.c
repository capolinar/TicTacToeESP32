//headers go here
#include "stdlib.h"
#include "string.h"
#include <stdio.h>
#include  <stdlib.h> 
#include  <time.h>
#include <stdbool.h>
#include "unistd.h"
#include "MQTTClient.h"

#define ADDRESS     "tcp://broker.emqx.io:1883"
#define USERNAME    "emqx"
#define PASSWORD    "public"
#define CLIENTID    "eqmx_test"
#define QOS         2
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

bool msgd = false;
char *esp;
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char *payload = message->payload;
    printf("Received `%s` from `%s` topic \n", payload, topicName);
	msgd = true;
	esp = message->payload;
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

int win() //function that returns 1 if game is over, -1 if in progress, and 0 if over and no result(TIE)
{                                                             //each of these check if X or O are in a row
                                                        //essentially checks all combinations of win conditions
   
    if (square[1] != ' ' && square[2] != ' ' && square[1] == square[2] && square[2] == square[3]) //top row
        return 1;
        
    else if (square[4] != ' ' && square[5] != ' ' &&  square[4] == square[5] && square[5] == square[6])//middle row
        return 1;
        
    else if (square[7] != ' ' && square[8] != ' ' && square[7] == square[8] && square[8] == square[9])//bottom row
        return 1;
        
    else if (square[1] != ' ' && square[4] != ' ' && square[1] == square[4] && square[4] == square[7])//left column
        return 1;
        
    else if (square[2] != ' ' && square[5] != ' ' && square[2] == square[5] && square[5] == square[8])//middle column
        return 1;
        
    else if (square[3] != ' ' && square[6] != ' ' && square[3] == square[6] && square[6] == square[9])//bottom column
        return 1;
        
    else if (square[1] != ' ' && square[5] != ' ' && square[1] == square[5] && square[5] == square[9])//left diaganol
        return 1;
        
    else if (square[5] != ' ' && square[5] != ' ' && square[3] == square[5] && square[5] == square[7])//right diagnol
        return 1;
        
    else if (square[1] != ' ' && square[2] != ' ' && square[3] != ' ' && //checks if any remaining spaces
        square[4] != ' ' && square[5] != ' ' && square[6] != ' ' && square[7] 
        != ' ' && square[8] != ' ' && square[9] != ' ')

        return 0; //if no remaining spaces, game is over(tie)
    else
        return  - 1; //if there is remaining spaces, game is in progress

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
////////////////////////////////////////////////End of MQTT main items


/////////////////////////////////////////////////Start of tictactoe
//prompt user for game they wish to play
    printf("Welcome to Tic Tac Toe! \n"); 

    printf("Make sure to enter a number between 1 and 9 that correspond to these spaces:\n\n");
    printf("+-----------+\n");
    printf("| 1 | 2 | 3 | \n");
    printf("+-----------+\n");
    printf("| 4 | 5 | 6 | \n");
    printf("+-----------+\n");
    printf("| 7 | 8 | 9 | \n");
    printf("+-----------+\n\n");

    printf("1---ESP32 vs 2---Person or Script \n \n");

	printBoard();

	int w = win();//w is win status
	 while(w == -1)//keeps loop while game is in progress
        {
            bool valid = false;
            while(!valid)
            {
                printf("Player 1: make your move\n\n"); //player1 prompt
				int choice;
				while(msgd == false)
				{
					//choice = strtol(esp,NULL,10);
				}
				choice = strtol(esp,NULL,10);
                
                 if(choice > 0 && choice < 10 && msgd == true)
                 {
                    if(square[choice] == ' ')
                    {
                        square[choice] = 'X';
                        printBoard();
                        valid = true;
						msgd = false;
                    }
                    else if(square[choice] != ' ')
                    {
                        printf("Space is already taken. \n");
						msgd = false;
                       
                    }

                 }
                 else
                 {
                    printf("Number is not between 1 and 9, please enter again\n");
                 }
            

            }
            w = win();
            if(w == 1) //this updates win status, breaks loop if game is over
            {
                printf("\nPlayer 1 has won!!!\n\n");
                break;
            }
            else if(w == 0)
            {
                printf("\nIt is a tie, no winners.\n\n");
                break;
            }
            //////Player 2's turn(Person)
            valid = false;
            while(!valid)
            {
                printf("Player 2: make your move\n\n"); //player1 prompt
                int choice;
                scanf("%d", &choice);
                 if(choice > 0 && choice < 10)
                 {
                    if(square[choice] == ' ')
                    {
                        square[choice] = 'O';
                        printBoard();
                        valid = true;
                    }
                    else if(square[choice] != ' ')
                    {
                        printf("Space is already taken. \n");
                       
                    }

                 }
                 else
                 {
                    printf("Number is not between 1 and 9, please enter again\n");
                 }

            }
            w = win();
            if(w == 1) //this updates win status, breaks loop if game is over
            {
                printf("\nPlayer 2 has won!!!\n\n"); //win message for player 2
                break;
            }
            else if(w == 0)
            {
                printf("\nIt is a tie, no winners.\n\n");
                break;
            }




        }





	
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

