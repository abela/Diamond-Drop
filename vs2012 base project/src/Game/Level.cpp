#include "Level.h"
#include "Diamond.h"
#include <algorithm>

using namespace Game;

Level::Level() : diamonds(), mPdiamondSelection(new GameObject(Constants::Texture::TEXTURE_DIAMOND_SELECTION, -100, -100, 0, 1))
{

}

Level::~Level()
{
}

void Level::SetLevelEnded(bool levelEnded)
{
	mLevelEnded = levelEnded;
}

bool Level::GetLevelEnded()
{
	return mLevelEnded;
}


void Level::RenderLevel()
{
	// draw diamonds
	for (int i = 0; i < Constants::GAME_TABLE_DIMENSION; i++)
	{
		for (int j = 0; j < Constants::GAME_TABLE_DIMENSION; j++)
		{
			if(diamonds[make_pair(i, j)])
				diamonds[make_pair(i,j)]->Draw();
		}
	}
	//
	UpdateDiamondsArray();
	//
	if (mShowDiamondSelection)
		mPdiamondSelection->Draw();
	//
}

void Level::LoadLevel()
{
	CreateLevelAtStart();
}


int Level::GetCurrentLevelScore()
{
	return mCurrentLevelScore;
}


void Level::CreateLevelAtStart()
{
	// initialize board parameters

	mRowCount = Constants::GAME_TABLE_DIMENSION;
	mColumnCount =  Constants::GAME_TABLE_DIMENSION;

	mDiamondsStarPosX = 320;
	mDiamondsStartPosY = -260;
	//
	mDiamondsStartNextPosY = 100;
	//
	mDiamondsPositionStepX = 45;
	mDiamondsPositionStepY = 45;

	float diamondPositionX = mDiamondsStarPosX;
	float diamondPositionY = mDiamondsStartPosY;
	//
	int previousDiamondRowIndex = 0;
	int previousDiamondColIndex = 0;
	//
	mTexturesRandomerStartIndex = (int)Constants::Texture::TEXTURE_BLUE;
	mTexturesRandomEndIndex = (int)Constants::TEXTURE_YELLOW;
	//
	// enumerate diamonds with unique id-s
	int gridId = 0;
	//
	// create and shuffle diamonds
	for (int i = 0; i < mRowCount; i++)
	{		
		for (int j = 0; j < mColumnCount; j++)
		{
			// generate random texture - avoid horizontal triple diamonds
			int randomTextureId = Utils::Instance().GetRandomDiamondPlacementIndex(previousDiamondColIndex, previousDiamondRowIndex,mTexturesRandomerStartIndex, mTexturesRandomEndIndex);
			//
			Constants::Texture nextTexture = (Constants::Texture)randomTextureId;
			//
			unique_ptr <Diamond>diamond(new Diamond(nextTexture));
			diamond->SetPositionX(diamondPositionX);
			diamond->SetPositionY(diamondPositionY);
			diamond->SetDiamondId(randomTextureId);
			diamond->SetRotation(0.0f);
			diamond->SetDiamondGridId(gridId);
			diamond->SetNewMovePositionY(diamondPositionY + mColumnCount * mDiamondsPositionStepY);
			diamond->SetMoving(true,0,1);
			diamond->SetMoveSpeed(Constants::FALLING_SPEED * 1.5f);
			diamond->SetShouldStartMatching(false);
			diamond->SetColumn(i);
			diamond->SetCurrentSwapObjectIndice(make_pair(-1,-1));
			diamond->SetRow(j);
			diamond->SetShouldStartMatching(true);
			diamondPositionX += mDiamondsPositionStepX;
			//
			// save last row and column indices to avoid triple diamonds together
			previousDiamondRowIndex = randomTextureId;
			if (i > 0)
			{
				previousDiamondColIndex = diamonds[make_pair(i-1,j)]->GetDiamondId();
			}
			//
			diamonds[make_pair(i, j)] = move(diamond);
			//
			gridId++;
			//
		}
		//
		diamondPositionX = mDiamondsStarPosX;
		diamondPositionY += mDiamondsPositionStepY;
		//
	}
	//
}

