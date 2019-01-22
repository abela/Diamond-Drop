// (C) king.com Ltd 2014

#pragma once

#include <glm/fwd.hpp>
#include <memory>
#include <mutex>
#include "Constants.h"
#include <iostream>
using namespace Game;

namespace King {
	class Updater;
	class Engine {
	public:
		
		Engine() = default;
		Engine(const char* assetsDirectory);
		~Engine();

		static void Initsingleton();

		static Engine& Engine::Instance();
		
		float GetLastFrameSeconds() const;
		float GetMouseX() const;
		float GetMouseY() const;
		bool GetMouseButtonDown() const;
		bool GetMouseButtonUp() const;
		void SetMouseButtonDownFlag(bool flag);
		bool GetMouseButton() const;
		int GetMouseRelativeX();
		int GetMouseRelativeY();
		bool MouseButtonDownFlag();
		
		void Start(Updater& updater);
		void Quit();

		int GetTextureHeight(Constants::Texture texture) const;
		int GetTextureWidth(Constants::Texture texture) const;
		void Render(Constants::Texture texture, const glm::mat4& transform, float scale = 1);
		void Render(Constants::Texture texture, float x, float y, float rotation = 0.0f, float scale = 1);


		float CalculateStringWidth(const char* text) const;
		void Write(const char* text, const glm::mat4& transform);
		void Write(const char* text, float x, float y, float rotation = 0.0f);

		int GetWidth() const;
		int GetHeight() const;

	private:
		//
		static std::unique_ptr <Engine> engineInstance;
		static std::once_flag onceFlag;
		//
		struct EngineImplementation;
		std::unique_ptr<EngineImplementation> mPimpl;
		//
		Engine(const Engine&) = delete;
		Engine & operator=(const Engine&) = delete;
		//
	};
}