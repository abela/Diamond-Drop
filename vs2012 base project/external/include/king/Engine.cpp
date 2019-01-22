#include "Engine.h"

#include <stdexcept>
#include <algorithm>
#include <vector>

#define GLM_FORCE_RADIANS 
#include <glm/gtc/matrix_transform.hpp>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <sdl/Sdl.h>

#include "Font.h"
#include "GlContext.h"
#include "Sdl.h"
#include "SdlWindow.h"
#include "SdlSurface.h"
#include "Updater.h"


namespace King {

	std::unique_ptr<Engine> Engine::engineInstance;
	std::once_flag Engine::onceFlag;

	static const int WindowWidth = 755;
	static const int WindowHeight = 600;
	static const float MaxFrameTicks = 300.0f;
	static const float TextScale = 0.5f;

	struct Engine::EngineImplementation {
		Sdl mSdl;
		SdlWindow mSdlWindow;
		GlContext mGlContext;
		std::unique_ptr<SdlSurface> mSdlSurfaceContainer[Constants::Texture::TEXTURE_MAX];
		std::unique_ptr<SdlSurface> mFontSdlSurface;
		float mElapsedTicks;
		float mLastFrameSeconds;
		Updater* mUpdater;
		bool mQuit;

		float mMouseX;
		float mMouseY;
		bool mMouseButton;
		bool mMouseButtonDown;
		bool mMouseButtonUp;
		bool mouseButtonDownFlag;
		int mMouseRelativeX;
		int mMouseRelativeY;
		
		EngineImplementation()
			: mSdl(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE)
			, mSdlWindow(WindowWidth, WindowHeight)
			, mGlContext(mSdlWindow)
			, mLastFrameSeconds(1.0f / 60.0f)
			, mMouseX(WindowWidth * 0.5f)
			, mMouseY(WindowHeight * 0.5f)
			, mMouseButtonDown(false)
			, mouseButtonDownFlag(false)
			, mMouseButtonUp(false)
			, mMouseButton(false)
			, mQuit(false)
			, mUpdater(nullptr)
			, mElapsedTicks(static_cast<float>(SDL_GetTicks()))
		{
		}

		void Start();
		void ParseEvents();
	};

