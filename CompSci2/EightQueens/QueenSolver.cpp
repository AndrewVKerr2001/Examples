#include <iostream>
#include "QueenSolver.hpp"

#define BOARD_SIZE 8
#define QUEEN_COUNT 8

char board[BOARD_SIZE][BOARD_SIZE];

void InitBoard() {
	for(int x = 0; x < BOARD_SIZE; x++)
		for(int y = 0; y < BOARD_SIZE; y++)
			board[y][x] = 0;
}

bool canPlace(int row, int col) {
	for(int i = 0; i < BOARD_SIZE; i++) {
		if(board[col][(row+i) % 8] == 'Q') return false;
		if(board[(col+i)%8][row] == 'Q') return false;
		if(col - i >= 0 && row - i >= 0){
			if(board[col-i][row-i] == 'Q') return false;
		}
		if(col - i >= 0 && row + i < 8){
			if(board[col-i][row+i] == 'Q') return false;
		}
	}
	return true;
}

bool solveQueen(int fromCol) {
	if(fromCol >= BOARD_SIZE) return true;
	for(int row = 0; row < BOARD_SIZE; row++){
		if(canPlace(row,fromCol)) {
			board[fromCol][row] = 'Q';
			bool bItWorked = solveQueen(fromCol+1);
			
			if(bItWorked) return true;
			
			board[fromCol][row] = ' ';
		}
	}
	return false;
}

void solveBoard() {
	solveQueen(0);
}

void printBoard() {
	std::cout << "+";
	for(int j = 0; j < BOARD_SIZE; j++){
		std::cout << "---+";
	}
	std::cout << std::endl;
	for(int i = 0; i < BOARD_SIZE; i++){
		std::cout << "|";
		for(int j = 0; j < BOARD_SIZE; j++){
			std::cout << " " << (board[i][j] ? board[i][j] : ' ') << " |";
		}
		std::cout << std::endl << "+";
		for(int j = 0; j < BOARD_SIZE; j++){
			std::cout << "---+";
		}
		std::cout << std::endl;
	}
}
