#include "Diamond.h"


using namespace Game;

Diamond::Diamond()
{
}
Diamond::Diamond(Constants::Texture drawTexture) : GameObject(mPositionX,mPositionY,mRotatation,mScale)
{
	this->mTextureToDraw = drawTexture;
	diamondId = -1;
	mMoveSpeed = Constants::SWAP_SPEED;
	mScale = 1;
	mScaleAnimUp = true;
	mScaleAnimDown = false;
	mStartMatching = false;
}

Diamond::~Diamond()
{	
	
}

void Diamond::SetDiamondId(int id)
{
	diamondId = id;
}

int Diamond::GetDiamondId()
{
	return diamondId;
}
void Diamond::SetDiamondGridId(int id)
{
	gridId = id;
}
int Diamond::GetDiamondGridId()
{
	return gridId;
}
void Diamond::SetColumn(int c)
{
	mPreviousColumn = mColumn;
	mColumn = c;
}
void Diamond::SetRow(int r)
{
	mPreviousRow = mRow;
	mRow = r;
}
int Diamond::GetColumn()
{
	return mColumn;
}
int Diamond::GetRow()
{
	return mRow;
}
int Diamond::GetPreviousColumn()
{
	return mColumn;
}
int Diamond::GetPreviousRow()
{
	return mRow;
}
bool Diamond::GetStartMatching()
{
	return mStartMatching;
}

void Diamond::SetStartMatching(bool matching)
{
	mStartMatching = matching;
}

void Diamond::SetShouldStartMatching(bool shouldStartMatch)
{
	mShouldStartMatching = shouldStartMatch;
}

void Diamond::SetShouldGoBackToPlace(bool goBackToPlace)
{
	mShouldGoBachToPlace = goBackToPlace;
}

void Diamond::SetInitialAnimation(bool runFlag)
{
	mRunInitialAnimationscale = runFlag;
	if (mRunInitialAnimationscale == true)
		mScale = 0;
}

bool Diamond::GetShouldGoBackToPlace()
{
	return mShouldGoBachToPlace;
}

Diamond::MatchDirection Diamond::GetCurrentMatchDirection()
{
	return mCurrentMatchDirection;
}
void Diamond::Draw()
{
	// parent draw method
	GameObject::Draw();

	// initialized animation method
	if (mRunInitialAnimationscale)
		InitialAnimation(Engine::Instance().GetLastFrameSeconds());

	// move method
	if(mMoving)
		MoveToPosition(Engine::Instance().GetLastFrameSeconds());
	//
	// run delete animation if we need
	if (diamondId == -2)
		DiamondDeleteAnimation(Engine::Instance().GetLastFrameSeconds());
}
void Diamond::MoveToPosition(float frameDeltaTime)
{
	if (mMoveDirectionX == 1)
	{
		mPositionX += (frameDeltaTime*mMoveSpeed);
		if (mPositionX >= mNewMovePositionX)
		{
			mPositionX = mNewMovePositionX;
			mMoving = false;
			mCurrentMatchDirection = MatchDirection::VERTICAL_MATCH;
			mStartMatching = mShouldStartMatching;
		}
	}
	else if (mMoveDirectionX == -1)
	{
		mPositionX -= (frameDeltaTime*mMoveSpeed);
		if (mPositionX <= mNewMovePositionX)
		{
			mPositionX = mNewMovePositionX;
			mMoving = false;
			mCurrentMatchDirection = MatchDirection::VERTICAL_MATCH;
			mStartMatching = mShouldStartMatching;
		}
	}
	if (mMoveDirectionY == 1)
	{
		mPositionY += (frameDeltaTime*mMoveSpeed);
		if (mPositionY >= mNewMovePositionY)
		{
			mPositionY = mNewMovePositionY;
			mMoving = false;
			mCurrentMatchDirection = MatchDirection::HORIZONTAL_MATCH;
			mStartMatching = mShouldStartMatching;
		}
	}
	else if (mMoveDirectionY == -1)
	{
		mPositionY -= (frameDeltaTime*mMoveSpeed);
		if (mPositionY <= mNewMovePositionY)
		{
			mPositionY = mNewMovePositionY;
			mMoving = false;
			mCurrentMatchDirection = MatchDirection::HORIZONTAL_MATCH;
			mStartMatching = mShouldStartMatching;
		}
	}
}
void Diamond::SetCurrentSwapObjectIndice(pair<int, int> swapObjectIndice)
{
	currentSwapObjectIndice = swapObjectIndice;
}

pair<int, int> Diamond::GetetCurrentSwapObjectIndice()
{
	return currentSwapObjectIndice;
}
void Diamond::DiamondDeleteAnimation(float frameDeltaTime)
{
	if (mScaleAnimUp)
	{
		mScale += (frameDeltaTime * Constants::DIAMOND_DELETE_SPEED);
		if (mScale >= 1.2f)
		{
			mScaleAnimDown = true;
			mScaleAnimUp = false;
		}
	}
	if (mScaleAnimDown)
	{
		mScale -= (frameDeltaTime * Constants::DIAMOND_DELETE_SPEED);
		if (mScale <= Constants::MINIMIM_SIZE_TO_DELETE_DIAMOND)
			diamondId = -1;
	}
		
}
void Diamond::InitialAnimation(float frameDeltaTime)
{
	mScale += (frameDeltaTime * Constants::DIAMOND_DELETE_SPEED);
	if (mScale >= 1)
	{
		mScale = 1;
		mRunInitialAnimationscale = false;
	}
}
