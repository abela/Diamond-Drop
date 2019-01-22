#pragma once

#include <king/Engine.h>
#include <king/Updater.h>

using namespace King;

namespace Game
{
	// abstract class of all gameobjects
	class IGameObject
	{
	public:
		// main drawing function
		virtual void Draw() = 0;

		virtual void SetPositionX(float posX) = 0;
		virtual void SetPositionY(float posY) = 0;
		virtual void SetRotation(float rot) = 0;
		virtual void SetScale(float scale) = 0;

		virtual float GetPositionX() = 0;
		virtual float GetPositionY() = 0;
		virtual float GetRotation() = 0;
		virtual float GetScale() = 0;

	protected:

		// current draw texture Id
		Constants::Texture mTextureToDraw;
		// positions and rotation
		float mPositionX;
		float mPositionY;
		float mRotatation;
		float mScale;
	};

	class GameObject : public IGameObject
	{
	public:
		// assign draw engine with dependence injection
		GameObject(Constants::Texture texture, float posX, float posY, float rot, float scale);
		GameObject();
		GameObject(float posX, float posY, float rot, float scale);
		~GameObject();

		// Setters
		void SetPositionX(float posX);
		void SetPositionY(float posY);
		void SetRotation(float rot);
		void SetScale(float scale);
		//
		void SetTextureToDraw(Constants::Texture texture);
		void SetMoving(bool move, int dirX, int dirY);
		void SetNewMovePositionX(float newMovePosX);
		void SetNewMovePositionY(float newMovePosY);
		void SetMoveSpeed(float moveSpeed);

		// getters
		float GetPositionX();
		float GetPositionY();
		float GetRotation();
		float GetScale();
		float  GetNewMovePositionX();
		float  GetNewMovePositionY();
		Constants::Texture GetTexture();

		// main drawing function
		void Draw();

		// move To Position
		void MoveToPosition(float frameDeltaTime);

	protected:

		bool mMoving;
		float mMoveSpeed;
		//
		float mNewMovePositionX;
		float mNewMovePositionY;
		//
		int mMoveDirectionX;
		int mMoveDirectionY;
		//
	};
}





