#include "Utils.h"
#include <random>
#include <iostream>


using namespace Game;


std::unique_ptr<Utils> Utils::mUtilsInstance;
std::once_flag Utils::onceFlag;

Utils::~Utils()
{
}

void Utils::Initsingleton()
{
	std::cout << "Initializing utils singleton" << '\n';
}

Utils & Utils::Instance()
{
	std::call_once(Utils::onceFlag, []() {
		Initsingleton();
		mUtilsInstance.reset(new Utils);
	});
	return *(mUtilsInstance.get());
}

// get random index, if index is equal to previous index regenerate it
int Utils::GetRandomDiamondPlacementIndex(int previousColIndex, int previousRowIndex, int min, int max)
{
	int randomIndex = GetRandomIndexInInterval(min, max);
	if (randomIndex == previousColIndex || randomIndex == previousRowIndex)
		return GetRandomDiamondPlacementIndex(previousColIndex, previousRowIndex, min, max);
	return randomIndex;
}

	
int Utils::GetRandomIndexInInterval(int min, int max)
{
	int randomIndex = 0;
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::minstd_rand::result_type> dist6(min, max);
	//
	return dist6(rng);
}

bool Utils::IsMouseInSquare(float mousePosX, float mousePosY, float x, float y, int width, int height)
{
	return ((mousePosX > x) && (mousePosX < x + width) && (mousePosY > y) && (mousePosY < y + height));
}

string Utils::GetNiceTimeBasedOnValue(float timeValue)
{
	string minutes = to_string((int)(timeValue / 60)) + " : ";
	string seconds = to_string((int)(timeValue));
	//
	minutes += seconds;
	return minutes;
}

string Utils::GetSecondBasedOnValue(float timeValue)
{
	string seconds = to_string((int)(timeValue));
	//
	seconds;
	return seconds;
}


