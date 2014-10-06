#pragma once
#include "PuzzDraOperator.h"
#include "PuzzDraRoute.h"

namespace tnkt37
{
	namespace PuzzDraSolver
	{
		extern void inputBoard(BOARD&);
		extern BOARD inputBoard();
		extern string toStringBoard(BOARD&);
		extern string toStringRoute(Route);
		extern void debug(BOARD& board);
		extern void debug(Route route);

		extern void firstSearchTest();
		extern vector<vector<int>> generateRandomBoard();
		extern void countReachTest(BOARD&);

		extern int fillBoardRandom(BOARD& board);
	}
}