	Engine::Engine(const char* assetsDirectory)
		: mPimpl(new EngineImplementation) {
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetSwapInterval(1);

		glEnable(GL_TEXTURE_2D);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		std::string assets(assetsDirectory);
		std::string background = assets; background += "/BackGround.jpg";
		std::string backgroundHover = assets; backgroundHover += "/BcHover.jpg";
		std::string blue = assets; blue += "/Blue.png";
		std::string green = assets; green += "/Green.png";
		std::string purple = assets; purple += "/Purple.png";
		std::string red = assets; red += "/Red.png";
		std::string yellow = assets; yellow += "/Yellow.png";
		std::string diamondSelection = assets; diamondSelection += "/square.png";
		std::string restart = assets; restart += "/restart.png";
		std::string menuBc = assets; menuBc += "/MenuBc.jpg";
		std::string menuTitle = assets; menuTitle += "/Menu_Title.png";
		std::string menuStartGame = assets; menuStartGame += "/Menu_Start.png";
		std::string menuAboutGame = assets; menuAboutGame += "/Menu_About.png";
		std::string menuQuitGame = assets; menuQuitGame += "/Menu_Quit.png";
		std::string menuGoBack = assets; menuGoBack += "/Go-back-icon.png";
		std::string font = assets; font += "/berlin_sans_demi_72_0.png";

		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_BACKGROUND].reset(new SdlSurface(background.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_BACKGROUND_HOVER].reset(new SdlSurface(backgroundHover.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_BLUE].reset(new SdlSurface(blue.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_GREEN].reset(new SdlSurface(green.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_PURPLE].reset(new SdlSurface(purple.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_RED].reset(new SdlSurface(red.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_YELLOW].reset(new SdlSurface(yellow.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_DIAMOND_SELECTION].reset(new SdlSurface(diamondSelection.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_RESTART].reset(new SdlSurface(restart.c_str()));

		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_MENU_BC].reset(new SdlSurface(menuBc.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_MENU_TITLE].reset(new SdlSurface(menuTitle.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_START_GAME].reset(new SdlSurface(menuStartGame.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_ABOUT_GAME].reset(new SdlSurface(menuAboutGame.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_QUIT_GAME].reset(new SdlSurface(menuQuitGame.c_str()));
		mPimpl->mSdlSurfaceContainer[Constants::Texture::TEXTURE_GO_BACK].reset(new SdlSurface(menuGoBack.c_str()));

		mPimpl->mFontSdlSurface.reset(new SdlSurface(font.c_str()));

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, WindowWidth, WindowHeight, 0.0f, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);

	}

	Engine::~Engine() {
	}

	void Engine::Initsingleton()
	{
		std::cout << "Initializing engine singleton instance" << '\n';
	}

	Engine & Engine::Instance()
	{
		std::call_once(Engine::onceFlag, []() {
			Initsingleton();
			engineInstance.reset(new Engine(Constants::ASSETS_DIR));
		});
		return *(engineInstance.get());
	}

	float Engine::GetLastFrameSeconds() const {
		return mPimpl->mLastFrameSeconds;
	}

	float Engine::GetMouseX() const {
		return mPimpl->mMouseX;
	}

	float Engine::GetMouseY() const {
		return mPimpl->mMouseY;
	}

	int Engine::GetMouseRelativeX()
	{
		return mPimpl->mMouseRelativeX;
	}

	int Engine::GetMouseRelativeY()
	{
		return mPimpl->mMouseRelativeY;
	}

	bool Engine::GetMouseButton() const {
		return mPimpl->mMouseButton;
	}

	bool Engine::GetMouseButtonDown() const
	{
		return mPimpl->mMouseButtonDown;
	}

	bool Engine::GetMouseButtonUp() const {
		return mPimpl->mMouseButtonUp;
	}

	void Engine::SetMouseButtonDownFlag(bool flag)
	{
		mPimpl->mouseButtonDownFlag = flag;
	}

	bool Engine::MouseButtonDownFlag()
	{
		return mPimpl->mouseButtonDownFlag;
	}
	
	void Engine::Quit() {
		mPimpl->mQuit = true;
	}

	void Engine::Start(Updater& updater) {
		mPimpl->mUpdater = &updater;
		mPimpl->mSdlWindow.Show();
		mPimpl->Start();
	}

	int Engine::GetTextureHeight(Constants::Texture texture) const {
		return mPimpl->mSdlSurfaceContainer[texture]->Height();
	}

	int Engine::GetTextureWidth(Constants::Texture texture) const {
		return mPimpl->mSdlSurfaceContainer[texture]->Width();
	}

	void Engine::Render(Constants::Texture texture, const glm::mat4& transform, float scale) {
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));

		SdlSurface& surface = *mPimpl->mSdlSurfaceContainer[texture];

		surface.Bind();

		glBegin(GL_QUADS);
		glTexCoord2i(0, 1); glVertex2i((GLint)(surface.Width() * (1 - scale)), (GLint)(surface.Height() * scale));
		glTexCoord2i(1, 1); glVertex2i((GLint)(surface.Width() * scale), (GLint)(surface.Height() * scale));
		glTexCoord2i(1, 0); glVertex2i((GLint)(surface.Width() * scale), (GLint)(surface.Height() * (1 - scale)));
		glTexCoord2i(0, 0); glVertex2i((GLint)(surface.Width() * (1 - scale)), (GLint)(surface.Height() * (1 - scale)));
		glEnd();
	}

	void Engine::Render(Constants::Texture texture, float x, float y, float rotation, float scale) {
		glm::mat4 transformation;
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
		if (rotation) {
			transformation = glm::rotate(transformation, rotation, glm::vec3(0.0, 0.0f, 1.0f));
		}
		Render(texture, transformation,scale);
	}

	Glyph& FindGlyph(char c) {
		auto found = std::lower_bound(std::begin(Font), std::end(Font), c);
		if (found == std::end(Font) || c < *found) {
			found = std::lower_bound(std::begin(Font), std::end(Font), static_cast<int>('_'));
		}
		return *found;
	}

	float Engine::CalculateStringWidth(const char* text) const {
		int advance = 0;
		for (; *text; ++text) {
			Glyph& g = FindGlyph(*text);
			advance += g.advance;
		}
		return advance*TextScale;
	}

	void Engine::Write(const char* text, const glm::mat4& transform) {
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		int advance = 0;
		for (; *text;++text) {
			Glyph& g = FindGlyph(*text);

			float fontTexWidth  = static_cast<float>(mPimpl->mFontSdlSurface->Width());
			float fontTexHeight = static_cast<float>(mPimpl->mFontSdlSurface->Height());

			float uvLeft = static_cast<float>(g.x) / fontTexWidth;
			float uvRight = static_cast<float>(g.x + g.width) / fontTexWidth;
			float uvBottom = static_cast<float>(g.y) / fontTexHeight;
			float uvTop = static_cast<float>(g.y + g.height) / fontTexHeight;

			float worldLeft = static_cast<float>(g.xoffset + advance);
			float worldRight = static_cast<float>(g.xoffset + g.width + advance);
			float worldBottom = static_cast<float>(g.yoffset);
			float worldTop = static_cast<float>(g.yoffset + g.height);

			mPimpl->mFontSdlSurface->Bind();

			glBegin(GL_QUADS);
			glTexCoord2f(uvLeft / 2.0f, uvTop / 2.0f); glVertex2f(worldLeft * TextScale, worldTop * TextScale);
			glTexCoord2f(uvRight / 2.0f, uvTop / 2.0f); glVertex2f(worldRight * TextScale, worldTop * TextScale);
			glTexCoord2f(uvRight / 2.0f, uvBottom / 2.0f); glVertex2f(worldRight * TextScale, worldBottom * TextScale);
			glTexCoord2f(uvLeft / 2.0f, uvBottom / 2.0f); glVertex2f(worldLeft * TextScale, worldBottom * TextScale);
			glEnd();
			advance += g.advance;
		}
	}

	void Engine::Write(const char* text, float x, float y, float rotation) {
		glm::mat4 transformation;
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
		if (rotation) {
			transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			transformation = glm::translate(transformation, glm::vec3(-CalculateStringWidth(text)/2.0f, -20.0f, 0.0f));
		}
		Write(text, transformation);
	}

	int Engine::GetWidth() const {
		return WindowWidth;
	}

	int Engine::GetHeight() const {
		return WindowHeight;
	}

	void Engine::EngineImplementation::Start() {
		while (!mQuit) {
			SDL_GL_SwapWindow(mSdlWindow);
			glClear(GL_COLOR_BUFFER_BIT);

			ParseEvents();
			
			float currentTicks = static_cast<float>(SDL_GetTicks());
			float lastFrameTicks = currentTicks - mElapsedTicks;
			mElapsedTicks = currentTicks;

			lastFrameTicks = std::min(lastFrameTicks, MaxFrameTicks);
			mLastFrameSeconds = lastFrameTicks * 0.001f;

			if (mUpdater) {
				mUpdater->Update();
			}
		}
	}

	void Engine::EngineImplementation::ParseEvents() {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				//mQuit = true;
				Engine::Instance().Quit();
				break;
			case SDL_MOUSEBUTTONDOWN:
				mMouseButton = true;
				if (event.button.state == SDL_PRESSED)
				{
					if(event.button.clicks == 1)
					{
						mMouseButtonUp = false;
						mMouseButtonDown = true;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.clicks == 1)
				{
					mMouseButton = false;
					mMouseButtonUp = true;
					mMouseButtonDown = false;
					mouseButtonDownFlag = false;
				}
				break;
			case SDL_MOUSEMOTION:
				mMouseX = static_cast<float>(event.motion.x);
				mMouseY = static_cast<float>(event.motion.y);
				//
				mMouseRelativeX = event.motion.xrel;
				mMouseRelativeY = event.motion.yrel;
				//
				//printf("%d\n", mMouseRelativeX);
				//
				break;
			default:
				break;
			}
		}
	}
}
