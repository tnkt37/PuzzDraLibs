#pragma once

#include "PuzzDraOperator.h"
#include "PuzzDraRoute.h"

namespace tnkt37
{
	namespace PuzzDraSolver
	{
		extern Route beamSearch(BOARD& board, int aheads, int length, int beamWeight, int startRow, int startColumn, int& outScore);
		extern Route doubleLimitedSearch(BOARD& board, int beamWeight, int limitWeight, int startRow, int startColumn, int& outScore);

		extern void beamSearchTest();
		extern void doubleLimitedSearchTest();
	}
}