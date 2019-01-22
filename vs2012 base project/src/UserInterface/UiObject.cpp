#include "UiObject.h"




UiObject::UiObject(Constants::Texture texture, float posX, float posY, float rot, float scale) : 
	GameObject(texture, posX, posY, rot, scale)
{
}

UiObject::~UiObject()
{
}