// mouse button down in level
void Level::MouseButtonDown()
{
	if (mShowDiamondSelection == false)
	{
		// try to pick diamond on board
		currentPickedDiamondIndex = pickDiamond();
		if (diamonds[currentPickedDiamondIndex])
		{
			mShowDiamondSelection = true;
			mPdiamondSelection->SetPositionX(diamonds[currentPickedDiamondIndex]->GetPositionX());
			mPdiamondSelection->SetPositionY(diamonds[currentPickedDiamondIndex]->GetPositionY());
			//
			mouseXCoordsCounter = 0;
			mouseYCoordsCounter = 0;
			//
			// save mouse first button down position
			preMouseX = Engine::Instance().GetMouseX();
			prevMouseY = Engine::Instance().GetMouseY();
		}
	}
	else
	{
		// try to pick next diamond on board
		nextPickeddiamondIndex = pickDiamond();
		if (diamonds[nextPickeddiamondIndex])
		{
			// check same object click and same color object swap
			if (diamonds[nextPickeddiamondIndex]->GetDiamondId() != diamonds[currentPickedDiamondIndex]->GetDiamondId())
			{
				//
				SwapTwoDiaomnds(diamonds[nextPickeddiamondIndex].get(), diamonds[currentPickedDiamondIndex].get(), true);
				//
				mShowDiamondSelection = false;
				//
			}
		}
	}
}

void Level::MouseMove()
{
	// if button is down detect next diamond based on mouse elative motions
	if (Engine::Instance().GetMouseButton())
	{
		if (mShowDiamondSelection)
		{
			if (Engine::Instance().GetMouseX() - preMouseX > Constants::MINIMUM_SWIPE_DELTA)
			{
				if (diamonds[currentPickedDiamondIndex])
				{
					// awoid same color swap
					if (diamonds[currentPickedDiamondIndex]->GetRow() < mRowCount - 1)
					{
						nextPickeddiamondIndex = make_pair(diamonds[currentPickedDiamondIndex]->GetColumn(), diamonds[currentPickedDiamondIndex]->GetRow() + 1);
						if (diamonds[currentPickedDiamondIndex]->GetDiamondId() != diamonds[nextPickeddiamondIndex]->GetDiamondId())
						{
							SwapTwoDiaomnds(diamonds[nextPickeddiamondIndex].get(), diamonds[currentPickedDiamondIndex].get(), true);
							//
							mShowDiamondSelection = false;
						}
							
					}
				}
			}

			if (Engine::Instance().GetMouseX() - preMouseX < -Constants::MINIMUM_SWIPE_DELTA)
			{
				if (diamonds[currentPickedDiamondIndex])
				{
					if (diamonds[currentPickedDiamondIndex]->GetRow() > 0)
					{
						nextPickeddiamondIndex = make_pair(diamonds[currentPickedDiamondIndex]->GetColumn(), diamonds[currentPickedDiamondIndex]->GetRow() - 1);
						// avoid same color swap
						if (diamonds[currentPickedDiamondIndex]->GetDiamondId() != diamonds[nextPickeddiamondIndex]->GetDiamondId())
						{
							SwapTwoDiaomnds(diamonds[nextPickeddiamondIndex].get(), diamonds[currentPickedDiamondIndex].get(), true);
							//
							mShowDiamondSelection = false;
						}	
					}
				}
			}

			if (Engine::Instance().GetMouseY() - prevMouseY > Constants::MINIMUM_SWIPE_DELTA)
			{
				if (diamonds[currentPickedDiamondIndex])
				{
					if (diamonds[currentPickedDiamondIndex]->GetColumn() < mColumnCount - 1)
					{
						nextPickeddiamondIndex = make_pair(diamonds[currentPickedDiamondIndex]->GetColumn() + 1, diamonds[currentPickedDiamondIndex]->GetRow());
						// avoid same color swap
						if (diamonds[currentPickedDiamondIndex]->GetDiamondId() != diamonds[nextPickeddiamondIndex]->GetDiamondId())
						{
							SwapTwoDiaomnds(diamonds[nextPickeddiamondIndex].get(), diamonds[currentPickedDiamondIndex].get(), true);
							//
							mShowDiamondSelection = false;
						}
					}
				}
			}

			if (Engine::Instance().GetMouseY() - prevMouseY < -Constants::MINIMUM_SWIPE_DELTA)
			{
				if (diamonds[currentPickedDiamondIndex])
				{
					if (diamonds[currentPickedDiamondIndex]->GetColumn() > 0)
					{
						nextPickeddiamondIndex = make_pair(diamonds[currentPickedDiamondIndex]->GetColumn() - 1, diamonds[currentPickedDiamondIndex]->GetRow());
						if (diamonds[currentPickedDiamondIndex]->GetDiamondId() != diamonds[nextPickeddiamondIndex]->GetDiamondId())
						{
							SwapTwoDiaomnds(diamonds[nextPickeddiamondIndex].get(), diamonds[currentPickedDiamondIndex].get(), true);
							//
							mShowDiamondSelection = false;
						}
					}
				}
			}
		}
	}
	//

	//
}

