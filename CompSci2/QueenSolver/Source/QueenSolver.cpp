#include <iostream>
#include <new>
#include "QueenSolver.hpp"

#define INDEX(x, y)  y * iSize + x

bool Board::canPlace(int row, int col) {
	for(int i = 0; i < iSize; i++) {
		if(pBoard[INDEX(col,(row+i) % iSize)] == 'Q') return false;
		if(pBoard[INDEX((col+i)%iSize,row)] == 'Q') return false;
		if(col - i >= 0 && row - i >= 0){
			if(pBoard[INDEX(col-i,row-i)] == 'Q') return false;
		}
		if(col - i >= 0 && row + i < iSize){
			if(pBoard[INDEX(col-i,row+i)] == 'Q') return false;
		}
	}
	return true;
}

bool Board::solveQueen(int fromCol) {
	if(fromCol >= iSize) return true;
	for(int row = 0; row < iSize; row++){
		if(canPlace(row,fromCol)) {
			pBoard[INDEX(fromCol,row)] = 'Q';
			bool bItWorked = solveQueen(fromCol+1);
			
			if(bItWorked) {
				return true;
			}
			
			pBoard[INDEX(fromCol,row)] = ' ';
		}
	}
	return false;
}

Board::Board(int iSize){

	//Setup
	this->iSize = iSize;
	this->pBoard = new char[iSize * iSize];
	
	//Check validity
	if(this->pBoard == nullptr)
		throw std::bad_alloc();
		
	//Init
	for(int x = 0; x < iSize; x++)
		for(int y = 0; y < iSize; y++)
			pBoard[INDEX(x,y)] = 0;
}

Board::~Board(){
	if(this->pBoard != nullptr)
		delete[] this->pBoard;
	this->pBoard = nullptr;
}

void Board::solve(){
	solveQueen(0);
}

void Board::print() {
	std::cout << "+";
	for(int j = 0; j < iSize; j++){
		std::cout << "---+";
	}
	std::cout << std::endl;
	for(int i = 0; i < iSize; i++){
		std::cout << "|";
		for(int j = 0; j < iSize; j++){
			std::cout << " " << (pBoard[INDEX(j,i)] ? pBoard[INDEX(j,i)] : ' ') << " |";
		}
		std::cout << std::endl << "+";
		for(int j = 0; j < iSize; j++){
			std::cout << "---+";
		}
		std::cout << std::endl;
	}
}
