#include "chessHelper.h"
using namespace chess;
bool chess::contains(Square* arr, int len, Square& item) {
	for (int i = 0;i<len;i++) {
		if (arr[i] == item) {
			return true;
		}
	}
	return false;
}
bool Piece::legalMove(Square& s) {
	if (numLegalMoves() == 0) {
		return false;
	} else {
		Square* moves = legalMoves();
		return contains(moves, numLegalMoves(), s);
	}
}
std::string Piece::toString() {
	return str;
}
int Piece::numLegalMoves() {
	return 0;
}
Square* Piece::legalMoves() { return NULL; }
int null() { return true; }

std::string Piece::toString(Piece p) {
	return p.toString();
}
void Piece::move(Square s) {
	place = s;
	moved = true;
}
bool Piece::hasMoved() {
	return moved;
}
bool Piece::null() {
	return id == 0;
}

int Piece::ID = 0;
int Pawn::ID = 1;
int Knight::ID = 2;
int Bishop::ID = 3;
int Rook::ID = 4;
int Queen::ID = 5;
int King::ID = 6;

int Pawn::numLegalMoves() {
	if (hasMoved()) {
		return 3;
	} else {
		return 4;
	}
}

Square* Pawn::legalMoves() {
	int r = place.r;
	int c = place.c;
	int addr = 1-2 * color;
	Square *moves;
	if (hasMoved()) {
		moves = new Square[3];
		moves[0] = {r+addr,c-1};
		moves[1] = {r+addr,c};
		moves[2] = {r+addr,c+1};
	} else {
		moves = new Square[4];
		moves[0] = {r+addr,c-1};
		moves[1] = {r+addr,c};
		moves[2] = {r+addr,c+1};
		moves[3] = {r+2*addr,c};
	}
	return moves;
}

int Knight::numLegalMoves() {
	return 8;
}
Square* Knight::legalMoves() {
	Square * moves = new Square[8];
	moves[0] = {place.r + 2, place.c + 1};
	moves[1] = {place.r - 2, place.c + 1};
	moves[2] = {place.r + 2, place.c - 1};
	moves[3] = {place.r - 2, place.c - 1};
	moves[4] = {place.r + 1, place.c + 2};
	moves[5] = {place.r - 1, place.c + 2};
	moves[6] = {place.r + 1, place.c - 2};
	moves[7] = {place.r - 1, place.c - 2};
	return moves;
}

int Bishop::numLegalMoves() {
	return 28;
}
Square* Bishop::legalMoves() {
	Square * moves = new Square[28];
	Square ways[4] = {{1,1},{1,-1},{-1,1},{-1,-1}};
	for (int way = 0;way<4;way++) {
		for (int i = 1;i<8;i++) {
			moves[way*7 + i-1] = {place.r + i*ways[way].r, place.c + i*ways[way].c};
		}
	}
	return moves;
}

int Rook::numLegalMoves() {
	return 28;
}
Square* Rook::legalMoves() {
	Square * moves = new Square[28];
	Square ways[4] = {{1,0},{-1,0},{0,1},{0,-1}};
	for (int way = 0;way<4;way++) {
		for (int i = 1;i<8;i++) {
			moves[way*7 + i-1] = {place.r + i*ways[way].r, place.c + i*ways[way].c};
		}
	}
	return moves;
}