// swap two diamonds, set should start matching if we want check pair match
void Level::SwapTwoDiaomnds(Diamond *diamond1, Diamond *diamond2, bool shouldStartMatching)
{
	int dirX = 0, dirY = 0;
	bool changeDiamond = false;
	//
	if (diamond1->GetColumn() == diamond2->GetColumn() &&
		diamond1->GetRow() - diamond2->GetRow() == 1) // we are switching to right
	{
		dirX = 1;
		changeDiamond = true;
		diamond1->SetRow(diamond1->GetRow() - 1);
		diamond2->SetRow(diamond2->GetRow() + 1);
		diamond1->SetDiamondGridId(diamond1->GetDiamondGridId() - 1);
		diamond2->SetDiamondGridId(diamond2->GetDiamondGridId() + 1);
	}
	else if (diamond1->GetColumn() == diamond2->GetColumn() &&
			diamond1->GetRow() - diamond2->GetRow() == -1) // we are switching to left
	{
		dirX = -1;
		changeDiamond = true;
		diamond1->SetRow(diamond1->GetRow() + 1);
		diamond2->SetRow(diamond2->GetRow() - 1);
		diamond1->SetDiamondGridId(diamond1->GetDiamondGridId() + 1);
		diamond2->SetDiamondGridId(diamond2->GetDiamondGridId() - 1);
	}
	else if (diamond1->GetRow() == diamond2->GetRow() &&
		diamond1->GetColumn() - diamond2->GetColumn() == 1) // we are switching to down
	{
		dirY = -1;
		changeDiamond = true;
		diamond1->SetColumn(diamond1->GetColumn() - 1);
		diamond2->SetColumn(diamond2->GetColumn() + 1);
		diamond1->SetDiamondGridId(diamond1->GetDiamondGridId() - mColumnCount);
		diamond2->SetDiamondGridId(diamond2->GetDiamondGridId() + mColumnCount);
	}
	else if (diamond1->GetRow() == diamond2->GetRow() &&
		diamond1->GetColumn() - diamond2->GetColumn() == -1) // we are switching to up
	{
		dirY = 1;
		changeDiamond = true;
		diamond1->SetColumn(diamond1->GetColumn() + 1);
		diamond2->SetColumn(diamond2->GetColumn() - 1);
		diamond1->SetDiamondGridId(diamond1->GetDiamondGridId() + mColumnCount);
		diamond2->SetDiamondGridId(diamond2->GetDiamondGridId() - mColumnCount);
	}
	if (changeDiamond)
	{
		// move diamonds to switch place
		diamond1->SetNewMovePositionX(diamond2->GetPositionX());
		diamond1->SetNewMovePositionY(diamond2->GetPositionY());
		//
		diamond2->SetNewMovePositionX(diamond1->GetPositionX());
		diamond2->SetNewMovePositionY(diamond1->GetPositionY());
		//
		diamond1->SetMoveSpeed(Constants::SWAP_SPEED);
		diamond2->SetMoveSpeed(Constants::SWAP_SPEED);
		//
		diamond1->SetMoving(true, -dirX, dirY);
		diamond2->SetMoving(true, dirX, -dirY);
		//
		diamond1->SetShouldStartMatching(shouldStartMatching);
		diamond2->SetShouldStartMatching(shouldStartMatching);
		//
		diamond1->SetShouldGoBackToPlace(false);
		diamond2->SetShouldGoBackToPlace(false);
		//
		// exhange diamonds in map
		swap(*diamond1, *diamond2);

		// save current swap object to each other
		diamond1->SetCurrentSwapObjectIndice(make_pair(diamond2->GetColumn(),diamond2->GetRow()));
		diamond2->SetCurrentSwapObjectIndice(make_pair(diamond1->GetColumn(), diamond1->GetRow()));
	}
}


