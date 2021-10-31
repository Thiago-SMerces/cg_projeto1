#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "pipes.hpp"
#include "bird.hpp"
#include "starlayers.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
	protected:
		void handleEvent(SDL_Event& event) override;
  		void initializeGL() override;
		void paintGL() override;
		void paintUI() override;
		void resizeGL(int width, int height) override;
		void terminateGL() override;

 	private:
	 	GLuint m_starsProgram{};
  		GLuint m_objectsProgram{};
  
		int m_viewportWidth{};
		int m_viewportHeight{};
		// bool flag = false;

		GameData m_gameData;

		Pipes m_pipes;
		Bird m_bird;
		StarLayers m_starLayers;

		abcg::ElapsedTimer m_restartWaitTimer;

		ImFont* m_font{};

		std::default_random_engine m_randomEngine;

		void restart();
		void update();
		void checkCollisions();
		void checkWinCondition();
		void scored();
};

#endif