#pragma once

#include <king/Engine.h>
#include <king/Updater.h>
#include <vector>
#include "Diamond.h"
#include "Constants.h"
#include "Utils.h"
#include <map>


using namespace King;
using namespace std;

namespace Game
{
	class Level
	{
	public:
		Level();
		~Level();

		void RenderLevel();

		void LoadLevel();

		void MouseButtonDown();

		void MouseMove();

		int GetCurrentLevelScore();

		void SetLevelEnded(bool levelEnded);

		bool GetLevelEnded();
	
	private:

		// game area dimensions
		int mRowCount;
		int mColumnCount;

		// diamonds draw start position
		float mDiamondsStarPosX;
		float mDiamondsStartPosY;

		float mDiamondsStartNextPosY;

		// delta x and y between two diamonds
		float mDiamondsPositionStepX;
		float mDiamondsPositionStepY;

		int mTexturesRandomerStartIndex;
		int mTexturesRandomEndIndex;

		int mCurrentLevelScore;

		bool mLevelEnded;

		float mouseXCoordsCounter;
		float mouseYCoordsCounter;

		float preMouseX;
		float prevMouseY;

		// if click on somediamond show selection
		bool mShowDiamondSelection;

		// diamond selection image
		unique_ptr <GameObject> mPdiamondSelection;

		// current picked diamond
		pair<int,int> currentPickedDiamondIndex;

		// next picked diamond
		pair<int, int> nextPickeddiamondIndex;

		void CreateLevelAtStart();

		// in game diamonds
		map <pair<int,int> , unique_ptr<Diamond>> diamonds;

		void AddDiamondAt(int column, int row);
		
		// update diamond elements state for each game frame
		void UpdateDiamondsArray();

		vector <pair<int,int>> DetectHorizontalMatchWithDiamond(int diamondId, int column, int row);

		vector <pair<int, int>> DetectVerticalMatchWithDiamond(int diamondId, int gridId, int column, int row);

		pair<int, int> pickDiamond();

		void PrintDiamondsArray();

		void SwapTwoDiaomnds(Diamond *diamond1, Diamond *diamond2, bool shouldStartMatching);
	};
}



