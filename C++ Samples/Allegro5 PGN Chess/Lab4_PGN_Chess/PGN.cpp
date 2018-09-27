#include "PGN.hpp"

PGNGame::PGNGame(){};

PGNGame::PGNGame(const char * filename)
{
	std::ifstream file(filename);
	std::string replay((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = replay.find(std::string("\n"), prev)) != std::string::npos)
	{
		moves.push_back(replay.substr(prev, pos - prev));
		prev = pos + 1;
	}
	moves.push_back(replay.substr(prev));
}

std::string PGNGame::getMove()
{
	return moves.at(current_move++);
}