#include "chess.h"
#include <numeric>

using namespace chess;
using namespace std;

// #define DEBUG
// #define DEBUG_MOVES
string HARD_CODED_MOVES[] = {"E2E4", "E7E5", "D1H5", "B8C6", "H5E5", "D8E7", "F2F4", "E7H4", ""};
//make sure to include an empty string at the end to stop the program

ostream debug(new NullBuffer());

#define out cout
#define error cerr
#ifdef DEBUG
	#define log cout
#else
	#define log debug
#endif

Move NULL_MOVE = {{-1,-1}, {-1,-1}};

ChessState cs;
Display display;
ChessPlayer * players[2];

void chess::setup() {
	cs = ChessState();
	display = Display();
	players[BLACK] = new ChessEngine();
	players[WHITE] = new UserInputChessPlayer();
}
void chess::parseMove(string move, int* fromTo) {
	fromTo[0] = move[1]-'1';
	fromTo[1] = move[0]-'A';
	fromTo[2] = move[3]-'1';
	fromTo[3] = move[2]-'A';
}
bool chess::inBounds(Square s) {
	if (s.r >= h || s.r < 0) {
		return false;
	} else if (s.c >= w || s.c < 0) {
		return false;
	} else {
		return true;
	}
}
int chess::status(ChessState cs, Color color) {
	Status status = CLEAR;
	if (inCheck(color)) {
		status = Status(status | CHECK);
	}
	for (int r = 0;r<h;r++) {
		for (int c = 0;c<w;c++) {
			Square from = {r,c};
			Piece * p = cs.square(from);
			for (int move = 0;move<p->numLegalMoves();move++) {
				if (isValidMove(from, p->legalMoves()[move], color) == NO_EXCEPTION) {
					return status;
				}
			}
		}
	}
	return status | GAME_OVER;
}
int chess::status(Color color) {
	return status(cs, color);
}
bool chess::inCheck(Color color, Square k) {
	for (int r = 0;r<h;r++) {
		for (int c = 0;c<w;c++) {
			Square from = {r,c};
			int result = isValidMove(from,k,!color);
			if (from != k && result==NO_EXCEPTION) {
				return true;
			}
		}
	}
	return false;
}
bool chess::inCheck(Color color) {
	return inCheck(color, cs.kingSquare(color));
}
void chess::promotePawn(Square s) {
	out << "Promote your pawn from the following options. Press the return key for a Queen." << endl;
	out << "0: Queen" << endl;
	out << "1: Rook" << endl;
	out << "2: Bishop" << endl;
	out << "3: Knight" << endl;
	string in;
	do {
		getline(cin, in);
		if (in == "") {
			cs.forcePlace(Queen::convert(cs.square(s), true));
			return;
		}
		if (in == "0") {
			cs.forcePlace(Queen::convert(cs.square(s), true));
			return;
		}
		if (in == "1") {
			cs.forcePlace(Rook::convert(cs.square(s), true));
			return;
		}
		if (in == "2") {
			cs.forcePlace(Bishop::convert(cs.square(s), true));
			return;
		}
		if (in == "3") {
			cs.forcePlace(Knight::convert(cs.square(s), true));
			return;
		}
	} while (true);
}
int chess::isValidMove(Square from, Square to, Color currentPlayer = cs.player) {
	//inside the board
	if (!inBounds(from) || !inBounds(to)) {
		return BOARD_OUT_OF_BOUNDS_EXCEPTION;
	}

	if (cs.square(from) == NULL || cs.square(from)->null()) {
		return NULL_PIECE_EXCEPTION;
	}
	
	//rules of the pieces
	if (!cs.square(from)->legalMove(to)) {
		return ILLEGAL_MOVE_EXCEPTION;
	}
	if (cs.square(from)->color != currentPlayer) {
		return WRONG_TURN_EXCEPTION;
	}
	//special pawn rules
	//by Pawn's isValidMove, must be 1-2 forward or 1 diagonal
		//also can only be 2 forward when it is on its own 2/7 row
	if (Pawn* p = Pawn::convert(cs.square(from))) {//if its a pawn
		if (to.c == from.c) {//straight forward, not capturing
			if (!cs.square(to)->null()) { //pawns cannot take forward or go through a piece
				return ILLEGAL_CAPTURE_EXCEPTION;
			}
			if (abs(to.r - from.r) == 2 && !cs.square({(from.r + to.r)/2,from.c})->null()) {
				return ILLEGAL_JUMP_EXCEPTION;
			}
		} else {//pawn capturing
			Square fpdm = cs.firstPawnDoubleMove();
			if (fpdm.r == from.r && fpdm.c == to.c) {//en passant
				cs.forceCapture(fpdm);
			} else if (cs.square(to)->null()) { //nothing to capture
				return ILLEGAL_MOVE_EXCEPTION;
			} else if (cs.square(to)->color == p->color) {//capturing a friendly piece
				return FRIENDLY_CAPTURE_EXCEPTION;
			}
		}
	} else if (King* k = King::convert(cs.square(from))) { //castling
		if (abs(to.c - from.c) == 2) {
			//cannot castle if the king has moved
			if (k->hasMoved()) {
				return ILLEGAL_CASTLE_EXCEPTION;
			}
			Piece * rook;
			if (to.c > from.c) { //castling to the "right"
				rook = cs.square(k->place.r, w-1);
			} else { //castling to the "left"
				rook = cs.square(k->place.r, 0);
			}
			//cannot castle if the rook has moved
			if (rook->id != Rook::ID || rook->color != k->color || rook->hasMoved()) {
				return ILLEGAL_CASTLE_EXCEPTION;
			}
			Square inBetweenSquare = {from.r, (from.c + to.c)/2};
			if (!cs.square(to)->null() || !cs.square(inBetweenSquare)->null()) {
				return ILLEGAL_CASTLE_EXCEPTION;
			}
			if (inCheck(cs.player, from) || inCheck(cs.player, inBetweenSquare) || inCheck(cs.player, to)) {
				return KING_CHECK_EXCEPTION;
			}
			//castle
			cs.loadMovePiece(rook->place, inBetweenSquare);
			cs.loadMovePiece(from, to);
			if (inCheck(cs.player)) {
				cs.cancelMove(from, to);
				cs.cancelMove(rook->place, inBetweenSquare);
				return KING_CHECK_EXCEPTION;
			}
			
			cs.cancelMove(from, to);
			return NO_EXCEPTION;
			
		}
	}
	//normal dynamics: cannot move through pieces or friendly capture
	int divisor = gcd(to.r-from.r, to.c-from.c);
	if (divisor == 0) {
		divisor = max(to.r-from.r, to.c-from.c);
	}
	Square unitMove = {(to.r-from.r)/divisor, (to.c-from.c)/divisor};
	Square squareToCheck = from;
	squareToCheck.r += unitMove.r;
	squareToCheck.c += unitMove.c; //start at the place after from
	while (squareToCheck != to) { //end at the place before to
		if (!cs.square(squareToCheck)->null()) {
			return ILLEGAL_JUMP_EXCEPTION;
		}
		squareToCheck.r += unitMove.r;
		squareToCheck.c += unitMove.c;
	}
	if (!cs.square(to)->null()&&cs.square(to)->color==cs.square(from)->color) {
		//friendly capturing
		return FRIENDLY_CAPTURE_EXCEPTION;
	}

	//king in check?
	cs.loadMovePiece(from,to);
	//make sure the king is not in check
	if (inCheck(!cs.player)) {
		cs.cancelMove(from,to);
		return KING_CHECK_EXCEPTION;
	}
	cs.cancelMove(from,to);
	return NO_EXCEPTION;
}
int chess::turn(string move) {
	int fromTo[4];
	//valid input
	if (move.length() == 4) {
		parseMove(move, fromTo);
	} else {
		return INVALID_INPUT_EXCEPTION;
	}
	Square from = {fromTo[0],fromTo[1]};
	Square to = {fromTo[2],fromTo[3]};
	int result = isValidMove(from, to);
	if (result != NO_EXCEPTION) {
		return result;
	}
	cs.loadMovePiece(from, to);
	cs.confirmMove(from, to);

	if (Pawn::convert(cs.square(to)) && to.r == 7*(1-cs.player)) { //if WHITE, to.r == 7, if BLACK, to.r == 0
		promotePawn(to);
	}
	return NO_EXCEPTION;
}

