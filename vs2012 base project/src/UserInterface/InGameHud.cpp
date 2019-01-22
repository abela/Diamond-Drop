#include "InGameHud.h"
#include "Utils.h"


InGameHud::InGameHud(float totalLeveltime, float levelStartedInTime, int currentLevelScore) : 
	mpRestartButton(new UiObject(Constants::TEXTURE_RESTART, Constants::RESTART_TEXT_POS_X, Constants::RESTART_TEXT_POS_Y,0.0f,1.0f))
{
	this->currentLevelScore = currentLevelScore;
	this->totalLeveltime = totalLeveltime;
	this->levelStartedInTime = levelStartedInTime;
}


InGameHud::~InGameHud()
{
}

void InGameHud::Render(float totalLeveltime, float levelStartedInTime,int currentLevelScore)
{
	// update
	this->currentLevelScore = currentLevelScore;
	this->totalLeveltime = totalLeveltime;
	this->levelStartedInTime = levelStartedInTime;
	//
	if (this->levelStartedInTime>0)
		GameStartCountDowntimer();
	//
	//
	DrawScore();
	LevelCountDowntimer();
}

void InGameHud::DrawScore()
{
	string score = "Score: ";
	string scoreString = to_string(this->currentLevelScore);
	score += scoreString;
	char const *pchar = score.c_str();
	Engine::Instance().Write(pchar, Constants::GAME_SCORE_POS_X, Constants::GAME_SCORE_POS_Y);
}

void InGameHud::LevelCountDowntimer()
{
	// create time string
	Engine::Instance().Write(Utils::Instance().GetNiceTimeBasedOnValue(this->totalLeveltime).c_str(), 75, 450);
}

void InGameHud::GameStartCountDowntimer()
{
	string s = this->levelStartedInTime < 1 ? "Go!" : Utils::Instance().GetSecondBasedOnValue(this->levelStartedInTime);
	Engine::Instance().Write(s.c_str(), Constants::GAME_START_CDN_TIMER_X, Constants::GAME_START_CDN_TIMER_Y);
}

void InGameHud::GameOver()
{
	Engine::Instance().Write("Game Ended", Constants::GAME_ENDED_TITLE_X, Constants::GAME_ENDED_TITLE_Y);
	string finalScoreStr = "Score: " + to_string(this->currentLevelScore);
	Engine::Instance().Write(finalScoreStr.c_str(), Constants::FINAL_SCORE_X, Constants::FINAL_SCORE_Y);
	mpRestartButton->Draw();
	//
}
