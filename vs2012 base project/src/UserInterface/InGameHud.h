#pragma once
#include "UserInterface.h"
#include "UiObject.h"

using namespace King;

class InGameHud : public UserInterface
{
public:
	InGameHud(float totalLeveltime, float levelStartedInTime, int currentLevelScore);
	~InGameHud();
	
	void Render(float totalLeveltime, float levelStartedInTime, int currentLevelScore);

	void GameOver();

private:

	unique_ptr <UiObject> mpRestartButton;

	void DrawScore();

	void LevelCountDowntimer();

	void GameStartCountDowntimer();

	float totalLeveltime;
	float levelStartedInTime;
	int currentLevelScore;
	
};

