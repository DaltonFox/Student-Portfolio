#pragma once

#include <string>

// Allegro Dependencies
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>

class ChessPiece
{
public:
	int row;
	int column;
	ALLEGRO_BITMAP * sprite;

	// Constructor
	ChessPiece(int y, int x, const char * prefix, std::string piece);

	// Draw chess piece sprite
	void render();

	// Clean-up
	void destroy();
};