// check mouse position if is in some diamond rect, if so return it
pair<int, int> Level::pickDiamond()
{
	//
	pair<int, int> foundDiamondIndex;
	bool diamondfound = false;
	//
	for (int i = 0; i < mRowCount; i++)
	{
		for (int j = 0; j < mColumnCount; j++)
		{
			if (diamonds[make_pair(i, j)]!=NULL)
			{
				float mousePosX = Engine::Instance().GetMouseX();
				float mousePosY = Engine::Instance().GetMouseY();
				//
				// check mouse position in every diamond's rect
				if (Utils::Instance().IsMouseInSquare(mousePosX,
					mousePosY,
					diamonds[make_pair(i, j)]->GetPositionX(),
					diamonds[make_pair(i, j)]->GetPositionY(),
					Engine::Instance().GetTextureWidth(diamonds[make_pair(i, j)]->GetTexture()),
					Engine::Instance().GetTextureHeight(diamonds[make_pair(i, j)]->GetTexture())))
				{
					foundDiamondIndex = make_pair(i, j);
					diamondfound = true;
					break;
				}
			}
				
		}
		if (diamondfound)
			break;
	}
	return foundDiamondIndex;
}

// Debug function: print array
void Level::PrintDiamondsArray()
{
	for (int i = 0; i < mRowCount; i++)
	{
		for (int j = 0; j < mColumnCount; j++)
		{
			if(diamonds[make_pair(i, j)]!=NULL)
				printf(" %d ", diamonds[make_pair(i, j)]->GetDiamondId());
			else printf(" 0 ");
		}
		printf("\n");
	}
	printf("\n\n\n");
}

