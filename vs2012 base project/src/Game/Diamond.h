#pragma once

#include "GameObject.h"
#include "Constants.h"

namespace Game 
{
	class Diamond : public GameObject
	{
	public:
		enum MatchDirection
		{
			NO_MATCH,
			VERTICAL_MATCH,
			HORIZONTAL_MATCH
		};
		// init Diamond with draw texture Id
		Diamond();
		Diamond(Constants::Texture drawTexture);
		~Diamond();
		//
		// id setter and getter
		void SetDiamondId(int id);
		int GetDiamondId();

		void SetDiamondGridId(int id);
		int GetDiamondGridId();

		void SetColumn(int c);
		void SetRow(int r);

		int GetColumn();
		int GetRow();

		int GetPreviousColumn();
		int GetPreviousRow();

		bool GetStartMatching();

		void SetStartMatching(bool matching);

		void SetShouldStartMatching(bool shouldStartMatch);

		void SetShouldGoBackToPlace(bool goBackToPlace);

		void SetInitialAnimation(bool runFlag);

		bool GetShouldGoBackToPlace();

		MatchDirection GetCurrentMatchDirection();

		// override draw
		void Draw();

		// override move to Position
		void MoveToPosition(float frameDeltaTime);

		void SetCurrentSwapObjectIndice(pair<int, int> swapObjectIndice);

		pair<int, int> GetetCurrentSwapObjectIndice();

		//
	private:
		
		float startPositionY;

		// diamond id
		int diamondId;

		// grid id
		int gridId;

		int mColumn;

		int mRow;

		int mPreviousColumn;

		int mPreviousRow;

		// diamond changed position, so we need to check matching
		bool mStartMatching;

		bool mRunInitialAnimationscale;

		// shoulddiamonds start matching algorithm after swap, or they just go back to own position
		bool mShouldStartMatching;

		bool mShouldGoBachToPlace;

		bool mScaleAnimUp;

		bool mScaleAnimDown;

		// current match direction
		MatchDirection mCurrentMatchDirection;

		// current swapObject
		pair<int,int> currentSwapObjectIndice;

		void DiamondDeleteAnimation(float frameDeltaTime);

		void InitialAnimation(float frameDeltaTime);

	};
}

