CC=gcc

program: TicTacToe.c 
	$(CC) -c TicTacToe.c -lpaho-mqtt3c -o Header Files

validInputText:
	a.exe <validInput.txt
	print("expected: x")