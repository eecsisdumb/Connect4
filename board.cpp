#ifndef iostream
#include <iostream>
#endif

#include "board.h"

Board::Board() {
	PieceType empty = Empty;

	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			data[i][j] = empty;
		}
	}

	nextPlayer_to_move = Player1;
}

Board::Board(const string &fen) {
	int currentRowToFill = 0;
	int numberOfSpacesInGivenRow = 0;
	int filledColumn = 0;

	for (int i = 0; i < fen.length() - 2; i++) {
		if (isdigit(fen[i])) {
			numberOfSpacesInGivenRow = fen[i] - 48;
			int j = filledColumn;
			for (; j < numberOfSpacesInGivenRow + filledColumn && filledColumn < NUM_COLS; j++)
				data[currentRowToFill][filledColumn] = Empty;
			filledColumn = j;

			numberOfSpacesInGivenRow = 0;
		}

		if (fen[i] == 'x') {
			data[currentRowToFill][filledColumn] = Player1;
			filledColumn++;
		}


		if (fen[i] == 'o') {
			data[currentRowToFill][filledColumn] = Player2;
			filledColumn++;
		}

		if (fen[i] == '/') {
			currentRowToFill++;
			filledColumn = 0;
			numberOfSpacesInGivenRow = 0;
		}

	}

	if (fen[fen.length() - 1] == 'x') {
		nextPlayer_to_move = Player1;
	}

	else if (fen[fen.length() - 1] == 'o') {
		nextPlayer_to_move = Player2;
	}


}

void Board::printBoard_as_FENstring(ostream &os) const {
	int spaceInBoard = 0;
	for (int currentRow = NUM_ROWS - 1; currentRow >= 0; currentRow--) {
		spaceInBoard = 0;
		for (int i = 0; i < NUM_COLS; i++) {
			if (data[currentRow][i] == Empty) {
				spaceInBoard++;
			}

			else if (data[currentRow][i] == Player1) {
				if (spaceInBoard > 0)
					os << spaceInBoard;
				spaceInBoard = 0;
				os << "x";
			}

			else if (data[currentRow][i] == Player2) {
				if (spaceInBoard > 0)
					os << spaceInBoard;
				spaceInBoard = 0;
				os << "o";
			}
		}
		if (spaceInBoard > 0 && spaceInBoard <= 7)
			os << spaceInBoard;
		if (currentRow > 0)
			os << "/";
	}

	if (nextPlayer_to_move == Player1) {
		os << " " << "x";
	}

	else if (nextPlayer_to_move == Player2) {
		os << " " << "o";
	}
}

PieceType Board::atLocation(int row, int col) {
    return data[row][col];
}

void Board::prettyPrintBoard(ostream &os) const {
	os << endl;
    for (int row = NUM_ROWS - 1; row >= 0; row--)  {
        os << "     +---+---+---+---+---+---+---+"
           << endl
           << "    ";
        
        for (int col = 0; col < NUM_COLS; col++)   {
            os << " | " ;
            if ( data[row][col] == Player1)
                os <<  PLAYER1_TOKEN;
            else if (data[row][col] == Player2)
                os << PLAYER2_TOKEN;
            else
                os << EMPTY_TOKEN;
        }
        os << " |" << endl;
    }
    os << "     +---+---+---+---+---+---+---+" << endl
       <<"  col  1   2   3   4   5   6   7" << endl;
    return;
}

int Board::toMove() const {
	if (nextPlayer_to_move == Player1)
		return 2;

	else if (nextPlayer_to_move == Player2)
		return 1;

	return 0;
}

Result Board::makeMove(int col) {
	int givenRowToInsert = getFirstFreeRow(col);
	int givenColToInsert = col;

	if (!inBounds(givenRowToInsert, givenColToInsert)) {
		return IllegalMove;
	}

	if (isWin(givenRowToInsert, givenColToInsert)) {
		return Win;
	}

	else if (isBoardFull()) {
		return Draw;
	}

	if (toMove() == 1) {
			data[givenRowToInsert][givenColToInsert] = Player1;
	}

	else if (toMove() == 2) {
		data[givenRowToInsert][givenColToInsert] = Player2;
	}

	updateToMove();

	return NoResult;
}

int Board::getFirstFreeRow(int col) const {
	for (int i = 0; i < NUM_ROWS; i++)
		if (data[i][col] == Empty)
			return i;

	return NUM_ROWS;
}

PieceType Board::updateToMove() {

	if (nextPlayer_to_move == Player1) {
		nextPlayer_to_move = Player2;
		return Player1;
	}

	else if (nextPlayer_to_move == Player2) {
		nextPlayer_to_move = Player1;
		return Player2;
	}
}

bool Board::isBoardFull() const {
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (data[i][j] == Empty)
				return false;
		}
	}

	return true;
}

bool Board::inBounds(int row, int col) const {
	if (row < 0)
		return false;

	if (row >= NUM_ROWS)
		return false;

	if (col < 0)
		return false;

	if (col >= NUM_COLS)
		return false;

	return true;
}

int Board::piecesInDirection(int row, int col, int dRow, int dCol) const {
	PieceType somePlayer = data[row][col];
	int matchingPieces = 0;

	row = row + dRow;
	col = col + dCol;

	while (data[row][col] == somePlayer && row >= 0 && col >= 0 && row < NUM_ROWS && col < NUM_COLS) {
		matchingPieces++;
		row = row + dRow;
		col = col + dCol;
	}

	return matchingPieces;
}

bool Board::isWin(int row, int col) const {
	if (data[row][col] == Empty) {
		return false;
	}

	int fourPiecesConsecutiveInRowDown = 1 + piecesInDirection(row, col, 1, 0);
	int fourPiecesConsecutiveInRowUp = 1 + piecesInDirection(row, col, -1, 0);
	int fourPiecesConsecutiveInColRight = 1 + piecesInDirection(row, col, 0, 1);
	int fourPiecesConsecutiveInColLeft = 1 + piecesInDirection(row, col, 0, -1);
	int fourPiecesConsecutiveInLeftDiagonalDown = 1 + piecesInDirection(row, col, 1, -1);
	int fourPiecesConsecutiveInRightDiagonalDown = 1 + piecesInDirection(row, col, 1, 1);
	int fourPiecesConsecutiveInLeftDiagonalUp = 1 + piecesInDirection(row, col, -1, -1);
	int fourPiecesConsecutiveInRightDiagonalUp = 1 + piecesInDirection(row, col, -1, 1);

	if (fourPiecesConsecutiveInColLeft == 4 || fourPiecesConsecutiveInColRight == 4 || fourPiecesConsecutiveInRowDown == 4 || fourPiecesConsecutiveInRowUp == 4 || fourPiecesConsecutiveInLeftDiagonalDown == 4) {
		return true;
	}

	if (fourPiecesConsecutiveInLeftDiagonalDown == 4 || fourPiecesConsecutiveInLeftDiagonalUp == 4 || fourPiecesConsecutiveInRightDiagonalDown == 4 || fourPiecesConsecutiveInRightDiagonalUp == 4) {
		return true;
	}
	
	return false;
}
