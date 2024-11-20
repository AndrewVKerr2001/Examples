#include <iostream>
#include <fstream>

char board[4][4];

#define UNDO_MOVE() board[row][col] = '.'

#define X_WIN 1
#define DRAW 0
#define O_WIN -1

bool didWin(char c) {
	for(int offset = 0; offset < 4; offset++){
		if(board[offset][0] == c && board[offset][1] == c && board[offset][2] == c  && board[offset][3] == c) return true;
		if(board[0][offset] == c && board[1][offset] == c && board[2][offset] == c  && board[3][offset] == c) return true;
	}
	if(board[0][0] == c && board[1][1] == c && board[2][2] == c && board[3][3] == c) return true;
	if(board[3][0] == c && board[2][1] == c && board[1][2] == c && board[0][3] == c) return true;
	return false;
}

int SolveBoardForOLoss();

int SolveBoardForXWin() {
	if(didWin('o')) return O_WIN;
	
	unsigned int nWins = 0;
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			if(board[row][col] == '.') {
				board[row][col] = 'x';
				int res = SolveBoardForOLoss();
				if(res == O_WIN){
					UNDO_MOVE();
					return O_WIN;
				}
				nWins += res;
				UNDO_MOVE();
			}
		}
	}
	
	return (nWins >= 2 ? X_WIN : DRAW);
}

int SolveBoardForOLoss() {
	if(didWin('x')) return X_WIN;
	
	unsigned int nWins = 0;
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			if(board[row][col] == '.') {
				board[row][col] = 'o';
				int res = SolveBoardForXWin();
				if(res == O_WIN){
					UNDO_MOVE();
					return O_WIN;
				}
				nWins += res;
				UNDO_MOVE();
			}
		}
	}
	
	return (nWins >= 2 ? X_WIN : DRAW);
}

void PrimeBoardForX() {
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			if(board[row][col] == '.') {
				board[row][col] = 'x';
				if(didWin('x')) {
					board[row][col] = '.';
					std::cout << "(" << row << ", " << col << ")" << std::endl;
					return;
				}
				if(SolveBoardForOLoss() == X_WIN) {
					board[row][col] = '.';
					std::cout << "(" << row << ", " << col << ")" << std::endl;
					return;
				}
				board[row][col] = '.';
			}
		}
	}
	std::cout << "X cannot make a move that would enable a forced-win senerio." << std::endl;
}

int main(int argc, const char *argv[]){
	std::string filename;
	std::cout << "Please input filename?: ";
	std::cin >> filename;
	
	std::ifstream file {filename};
	if(!file.is_open()){
		std::cerr << "Filename " << filename << " does not exist or could not be found!" << std::endl;
		return 0;
	}
	
	char keyword;
	file >> keyword;
	while(file && keyword != '!') {
		if(keyword == '?'){
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					file >> board[i][j];
					std::cout << board[i][j];
				}
				std::cout << std::endl;
			}
			PrimeBoardForX();
			std::cout << std::endl;
		}
		else {
			std::cout << "Unknown keyword identifier?: " << keyword << std::endl;
		}
		file >> keyword;
	}
	
	return 0;
}
