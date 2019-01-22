#pragma once

#include <string>
using namespace std;

namespace Game
{
	class Constants
	{
	public:
		// main assets directory path
		static const char* ASSETS_DIR;

		// texture enum, handle all texture id-s, for adding new resource in game add enum item here before TEXTURE_MAX
		enum Texture {
			TEXTURE_BACKGROUND,
			TEXTURE_BACKGROUND_HOVER,
			TEXTURE_BLUE,				// 2
			TEXTURE_GREEN,				// 3
			TEXTURE_PURPLE,				// 4
			TEXTURE_RED,				// 5
			TEXTURE_YELLOW,				// 6
			TEXTURE_DIAMOND_SELECTION,
			TEXTURE_RESTART,
			TEXTURE_MENU_BC,
			TEXTURE_MENU_TITLE,
			TEXTURE_START_GAME,
			TEXTURE_ABOUT_GAME,
			TEXTURE_QUIT_GAME,
			TEXTURE_GO_BACK,
			TEXTURE_MAX
		};

		static const int GAME_TABLE_DIMENSION;

		static const float GAME_SCORE_POS_X;

		static const float GAME_SCORE_POS_Y;

		static const int MINIMUM_DIAMONDS_TO_DELETE;

		static const float SWAP_SPEED;

		static const float DIAMOND_ROTATION_SPEED;

		static const float FALLING_SPEED;

		static const int DIAMOND_SCORE_MULTIPLIER;
	
		static const float TOTAL_LEVEL_TIME;

		static const float GAME_START_CDN_TIMER;

		static const float GAME_START_CDN_TIMER_X;

		static const float GAME_START_CDN_TIMER_Y;
		
		static const float RESTART_TEXT_POS_X;
		
		static const float RESTART_TEXT_POS_Y;

		static const float GAME_ENDED_TITLE_X;

		static const float GAME_ENDED_TITLE_Y;

		static const float FINAL_SCORE_X;

		static const float FINAL_SCORE_Y;

		static const int MINIMUM_SWIPE_DELTA;

		static const int GO_BUTTON_POSX;

		static const int GO_BUTTON_POSY;
		
		static const float MINIMIM_SIZE_TO_DELETE_DIAMOND;

		static const int DIAMOND_DELETE_SPEED;

		static const int MENU_TITLE_POSX;

		static const int MENU_TITLE_POSY;

		static const int MENU_START_GAME_POSX;

		static const int MENU_START_GAME_POSY;

		static const int MENU_ABOUT_GAME_POSX;

		static const int MENU_ABOUT_GAME_POSY;

		static const int MENU_QUIT_GAME_POSX;

		static const int MENU_QUIT_GAME_POSY;
	};
}