void Level::AddDiamondAt(int column, int row)
{
	float startFallSpeed = Constants::FALLING_SPEED;
	float fallspeedDecreaser = startFallSpeed / 20;
	for (int i = column; i > 0; i--)
	{
		// check upper diamonds
		if (diamonds[make_pair(i-1,row)])
		{
			// exchange new diamonds position and calculate new drop position
			diamonds[make_pair(i, row)] = move(diamonds[make_pair(i - 1, row)]);
			diamonds[make_pair(i, row)]->SetNewMovePositionY(mDiamondsStartNextPosY + i * mDiamondsPositionStepY);
			diamonds[make_pair(i, row)]->SetColumn(diamonds[make_pair(i, row)]->GetColumn() + 1);
			diamonds[make_pair(i, row)]->SetDiamondGridId(diamonds[make_pair(i, row)]->GetDiamondGridId() + mColumnCount);
			diamonds[make_pair(i, row)]->SetShouldStartMatching(true);
			diamonds[make_pair(i, row)]->SetMoveSpeed(startFallSpeed);
			diamonds[make_pair(i, row)]->SetMoving(true, 0, 1);
			startFallSpeed -= fallspeedDecreaser;
		}
	}
	// add last diamond at very top of position
	int randomTextureId = Utils::Instance().GetRandomIndexInInterval(mTexturesRandomerStartIndex, mTexturesRandomEndIndex);
	//
	Constants::Texture nextTexture = (Constants::Texture)randomTextureId;
	//
	// initialize diamond and save it in array
	unique_ptr <Diamond> diamond(new Diamond(nextTexture));
	diamond->SetPositionX(mDiamondsStarPosX + row * mDiamondsPositionStepX);
	diamond->SetPositionY(mDiamondsStartNextPosY - mDiamondsPositionStepY);
	diamond->SetRotation(0.0f);
	diamond->SetDiamondId(randomTextureId);
	diamond->SetDiamondGridId(row);
	diamond->SetColumn(0);
	diamond->SetRow(row);
	diamond->SetShouldStartMatching(true);
	diamond->SetCurrentSwapObjectIndice(make_pair(-1, -1));
	//
	diamond->SetNewMovePositionY(mDiamondsStartNextPosY);
	diamond->SetMoving(true, 0, 1);
	//
	diamonds[make_pair(0, row)] = move(diamond);
	//
}


// all diamonds checking function
void Level::UpdateDiamondsArray()
{
	// check if some diamond has invalid id and delete it
	for (int i = 0; i < mRowCount; i++)
	{
		for (int j = 0; j < mColumnCount; j++)
		{
			pair <int,int> nextDiamondIndex = make_pair(i, j);
			if (diamonds[nextDiamondIndex])
			{
				// check on delete
				if (diamonds[nextDiamondIndex]->GetDiamondId() == -1)
				{
					diamonds[nextDiamondIndex].reset();
					// add next diamond at index
					AddDiamondAt(i, j);
					return;
				}
				//
				// check matching
				if (diamonds[nextDiamondIndex]->GetStartMatching())
				{
					diamonds[nextDiamondIndex]->SetStartMatching(false);

					bool emptyHorizontals = true, emptyVerticals = true;
					vector <pair<int,int>> horizontalDiamondsToDelete = DetectHorizontalMatchWithDiamond(diamonds[nextDiamondIndex]->GetDiamondId(),
						diamonds[nextDiamondIndex]->GetColumn(),
						diamonds[nextDiamondIndex]->GetRow());
					//
					vector <pair<int, int>> verticalDiamondsDiamondsToDelete = DetectVerticalMatchWithDiamond(diamonds[nextDiamondIndex]->GetDiamondId(), 
						diamonds[nextDiamondIndex]->GetDiamondGridId(),
						diamonds[nextDiamondIndex]->GetColumn(),
						diamonds[nextDiamondIndex]->GetRow());
					//

					if ((int)horizontalDiamondsToDelete.size() >= Constants::MINIMUM_DIAMONDS_TO_DELETE)
					{
						emptyHorizontals = false;
						diamonds[nextDiamondIndex]->SetDiamondId(-2);
						//
						// update level score
						mCurrentLevelScore += (horizontalDiamondsToDelete.size() * Constants::DIAMOND_SCORE_MULTIPLIER);
						//
						for (int i = 0; i < (int)horizontalDiamondsToDelete.size(); i++)
						{
							diamonds[horizontalDiamondsToDelete[i]]->SetDiamondId(-2);
						}
					}

					if ((int)verticalDiamondsDiamondsToDelete.size() >= Constants::MINIMUM_DIAMONDS_TO_DELETE)
					{
						emptyVerticals = false;
						diamonds[nextDiamondIndex]->SetDiamondId(-2);
						//
						// update level score
						mCurrentLevelScore += (verticalDiamondsDiamondsToDelete.size() * Constants::DIAMOND_SCORE_MULTIPLIER);
						//
						for (int i = 0; i < (int)verticalDiamondsDiamondsToDelete.size(); i++)
						{
							diamonds[verticalDiamondsDiamondsToDelete[i]]->SetDiamondId(-2);
						}
						//
					}
					// if we couldn't delete any rows lets save diamonds and then get them back to place
					if (emptyHorizontals && emptyVerticals)
					{
						// avoid bad indexing initial swap
						if (diamonds[nextDiamondIndex]->GetetCurrentSwapObjectIndice().first!=-1 &&
							diamonds[nextDiamondIndex]->GetetCurrentSwapObjectIndice().second!=-1)
						{
							if (diamonds[diamonds[nextDiamondIndex]->GetetCurrentSwapObjectIndice()]->GetShouldGoBackToPlace())
							{
								SwapTwoDiaomnds(diamonds[nextDiamondIndex].get(), diamonds[diamonds[nextDiamondIndex]->GetetCurrentSwapObjectIndice()].get(), false);
							}
							else diamonds[nextDiamondIndex]->SetShouldGoBackToPlace(true);
						}
					}
				}
			}
		}
	}
}