int Queen::numLegalMoves() {
	return 56;
}
Square* Queen::legalMoves() {
	Square * moves = new Square[56];
	Square ways[8] = {{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{-1,0},{0,1},{0,-1}};
	for (int way = 0;way<8;way++) {
		for (int i = 1;i<8;i++) {
			moves[way*7 + i-1] = {place.r + i*ways[way].r, place.c + i*ways[way].c};
			// std::cout << moves[way*7+i-1];
		}
	}
	return moves;
}

int King::numLegalMoves() {
	return 10;
}
Square* King::legalMoves() {
	Square * moves = new Square[10];
	Square ways[10] = {{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{-1,0},{0,1},{0,-1},{0,2},{0,-2}};
	for (int way = 0;way<10;way++) {
		moves[way] = {place.r + ways[way].r, place.c + ways[way].c};
	}
	return moves;
}

void ChessState::startBoard() {
	square(0,0) = new Rook(WHITE,{0,0});
	square(0,1) = new Knight(WHITE,{0,1});
	square(0,2) = new Bishop(WHITE,{0,2});
	square(0,3) = new Queen(WHITE,{0,3});
	square(0,4) = new King(WHITE,{0,4});
	square(0,5) = new Bishop(WHITE,{0,5});
	square(0,6) = new Knight(WHITE,{0,6});
	square(0,7) = new Rook(WHITE,{0,7});

	square(1,0) = new Pawn(WHITE,{1,0});
	square(1,1) = new Pawn(WHITE,{1,1});
	square(1,2) = new Pawn(WHITE,{1,2});
	square(1,3) = new Pawn(WHITE,{1,3});
	square(1,4) = new Pawn(WHITE,{1,4});
	square(1,5) = new Pawn(WHITE,{1,5});
	square(1,6) = new Pawn(WHITE,{1,6});
	square(1,7) = new Pawn(WHITE,{1,7});
	
	square(7,0) = new Rook(BLACK,{7,0});
	square(7,1) = new Knight(BLACK,{7,1});
	square(7,2) = new Bishop(BLACK,{7,2});
	square(7,3) = new Queen(BLACK,{7,3});
	square(7,4) = new King(BLACK,{7,4});
	square(7,5) = new Bishop(BLACK,{7,5});
	square(7,6) = new Knight(BLACK,{7,6});
	square(7,7) = new Rook(BLACK,{7,7});

	square(6,0) = new Pawn(BLACK,{6,0});
	square(6,1) = new Pawn(BLACK,{6,1});
	square(6,2) = new Pawn(BLACK,{6,2});
	square(6,3) = new Pawn(BLACK,{6,3});
	square(6,4) = new Pawn(BLACK,{6,4});
	square(6,5) = new Pawn(BLACK,{6,5});
	square(6,6) = new Pawn(BLACK,{6,6});
	square(6,7) = new Pawn(BLACK,{6,7});
	
	for (int r = 2;r<6;r++) {
		for (int c = 0;c<8;c++) {
			square(r,c) = new Piece();
		}
	}
}

void ChessState::loadMovePiece(Square from, Square to) {
	captured.insert(captured.end(), square(to));
	square(from)->move(to);
	square(to) = square(from);
	square(from) = new Piece();
	square(from)->move(from);
	player = !player;
}
void ChessState::confirmMove(Square from, Square to) {
	firstPawnDoubleMoveSquare = Square();
	if (Pawn* p = Pawn::convert(square(to))) {
		if (std::abs(to.r-from.r) == 2) {
			// std::cout << "PAWN FIRST MOVE" << std::endl;
			firstPawnDoubleMoveSquare = to;
		}
	}
	numMoves++;
}
void ChessState::cancelMove(Square from, Square to) {
	// std::cout << "cancelling" << std::endl;
	square(from) = square(to);
	square(to) = captured.back();
	square(from)->move(from);
	captured.pop_back();
	player = !player;
}
Square ChessState::firstPawnDoubleMove() {
	return firstPawnDoubleMoveSquare;
}
void ChessState::forceCapture(Square to) {
	if (!square(to)->null()) {
		captured.insert(captured.end(), square(to));
	}
	square(to) = new Piece();
	square(to)->place = to;
}
void ChessState::forcePlace(Piece * p) { //use place of p as the square
	Square to = p->place;
	square(to) = p;
}
Square ChessState::kingSquare(Color color) {
	for (int r = 0;r<h;r++) {
		for (int c = 0;c<w;c++) {
			if (square(r,c)->id == King::ID && square(r,c)->color == color) {
				return {r,c};
			}
		}
	}
	return {-1,-1};
}