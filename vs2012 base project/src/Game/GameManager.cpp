#include "GameManager.h"
#include "Constants.h"

using namespace Game;

GameManager::GameManager() : mCurrentLevel(new Level()),
mpLevelBackgroundObject(new GameObject(Constants::Texture::TEXTURE_BACKGROUND, 0, 0, 0, 1)),
mpLevelBackgroundHoverObject(new GameObject(Constants::Texture::TEXTURE_BACKGROUND_HOVER, 182, 0, 0, 1)),
mpInGameHud(new InGameHud(mTotalLevelTime, mLevelStartedInTime, 0))
{
	// init game world textures
	mLevelStarted = false;
	mLevelEnded = false;
	mLevelStartedInTime = Constants::GAME_START_CDN_TIMER;
	//
}

GameManager::~GameManager()
{
}

// main game loop updater function
void GameManager::Update()
{
	// level countdown
	LevelCountDowntimer();
	// draw game world
	DrawGameWorld();
	// main user input on scene
	UserInputInScene();
}

void GameManager::Run()
{
	mTotalLevelTime = Constants::TOTAL_LEVEL_TIME;

	// start renderer
	Engine::Instance().Start(*this);
}

void GameManager::MouseButtonDown()
{
	if (mCurrentLevel)
		mCurrentLevel->MouseButtonDown();
	//
	if (mLevelEnded)
	{
		// check restart button down
		if (Utils::Instance().IsMouseInSquare(Engine::Instance().GetMouseX(),
			Engine::Instance().GetMouseY(),
			Constants::RESTART_TEXT_POS_X, Constants::RESTART_TEXT_POS_Y,
			Engine::Instance().GetTextureWidth(Constants::TEXTURE_RESTART),
			Engine::Instance().GetTextureHeight(Constants::TEXTURE_RESTART)))
		{
			Restartgame();
		}
		//
	}
	//
}

void GameManager::MouseMove()
{
	// set mouse motion event in level
	if (mLevelEnded == false)
	{
		if (mCurrentLevel)
			mCurrentLevel->MouseMove();
	}
	//
}

void GameManager::LoadLevel()
{
	mLevelStarted = true;
	mLevelEnded = false;
	mCurrentLevel->LoadLevel();
	//
}
// restart current level
void GameManager::Restartgame()
{
	// reset level
	mCurrentLevel.reset(new Level());
	//
	// re-initialize level variables again
	mLevelStarted = false;
	mLevelEnded = false;
	mTotalLevelTime = Constants::TOTAL_LEVEL_TIME;
	mLevelStartedInTime = Constants::GAME_START_CDN_TIMER;
}

void GameManager::GameOver()
{
	mpInGameHud->GameOver();
}

void GameManager::UserInputInScene()
{
	if (Engine::Instance().GetMouseButtonDown())
	{
		//
		if (Engine::Instance().MouseButtonDownFlag() == false)
		{
			Engine::Instance().SetMouseButtonDownFlag(true);
			//
			MouseButtonDown();
		}
	}
	MouseMove();
	//
}


void GameManager::DrawGameWorld()
{
	// first, draw level background
	if (mpLevelBackgroundObject) 
		mpLevelBackgroundObject->Draw();

	// play till level ends
	if (mLevelEnded == false)
	{
		if (mCurrentLevel)
		{
			// draw level
			mCurrentLevel->RenderLevel();
		}
		//
		// rebnder UI
		if(mpInGameHud)
			mpInGameHud->Render(mTotalLevelTime, mLevelStartedInTime, mCurrentLevel->GetCurrentLevelScore());
		//
		
	}
	// once level ends show highscore
	else
	{
		GameOver();
	}
	//
	// draw level background hover
	if (mpLevelBackgroundHoverObject)
		mpLevelBackgroundHoverObject->Draw();

}

// level countdown timer
void GameManager::LevelCountDowntimer()
{
	if (mLevelStarted == true)
	{
		mTotalLevelTime -= Engine::Instance().GetLastFrameSeconds();
		//
		// chek level end
		if (mTotalLevelTime <= 0)
		{
			mLevelEnded = true;
			mLevelStarted = false;
			// tell level to end
			mCurrentLevel->SetLevelEnded(mLevelEnded);
		}
		//
	}
	else GameStartCountDowntimer();
}
// game start 3,2,1 go timer
void GameManager::GameStartCountDowntimer()
{
	if (mLevelStarted == false && mLevelEnded == false)
	{
		mLevelStartedInTime -= Engine::Instance().GetLastFrameSeconds();
		//
		if (mLevelStartedInTime <= 0)
		{
			// load level after star timer ends
			LoadLevel();
		}
		//
	}
}

