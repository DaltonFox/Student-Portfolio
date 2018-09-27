#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class PGNGame
{
public:
	std::vector<std::string> moves;
	int current_move = 0;

	PGNGame();
	PGNGame(const char * filename);
	std::string getMove();
};