void chess::gameOver(int status) {
	if (status == CHECKMATE) {
		out << "Checkmate: " << !cs.player << " WINS!" << endl;
	} else if (status == STALEMATE) {
		out << "Draw: Stalemate" << endl;
	} else {
		out << "returning gameover " <<status << " " << GAME_OVER << endl;
	}
}

void chess::start() {
	out << "Press the return key to start. ";
	log << "DEBUG MODE ON. ";
	cin.ignore();
	setup();
	out << display.printBoard(cs, cs.player);
	string move = players[cs.player]->getMove(cs);
	int result;
	while (move != "") {
		result = turn(move);
		if (result == NO_EXCEPTION) {
			out << display.printBoard(cs);
			int stat = status(cs.player);
			if (stat == CHECKMATE || stat == STALEMATE) {
				gameOver(stat);
				break;
			}
		} else {
			out << display.clear();
			error << "ERROR. " << ErrorCode(result) << std::endl;
			out << endl;
			out << display.printBoard(cs, false);
		}
		move = players[cs.player]->getMove(cs);
	}
}

string chess::UserInputChessPlayer::getMove(ChessState & cs) {
	string s;
	#ifdef DEBUG_MOVES
		static int i = 0;
		s = HARD_CODED_MOVES[i];
		i++;
	#else
		getline(cin, s);
	#endif
	return s;
}
float chess::evaluate(ChessState & cs) {
	// log << "eval base start" << endl;
	if (status(cs, WHITE)==CHECKMATE) {
		return -99.9;
	}
	else if (status(cs, BLACK)==CHECKMATE) {
		return 99.9;
	}
	else if (status(cs, WHITE)==STALEMATE || status(cs, BLACK)==STALEMATE) {
		return 0;
	}
	float eval = 0;
	for (int r = 0; r<h;r++) {
		for (int c = 0;c<w;c++) {
			if (cs.square(r,c)->color == BLACK) {
				eval -= cs.square(r,c)->value;
			} else {
				eval += cs.square(r,c)->value;
			}
		}
	}
	// log << "eval base end" << endl;
	if (eval != 0) {
		log << eval << endl;
	}
	if (eval == 99.9 || eval == -99.9) {
		log << "CHECKMATE" << endl;
	}
	return eval;
}
float chess::evaluate(ChessState & cs, int depth) {
	// log << "evaluate function" << depth << endl;
	if (depth <= 0) {
		return evaluate(cs);
	}
	float maxEval;
	if (cs.player == BLACK) {
		maxEval = 100;
	} else if (cs.player == WHITE) {
		maxEval = -100;
	}
	for (int r = 0;r<h;r++) {
		for (int c = 0;c<w;c++) {
			Square from = {r,c};
			Piece * p = cs.square(from);
			for (int move = 0;move<p->numLegalMoves();move++) {
				Square sq = p->legalMoves()[move];
				if (isValidMove(from, sq, cs.player) == NO_EXCEPTION) {
					cs.loadMovePiece(from, sq);
					float testEval = evaluate(cs, depth-1);
					// log << from << " " << sq << " " << testEval << endl;
					if (cs.player == BLACK && testEval > maxEval) {
						maxEval = testEval;
					} else if (cs.player == WHITE && testEval < maxEval) {
						maxEval = testEval;
					}
					cs.cancelMove(from, sq);
				}
			}
		}
	}
	if (maxEval == 100 || maxEval == -100) {
		log << "CHECKMATE" << endl;
	}
	log << "end Eval" << endl;
	return maxEval;
}
Move chess::solve(ChessState & cs, int depth) {
	log << "solve function" << endl;
	float maxEval;
	Move bestMove = NULL_MOVE;
	if (cs.player == BLACK) {
		maxEval = 100;
	} else if (cs.player == WHITE) {
		maxEval = -100;
	}
	log << "here" << endl;
	for (int r = 0;r<h;r++) {
		for (int c = 0;c<w;c++) {
			Square from = {r,c};
			Piece * p = cs.square(from);
			for (int move = 0;move<p->numLegalMoves();move++) {
				Square sq = p->legalMoves()[move];
				if (isValidMove(from, sq, cs.player) == NO_EXCEPTION) {
					cs.loadMovePiece(from, sq);
					float testEval = evaluate(cs, depth-1);
					// log << from << " " << sq << " " << testEval << endl;
					// log << "after testEval 1" << endl;
					if (cs.player == BLACK && testEval > maxEval) {
						maxEval = testEval;
						bestMove = {from, sq};
					} else if (cs.player == WHITE && testEval < maxEval) {
						maxEval = testEval;
						bestMove = {from, sq};
					}
					// log << "after testEval 2" << endl;
					// log << display.printBoard(cs, false) << endl;
					cs.cancelMove(from, sq);
					// log << display.printBoard(cs, false) << endl;
					// log << "after testEval 3" << endl;
				}
			}
		}
	}
	return bestMove;
}