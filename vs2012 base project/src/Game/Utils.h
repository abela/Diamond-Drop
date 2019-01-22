#pragma once
#include <algorithm>
#include <string>
#include <stddef.h>
#include <memory>
#include <mutex>

using namespace std;




namespace Game
{
	class Utils
	{
	public:

		~Utils();

		Utils() = default;

		static Utils& Utils::Instance();

		static void Initsingleton();

		int GetRandomDiamondPlacementIndex(int previousColIndex, int previousRowIndex, int min, int max);

		int GetRandomIndexInInterval(int min, int max);

		bool IsMouseInSquare(float mousePosX, float mousePosY, float x, float y, int width, int height);

		string GetNiceTimeBasedOnValue(float timeValue);

		string GetSecondBasedOnValue(float timeValue);

	private:
		static std::unique_ptr <Utils> mUtilsInstance;
		static std::once_flag onceFlag;
		
	};
}