//// look left and right side of current diamond. if we find same kind of diamonds add them into elete aray
vector <pair<int,int>> Level::DetectHorizontalMatchWithDiamond(int diamondId,int column, int row)
{
	vector <pair<int, int>> diamondstoDelete;
	//
	//	
	bool stopLeftSearch = false, stopRightSearch = false, stopSearch = false;
	int nextDiamondId = 0;
	for (int i = row + 1, j = row - 1; stopSearch == false; i++, j--)
	{
		if (j < 0)
			stopLeftSearch = true;
		if (i > mRowCount - 1)
			stopRightSearch = true;

		if (stopRightSearch == false)
		{
			if (diamonds[make_pair(column, i)])
			{
				int nextDiamondId = diamonds[make_pair(column, i)]->GetDiamondId();
				if (nextDiamondId == diamondId)
				{
					diamondstoDelete.push_back(make_pair(column, i));
				}
				else stopRightSearch = true;
			}
			else stopRightSearch = true;
		}
		if (stopLeftSearch == false)
		{
			if (diamonds[make_pair(column, j)])
			{
				int nextDiamondId = diamonds[make_pair(column, j)]->GetDiamondId();
				if (nextDiamondId == diamondId)
				{
					diamondstoDelete.push_back(make_pair(column, j));
				}
				else stopLeftSearch = true;
			}
			else stopLeftSearch = true;
		}
		stopSearch = (stopLeftSearch == true && stopRightSearch == true);
	}

	return diamondstoDelete;
}
// look up and down side of current diamond. if we find same kind of diamonds add them into elete aray
vector <pair<int, int>> Level::DetectVerticalMatchWithDiamond(int diamondId, int gridId, int column, int row)
{
	vector <pair<int, int>> diamondstoDelete;
	//
	//
	bool stopUpperSearch = false, stopDownSearch = false, stopSearch = false;
	int nextDiamondId = 0;
	for (int i = column+1, j = column-1; stopSearch == false; i++, j--)
	{
		if (j < 0)
			stopUpperSearch = true;
		if (i > mRowCount - 1)
			stopDownSearch = true;

		if (stopDownSearch == false)
		{
			if (diamonds[make_pair(i, row)])
			{
				int nextDiamondId = diamonds[make_pair(i, row)]->GetDiamondId();
				if (nextDiamondId == diamondId)
				{
					diamondstoDelete.push_back(make_pair(i, row));
				}
				else stopDownSearch = true;
			}
			else stopDownSearch = true;
		}
		if (stopUpperSearch == false)
		{
			if (diamonds[make_pair(j, row)])
			{
				int nextDiamondId = diamonds[make_pair(j, row)]->GetDiamondId();
				if (nextDiamondId == diamondId)
				{
					diamondstoDelete.push_back(make_pair(j, row));
				}
				else stopUpperSearch = true;
			}
			else stopUpperSearch = true;
		}
		stopSearch = (stopUpperSearch == true && stopDownSearch == true);
	}
		
	return diamondstoDelete;
}
