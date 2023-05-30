#include "chessDisplay.h"
using namespace chess;

#define endl "\n"

std::string Display::clear() {
	std::string out;
	for (int i = 0;i<50;i++) {
		out += endl;
	}
	return out;
}
std::string Display::printBoard(ChessState cs, bool clearScreen) {
	Color turn = cs.player;
	std::string out;
	if (clearScreen) {
		out += clear();
	}
	out += endl;
	int r1;
	int c1;
	for (int r = 0;r < h;r++) {
		if (turn == WHITE) {
			r1 = h-r-1;
		} else {
			r1 = r;
		}
		//labels
		out += std::to_string(r1+1) + " ";
		//board
		for (int c = 0;c < w; c++) {
			if (turn == WHITE) {
				c1 = c;
			} else {
				c1 = w-c-1;
			}
			out += Piece::toString(*cs.square(r1,c1)) + " ";
		}
		out += endl;
	}
	//labels
	out += "  ";
	for (int c = 0;c < w; c++) {
		if (turn == WHITE) {
			c1 = c;
		} else {
			c1 = w-c-1;
		}
		out += std::string({char(c1 + 65)}) + " ";
	}
	out += endl;
	return out;
}