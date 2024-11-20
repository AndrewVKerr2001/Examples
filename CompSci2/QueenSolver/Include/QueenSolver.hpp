#ifndef _QUEENSOLVER_HPP_
#define _QUEENSOLVER_HPP_

class Board {
private:
	int iSize;
	char* pBoard;
	
public:
	Board(int iSize);
	~Board();
	
protected:
	bool canPlace(int row, int col);
	bool solveQueen(int fromCol);
	
public:
	void solve();
	void print();
};

#endif
