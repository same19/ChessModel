#pragma once
#include <string>
#include <iostream>
#include <vector>
namespace chess {
enum Color { WHITE=0, BLACK=1, NO_COLOR=-1 };
inline Color operator!(Color const & a) {
	if (a == WHITE) {
		return BLACK;
	} else if (a == BLACK) {
		return WHITE;
	} else {
		return NO_COLOR;
	}
}

inline std::ostream& operator<<(std::ostream & stream, const Color & color) {
	std::string s;
	switch (int(color)) {
		case BLACK:
			s = "BLACK";
			break;
		case WHITE:
			s = "WHITE";
			break;
		case NO_COLOR:
			s = "";
			break;
		default:
			s = "";
	}
	return stream << s;
		
}

const int w = 8;
const int h = 8;

struct Square {
	int r; //0 to 7
	int c; //0 to 7 -> A to H
};

bool contains(Square* arr, int len, Square& item);

inline bool operator==(Square const & a, Square const & b) {
	return a.r == b.r && a.c == b.c;
}

inline bool operator!=(Square const & a, Square const & b) {
	return a.r != b.r || a.c != b.c;
}

inline std::ostream& operator<<(std::ostream & s, Square const & v) {
	return s<<"("+std::to_string(v.r)<<", "<<std::to_string(v.c)<<")";
}

struct Piece {
	static int ID;
	int id;
	Color color;
	Square place;
	std::string str;
	bool moved;
	int value;

	Piece() {
		id = 0;
		str = "_";
		moved = false;
		value = 0;
	}

	virtual bool legalMove(Square& s);
	virtual std::string toString();
	virtual int numLegalMoves();
	virtual Square* legalMoves();
	static std::string toString(Piece p);
	static Piece* convert(Piece* p) {
		return p;
	}
	void move(Square s);
	bool hasMoved();

	virtual bool null();
};

struct Pawn : public Piece {
	static int ID;
	Pawn(Color c, Square p) {
		id = ID;
		color=c;
		place=p;
		str = !c ? "\u265F" : "\u2659";
		value = 1;
	}

	bool hasMoved() {
		if ((place.r == 1 && color == WHITE) || (place.r == 6 && color == BLACK)) {
			return false;
		} else {
			return true;
		}
	}

	int numLegalMoves();
	Square* legalMoves();

	static Pawn* convert(Piece* p, bool force = false) {
		if (p->id != ID && !force) {
			return NULL;
		}
		Pawn *x = new Pawn(p->color, p->place);
		if (force) {
			x->id = ID;
		}
		return x;
	}
};
struct Knight : public Piece {
	static int ID;
	Knight(Color c, Square p) {
		id = ID;
		color=c;
		place=p;
		str = !c ? "\u265E" : "\u2658";
		value = 3;
	}
	int numLegalMoves();
	Square* legalMoves();
	static Knight* convert(Piece* p, bool force = false) {
		if (p->id != ID && !force) {
			return NULL;
		}
		Knight *x = new Knight(p->color, p->place);
		if (force) {
			x->id = ID;
		}
		return x;
	}
};
struct Bishop : public Piece {
	static int ID;
	Bishop(Color c, Square p) {
		id = ID;
		color=c;
		place=p;
		str = !c ? "\u265D" : "\u2657";
		value = 3;
	}
	int numLegalMoves();
	Square* legalMoves();
	static Bishop* convert(Piece* p, bool force = false) {
		if (p->id != ID && !force) {
			return NULL;
		}
		Bishop *x = new Bishop(p->color, p->place);
		if (force) {
			x->id = ID;
		}
		return x;
	}
};
struct Rook : public Piece {
	static int ID;
	Rook(Color c, Square p) {
		id = ID;
		color=c;
		place=p;
		str = !c ? "\u265C" : "\u2656";
		value = 5;
	}
	int numLegalMoves();
	Square* legalMoves();
	static Rook* convert(Piece* p, bool force = false) {
		if (p->id != ID && !force) {
			return NULL;
		}
		Rook *x = new Rook(p->color, p->place);
		if (force) {
			x->id = ID;
		}
		return x;
	}
};
struct Queen : public Piece {
	static int ID;
	Queen(Color c, Square p) {
		id = ID;
		color=c;
		place=p;
		str = !c ? "\u265B" : "\u2655";
		value = 9;
	}
	int numLegalMoves();
	Square* legalMoves();
	static Queen* convert(Piece* p, bool force = false) {
		if (p->id != ID && !force) {
			return NULL;
		}
		Queen *x = new Queen(p->color, p->place);
		if (force) {
			x->id = ID;
		}
		return x;
	}
};
struct King : public Piece {
	static int ID;
	King(Color c, Square p) {
		id = 6;
		color=c;
		place=p;
		str = !c ? "\u265A" : "\u2654";
		value = 0;
	}
	int numLegalMoves();
	Square* legalMoves();
	static King* convert(Piece* p, bool force = false) {
		if (p->id != ID && !force) {
			return NULL;
		}
		King *x = new King(p->color, p->place);
		if (force) {
			x->id = ID;
		}
		return x;
	}
};
class ChessState {
private:
	Piece * board[h][w]; //h rows from 0 to 7, w colums from A to H
	std::vector<Piece*> captured;
	int numMoves;
	Square firstPawnDoubleMoveSquare;
public:
	Color player;
	Piece *& square(Square s) {return board[s.c][s.r];}
	Piece *& square(int r, int c) {return board[c][r];}
	void startBoard();
	ChessState() {
		startBoard();
		player = WHITE;
		numMoves = 0;
	}
	void loadMovePiece(Square from, Square to);
	void confirmMove(Square from, Square to);
	void cancelMove(Square from, Square to);
	Square firstPawnDoubleMove();
	void forceCapture(Square to);
	void forcePlace(Piece * p);
	Square kingSquare(Color color);
};
}