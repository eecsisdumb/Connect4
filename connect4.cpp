#include "connect4.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

void terminalPlay(){

	string playerAName = "";
	string playerBName = "";
	string currentPlayer = "";
	Board someGame = getBoard(playerAName, playerBName);
	Result moveResult = NoResult;
	char playerInputFromKeyboard = ' ';
	int currentColumnSelected = 0;

	currentPlayer = playerAName;

	while (true) {

		someGame.prettyPrintBoard(cout);
		
		if (moveResult == Win) {
			cout << "Congratulations ";
			cout << "! You won!" << endl;
			return;
		}

		else if (moveResult == IllegalMove) {
			cout << "ILLEGAL MOVE: Try again" << endl;
		}

		else if (moveResult == Draw) {
			cout << "Draw!" << endl;
			return;
		}

		cout << "\nEnter: the column to move, q to quit, s to save" << endl;
		cout << " enter your move: ";

		cin >> playerInputFromKeyboard;

		if (tolower(playerInputFromKeyboard) == 'q') {
			break;
		}

		else if (tolower(playerInputFromKeyboard) == 's') {
			saveBoard(playerAName, playerBName, someGame);
		}
		
		else if (isdigit(playerInputFromKeyboard)) {

			currentColumnSelected = playerInputFromKeyboard - 48;
		
			moveResult = someGame.makeMove(currentColumnSelected - 1);

		}
	}

	cout << "Thanks for playing!" << endl;
}

Board getBoard(string &p1, string &p2) {
	char modeIndicatorCharacter = ' ';
	string givenUserInput = "";
	string gameStateFromFile = "";
	Board someBoard;
	ifstream gameInFile;

	cout << "Enter L to load a game, or P to play: " << flush;
	cin >> modeIndicatorCharacter;

	if (toupper(modeIndicatorCharacter) == 'P') {

		cout << "Player 1, enter your name: " << flush;
		
		cin >> givenUserInput;

		p1 = givenUserInput;

		cout << endl << "Player 2, enter your name: " << flush;

		cin >> givenUserInput;

		p2 = givenUserInput;

	}

	else if (toupper(modeIndicatorCharacter) == 'L') {
		cout << "Enter the filename: " << flush;
		
		cin >> givenUserInput;

		gameInFile.open(givenUserInput);

		getline(gameInFile, p1);

		getline(gameInFile, p2);

		getline(gameInFile, gameStateFromFile);

		someBoard = Board(gameStateFromFile);

	}
   
	return someBoard;
}

void saveBoard(string p1, string p2, const Board &board) {
	string providedSaveFilename = "";
	ofstream gameToFile;

	cout << "Enter the filename: " << flush;

	cin >> providedSaveFilename;

	gameToFile.open(providedSaveFilename);

	gameToFile << p1 << endl;

	gameToFile << p2 << endl;

	board.printBoard_as_FENstring(gameToFile);
	
	gameToFile.close();
}
