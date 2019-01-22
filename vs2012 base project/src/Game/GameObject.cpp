#include "GameObject.h"

using namespace Game;

GameObject::GameObject(Constants::Texture texture,float posX, float posY, float rot, float scale)
{
	mTextureToDraw = texture;
	mPositionX = posX;
	mPositionY = posY;
	mRotatation = rot;
	mMoveSpeed = Constants::SWAP_SPEED;
	mScale = scale;
}

GameObject::GameObject()
{
	mMoveSpeed = Constants::SWAP_SPEED;
}

GameObject::GameObject(float posX, float posY, float rot, float scale)
{
	mPositionX = posX;
	mPositionY = posY;
	mRotatation = rot;
	mMoveSpeed = Constants::SWAP_SPEED;
	mScale = scale;
}

GameObject::~GameObject()
{
}


void GameObject::SetPositionX(float posX)
{
	mPositionX = posX;
}
void GameObject::SetPositionY(float posY)
{
	mPositionY = posY;
}

void GameObject::SetRotation(float rot)
{
	mRotatation = rot;
}

void GameObject::SetScale(float scale)
{
	mScale = scale;
}

void GameObject::SetTextureToDraw(Constants::Texture texture)
{
	mTextureToDraw = texture;
}

void GameObject::SetMoving(bool move, int dirX, int dirY)
{
	mMoving = move;
	mMoveDirectionX = dirX;
	mMoveDirectionY = dirY;
}

void GameObject::SetNewMovePositionX(float newMovePosX)
{
	mNewMovePositionX = newMovePosX;
}

void GameObject::SetNewMovePositionY(float newMovePosY)
{
	mNewMovePositionY = newMovePosY;
}

void GameObject::SetMoveSpeed(float moveSpeed)
{
	mMoveSpeed = moveSpeed;
}

float GameObject::GetPositionX()
{
	return mPositionX;
}
float GameObject::GetPositionY()
{
	return mPositionY;
}

float GameObject::GetRotation()
{
	return mRotatation;
}

float GameObject::GetScale()
{
	return mScale;
}

float GameObject::GetNewMovePositionX()
{
	return mNewMovePositionX;
}

float GameObject::GetNewMovePositionY()
{
	return mNewMovePositionY;
}

Constants::Texture GameObject::GetTexture()
{
	return mTextureToDraw;
}

void GameObject::Draw()
{
	// draw
	Engine::Instance().Render(mTextureToDraw, mPositionX, mPositionY,mRotatation,mScale);
}

void GameObject::MoveToPosition(float frameDeltaTime)
{
	if (mMoving)
	{
		// implement basic move feature for some game object
	}
}
