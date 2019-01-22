#pragma once
#include "GameObject.h"


class UiObject : public GameObject
{
public:
	UiObject(Constants::Texture texture, float posX, float posY, float rot, float scale);
	~UiObject();
};

