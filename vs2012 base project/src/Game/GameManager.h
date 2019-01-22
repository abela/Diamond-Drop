#pragma once

#include <king/Engine.h>
#include <king/Updater.h>
#include "Level.h"
#include "UiObject.h"
#include "InGameHud.h"
using namespace std;
using namespace King;

namespace Game
{
	class GameManager : public Updater
	{
	public:
		GameManager();
		~GameManager();

		// run game
		void Run();

	private:

		// level
		unique_ptr <Level> mCurrentLevel;

		unique_ptr <InGameHud> mpInGameHud;

		// game world images:
		unique_ptr <GameObject> mpLevelBackgroundObject;
		unique_ptr <GameObject> mpLevelBackgroundHoverObject;

		//
		bool mLevelStarted;

		bool mLevelEnded;

		bool mMouseButtonDown;

		float mTotalLevelTime;

		float mLevelStartedInTime;

		void UserInputInScene();

		// draw game world
		void DrawGameWorld();

		// mouse event actions
		void MouseButtonDown();

		void MouseMove();

		// main game loop updater function
		void Update();

		void LoadLevel();

		void Restartgame();

		void GameOver();

		void LevelCountDowntimer();

		void GameStartCountDowntimer();
	};
}



