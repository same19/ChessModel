#pragma once
#include <iostream>
#include <string>
#include "chessHelper.h"

namespace chess {
class Display {
private:

public:
	std::string clear();
	std::string printBoard(ChessState cs, bool clearScreen = true);
};
}
