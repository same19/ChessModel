#pragma once
#include "chessHelper.h"
#include "chessDisplay.h"

namespace chess {
	class NullBuffer : public std::streambuf
	{
	public:
	  int overflow(int c) { return c; }
	};
	enum ErrorCode {
		NO_EXCEPTION = 0,
		INVALID_INPUT_EXCEPTION = -1,
		ILLEGAL_MOVE_EXCEPTION = -2, //by the rules of the piece
		BOARD_OUT_OF_BOUNDS_EXCEPTION = -3,
		KING_CHECK_EXCEPTION = -4,
		FRIENDLY_CAPTURE_EXCEPTION = -5,
		ILLEGAL_CAPTURE_EXCEPTION = -6, //pawns cannot take directly forward
		ILLEGAL_JUMP_EXCEPTION = -7,
		WRONG_TURN_EXCEPTION = -8,
		AMBIGUOUS_INPUT_EXCEPTION = -9,
		NULL_PIECE_EXCEPTION = -10,
		ILLEGAL_CASTLE_EXCEPTION = -11
	};
	enum Status {
		CLEAR = 0, //0 = 00
		CHECK = 1 << 0, //1 = 01
		GAME_OVER = 1 << 1, //2 = 10
		CHECKMATE = CHECK | GAME_OVER, //3 = 11
		STALEMATE = ~CHECK | GAME_OVER //2 (== pure GAME_OVER) = 10
	};
	inline std::ostream& operator<<(std::ostream & stream, const ErrorCode & code) {
		std::string s;
		switch (int(code)) {
			case NO_EXCEPTION:
				s = "NO_EXCEPTION";
				break;
			case INVALID_INPUT_EXCEPTION:
				s = "INVALID_INPUT_EXCEPTION";
				break;
			case ILLEGAL_MOVE_EXCEPTION:
				s = "ILLEGAL_MOVE_EXCEPTION";
				break;
			case BOARD_OUT_OF_BOUNDS_EXCEPTION:
				s = "BOARD_OUT_OF_BOUNDS_EXCEPTION";
				break;
			case KING_CHECK_EXCEPTION:
				s = "KING_CHECK_EXCEPTION";
				break;
			case FRIENDLY_CAPTURE_EXCEPTION:
				s = "FRIENDLY_CAPTURE_EXCEPTION";
				break;
			case ILLEGAL_CAPTURE_EXCEPTION:
				s = "ILLEGAL_CAPTURE_EXCEPTION";
				break;
			case ILLEGAL_JUMP_EXCEPTION:
				s = "ILLEGAL_JUMP_EXCEPTION";
				break;
			case WRONG_TURN_EXCEPTION:
				s = "WRONG_TURN_EXCEPTION";
				break;
			case AMBIGUOUS_INPUT_EXCEPTION:
				s = "AMBIGUOUS_INPUT_EXCEPTION";
				break;
			case NULL_PIECE_EXCEPTION:
				s = "NULL_PIECE_EXCEPTION";
				break;
			case ILLEGAL_CASTLE_EXCEPTION:
				s = "ILLEGAL_CASTLE_EXCEPTION";
				break;
			default:
				s = "UNKNOWN_EXCEPTION";
		}
		return stream << "Error " << int(code) << ": " << s;
			
	}

	void setup();
	void parseMove(std::string move, int* fromTo);
	bool inBounds(Square s);
	int status(ChessState cs, Color color);
	int status(Color color);
	bool inCheck(Color color, Square k);
	bool inCheck(Color color);
	void promotePawn(Square s);
	int isValidMove(Square from, Square to, Color currentPlayer);
	int turn(std::string move);
	void getInput(std::string & s);
	void gameOver(int status);
	void start();

	class ChessPlayer {
	public:
		virtual std::string getMove(ChessState & cs) {
			return "";
		}
	};
	class UserInputChessPlayer : public ChessPlayer {
	public:
		std::string getMove(ChessState & cs);
	};
//engine stuff
	struct Move {
		Square from;
		Square to;
	};
	
	float evaluate(ChessState & cs);
	float evaluate(ChessState & cs, int depth); //returns + if white is winning, - if black is winning, 0 if drawn
	//if depth = 0, do a shallow evaluation (just evaluate without recursing)
	Move solve(ChessState & cs, int depth); //returns the best move
	//goes through all moves and recurses until depth, then evaluates all positions at depth 0, then uses minimax and alphabeta pruning to find best move
	class ChessEngine : public ChessPlayer {
		int depth = 2;
	public:
		std::string getMove(ChessState & cs) {
			Move m = solve(cs, depth); 
			std::string s = "";
			s += char(int('A') + m.from.c);
			s += char(int('1') + m.from.r);
			s += char(int('A') + m.to.c);
			s += char(int('1') + m.to.r);
			std::cout << s << std::endl;
			return s;
		}
	};
}