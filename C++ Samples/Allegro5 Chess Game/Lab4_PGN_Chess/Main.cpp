/// Dalton Fox ~ Lab 4: Chess

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

// Allegro Dependencies
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>

// Project Dependencies
#include "PGN.hpp"
#include "ChessPiece.hpp"

int main(int argc, char * argv[])
{
	// Parse argument
	std::string input_path(argv[1]);
	PGNGame pgn = PGNGame(input_path.c_str());
		
	// Initialization
	al_init(); al_init_image_addon(); al_init_primitives_addon();

	// Create a display
	ALLEGRO_DISPLAY * display = al_create_display(800, 800);
	std::string title = "Lab 4: Chess Replay";
	al_set_window_title(display, title.c_str());

	// Create an event queue
	ALLEGRO_EVENT_QUEUE * queue = al_create_event_queue();
	al_install_keyboard();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));

	// Initial variables
	int buffer = 2;

	// Make some chess pieces
	std::vector<ChessPiece> pieces;
	int row; const char * prefix;
	for (int i = 0; i < 2; i++)
	{
		// Choice for black or white
		if (i == 0) prefix = "b"; else prefix = "w";

		// Choice for top or bottom row
		if (i == 0) row = 1; else row = 8;
		pieces.push_back(ChessPiece(row, 1, prefix, std::string("Rook")));
		pieces.push_back(ChessPiece(row, 2, prefix, std::string("Knight")));
		pieces.push_back(ChessPiece(row, 3, prefix, std::string("Bishop")));
		pieces.push_back(ChessPiece(row, 4, prefix, std::string("King")));
		pieces.push_back(ChessPiece(row, 5, prefix, std::string("Queen")));
		pieces.push_back(ChessPiece(row, 6, prefix, std::string("Bishop")));
		pieces.push_back(ChessPiece(row, 7, prefix, std::string("Knight")));
		pieces.push_back(ChessPiece(row, 8, prefix, std::string("Rook")));
		// Choice for inner top or bottom row
		if (i == 0) row = 2; else row = 7;
		pieces.push_back(ChessPiece(row, 1, prefix, std::string("Pawn")));
		pieces.push_back(ChessPiece(row, 2, prefix, std::string("Pawn")));
		pieces.push_back(ChessPiece(row, 3, prefix, std::string("Pawn")));
		pieces.push_back(ChessPiece(row, 4, prefix, std::string("Pawn")));
		pieces.push_back(ChessPiece(row, 5, prefix, std::string("Pawn")));
		pieces.push_back(ChessPiece(row, 6, prefix, std::string("Pawn")));
		pieces.push_back(ChessPiece(row, 7, prefix, std::string("Pawn")));
		pieces.push_back(ChessPiece(row, 8, prefix, std::string("Pawn")));
	}

	// Start main loop
	bool running = true;
	while (running) 
	{
		// Clear screen
		al_clear_to_color(al_map_rgb(255, 255, 255));

		// Draw grid
		for (int i = 0, k = 1; i < 8; i++, k++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (((k % 2 == 1) && (j % 2 == 1)) || ((k % 2 == 0) && (j % 2 == 0)))
					al_draw_filled_rectangle(i * 100, j * 100, (i + 1) * 100, (j + 1) * 100, al_map_rgb(200, 200, 200));
				else
					al_draw_filled_rectangle(i * 100, j * 100, (i + 1) * 100, (j + 1) * 100, al_map_rgb(55, 55, 55));
			}
		}

		// Draw chess pieces
		for (ChessPiece piece : pieces)
			piece.render();

		// Update screen
		al_flip_display();

		// Update events
		ALLEGRO_EVENT event;
		if (!al_is_event_queue_empty(queue)) 
		{
			al_wait_for_event(queue, &event);
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				running = false;
			}
			else if (event.type == ALLEGRO_EVENT_KEY_UP)
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
				{
					if (pgn.current_move < pgn.moves.size() - 1)
					{
						std::string move = pgn.getMove();
						int atrow = (move.at(0) - '0');
						int atcolumn = 9 - (move.at(1) - '0');
						int torow = (move.at(3) - '0');
						int tocolumn = 9 - (move.at(4) - '0');

						// Delete a piece if its in the move
						int i = 0;
						for (ChessPiece piece : pieces)
						{
							if (piece.row == tocolumn && piece.column == torow)
							{
								pieces.erase(pieces.begin() + i);
							}
							i++;
						}

						// Move the piece in question
						int j = 0;
						for (ChessPiece piece : pieces)
						{
							if (piece.column == atrow && piece.row == atcolumn)
							{
								pieces.at(j).column = torow;
								pieces.at(j).row = tocolumn;
							}
							j++;
						}
					}
				}
			}
		}
	}

	// Finalize and quit
	al_destroy_display(display);
	al_uninstall_keyboard();
	for (ChessPiece piece : pieces)
		piece.destroy();
	return 0;
}
