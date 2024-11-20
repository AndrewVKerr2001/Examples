#include <iostream>
#include "QueenSolver.hpp"

int main(/*int argc, const char* argv[]*/)
{
	Board board(4);
	board.solve();
	board.print();
	return 0;
}
