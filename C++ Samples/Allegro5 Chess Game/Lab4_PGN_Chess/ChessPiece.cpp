#include "ChessPiece.hpp"

// Constructor
ChessPiece::ChessPiece(int y, int x, const char * prefix, std::string piece)
{
	row = y;
	column = x;
	std::string sprite_path = "Pieces/" + std::string(prefix) + piece + ".png";
	sprite = al_load_bitmap(sprite_path.c_str());
}

// Draw chess piece sprite
void ChessPiece::render()
{
	al_draw_bitmap(sprite, (column - 1) * 100, (row - 1) * 100, 0);
}

// Clean-up
void ChessPiece::destroy()
{
	al_destroy_bitmap(sprite);
}