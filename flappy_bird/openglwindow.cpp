#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define QTD 100

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height) {
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

void OpenGLWindow::handleEvent(SDL_Event &event) {
    // Keyboard events
    if (event.type == SDL_KEYDOWN) {
      	if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
        	m_gameData.m_input.set(static_cast<size_t>(Input::Up));
      	if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
        	m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    }
    if (event.type == SDL_KEYUP) {
      	if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
        	m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
      	if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
        	m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    }
	// Mouse events
	if (event.type == SDL_MOUSEBUTTONDOWN) {
    	if (event.button.button == SDL_BUTTON_LEFT)
        	m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    	if (event.button.button == SDL_BUTTON_RIGHT)
      		m_gameData.m_input.set(static_cast<size_t>(Input::Down));
  	}
  	if (event.type == SDL_MOUSEBUTTONUP) {
    	if (event.button.button == SDL_BUTTON_LEFT)
      		m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    	if (event.button.button == SDL_BUTTON_RIGHT)
      		m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
  	}
}

void OpenGLWindow::initializeGL() {
  	// Load a new font
  	ImGuiIO &io{ImGui::GetIO()};
  	auto filename{getAssetsPath() + "Inconsolata_Medium.ttf"};
  	m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 22.0f);
  	if (m_font == nullptr) {
    	throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  	}

  	// Create program to render the other objects
  	m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert", 
                                        	 getAssetsPath() + "objects.frag");
	m_starsProgram = createProgramFromFile(getAssetsPath() + "stars.vert", 
                                           getAssetsPath() + "stars.frag");
  	abcg::glClearColor(51.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.0f);
	abcg::glClear(GL_COLOR_BUFFER_BIT);
	abcg::glEnable( GL_BLEND );
	abcg::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  	#if !defined(__EMSCRIPTEN__)
    	abcg::glEnable(GL_PROGRAM_POINT_SIZE);
  	#endif

  	// Start pseudo-random number generator
  	m_randomEngine.seed(
    		std::chrono::steady_clock::now().time_since_epoch().count());
  	restart();
}

void OpenGLWindow::restart() {
  	m_gameData.m_state = State::Rest;
	m_gameData.hs = false;
	m_gameData.n_passed_pipes = 0;
	m_starLayers.initializeGL(m_starsProgram, 25);
  	m_bird.initializeGL(m_objectsProgram);
	m_pipes.initializeGL(m_objectsProgram, QTD);
}

void OpenGLWindow::update() {
  	float deltaTime{static_cast<float>(getDeltaTime())};

  	// Wait 5 seconds before restarting
  	if ((m_gameData.m_state == State::GameOver || m_gameData.m_state == State::Win) && m_restartWaitTimer.elapsed() > 5) {
    	restart();
    	return;
  	}
	else if(m_gameData.m_state == State::Rest && m_gameData.m_input[static_cast<size_t>(Input::Up)]) {
		m_gameData.m_state = State::Playing;
	}

	else if(m_gameData.m_state == State::Playing) {
	  	m_bird.update(m_gameData, deltaTime);
		m_starLayers.update(m_bird, deltaTime);
		m_pipes.update(deltaTime);
		checkCollisions();
		scored();
		checkWinCondition();
	}
}

void OpenGLWindow::paintGL() {
  	update();

  	abcg::glClear(GL_COLOR_BUFFER_BIT);
  	abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);
	
	m_starLayers.paintGL();
	m_pipes.paintGL();
  	m_bird.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
	int my_image_width = 0;
	int my_image_height = 0;
	GLuint my_image_texture = 0;
	auto image_path = getAssetsPath() + "leafforest.png";
	bool ret = LoadTextureFromFile(image_path.c_str(), &my_image_texture, &my_image_width, &my_image_height);
	IM_ASSERT(ret);
  	abcg::OpenGLWindow::paintUI();
  	{
		const auto size{ImVec2(700, 350)};
		const auto position{ImVec2(0, 0)};
    
    	ImGui::SetNextWindowPos(position);
    	ImGui::SetNextWindowSize(size);
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    	ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground | 
                            	ImGuiWindowFlags_NoTitleBar |
                            	ImGuiWindowFlags_NoInputs};
		ImGui::Begin(" ", nullptr, flags);
		ImGui::GetBackgroundDrawList()->AddImage((void*)(intptr_t)my_image_texture, ImVec2(-1, -1), ImVec2(-1, 1));
		// ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));

		ImGui::PushFont(m_font);

    	if (m_gameData.m_state == State::GameOver) {
      		ImGui::Text("*Game Over*");
      		ImGui::Text("Final Score: %d", m_gameData.final_score);

			if (m_gameData.hs) {
      			ImGui::Text("*New High Score!*");
			}
			m_gameData.score = 0;
    	}
    	else if (m_gameData.m_state == State::Rest || m_gameData.m_state == State::Playing) {
			ImGui::Text("High Score: %d", m_gameData.high_score);
      		ImGui::Text("Score: %d", m_gameData.score);
    	}
		else if (m_gameData.m_state == State::Win) {
      		ImGui::Text("*Congratulations!! You Win!*");
			ImGui::Text("Final Score: %d", m_gameData.final_score);
			
    	}

    	ImGui::PopFont();
    	ImGui::End();
  	}
}

void OpenGLWindow::resizeGL(int width, int height) {
  	m_viewportWidth = width;
  	m_viewportHeight = height;

  	abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  	abcg::glDeleteProgram(m_objectsProgram);

  	m_bird.terminateGL();
	m_pipes.terminateGL();
	m_starLayers.terminateGL();
}

void OpenGLWindow::checkCollisions() {
	// Careful not to reach screen limits
    if (m_bird.m_translation.x < -1.0f || m_bird.m_translation.x > +1.0f ||
		m_bird.m_translation.y < -0.95f || m_bird.m_translation.y > +0.95f) {
			m_gameData.m_state = State::GameOver;
			m_gameData.final_score = m_gameData.score;
			if(m_gameData.score > m_gameData.high_score) {
				m_gameData.high_score = m_gameData.score;
				m_gameData.hs = true;
			}
			m_restartWaitTimer.restart();
	}

	// Check collisions for bird and pipes
	int i = m_gameData.n_passed_pipes;
	float dist = 0.04f;
	do {
		if (i != m_gameData.n_passed_pipes) {
			const auto ldistance {glm::distance(m_bird.m_translation.x, m_pipes.m_lower_pipes[i].m_translation.x)};
			const auto udistance {glm::distance(m_bird.m_translation.x, m_pipes.m_upper_pipes[i].m_translation.x)};
			if ((ldistance < m_bird.m_scale * 0.85f + m_pipes.m_lower_pipes[i].m_scale * 0.85f 
				&& (m_bird.m_translation.y < m_pipes.m_lower_pipes[i].upper_y)) ||
				(udistance < m_bird.m_scale * 0.85f + m_pipes.m_upper_pipes[i].m_scale * 0.85f
				&& (m_bird.m_translation.y < m_pipes.m_lower_pipes[i].upper_y))) {
					m_gameData.m_state = State::GameOver;
					m_gameData.final_score = m_gameData.score;
					if (m_gameData.score > m_gameData.high_score) {
						m_gameData.high_score = m_gameData.score;
						m_gameData.hs = true;
					}
					m_restartWaitTimer.restart();
			}
		}

		const auto lower_distanceX = m_pipes.m_lower_pipes[i].m_translation.x;
		const auto upper_distanceX = m_pipes.m_upper_pipes[i].m_translation.x;

		if (((lower_distanceX > -dist && lower_distanceX < dist) && (m_bird.m_translation.y < m_pipes.m_lower_pipes[i].upper_y))
			|| ((upper_distanceX > -dist && upper_distanceX < dist) && (m_bird.m_translation.y > m_pipes.m_upper_pipes[i].upper_y))) {
			m_gameData.m_state = State::GameOver;
			m_gameData.final_score = m_gameData.score;
			if (m_gameData.score > m_gameData.high_score) {
				m_gameData.high_score = m_gameData.score;
				m_gameData.hs = true;
			}
			m_restartWaitTimer.restart();
		}
		i -= 1;
	} while (i >= 0 && i >= m_gameData.n_passed_pipes - 1);

}

// void OpenGLWindow::checkOut(bool &flag) {
// 	float upper_y = 0.0f;
// 	for (size_t i = 0; i < m_pipes.m_lower_pipes.size(); i++) {
// 		if (m_pipes.m_lower_pipes[i].m_translation.x < -1.0f) {
// 			std::generate_n(std::back_inserter(m_pipes.m_lower_pipes), 1, [&]() {
// 					return m_pipes.createPipe(
// 					m_pipes.m_lower_pipes[m_pipes.m_lower_pipes.size() - 1].DIST + (0.25f * 0.25f), 0.0f, 0.5f, 0.0f, upper_y, flag);
// 				});
// 			m_pipes.m_lower_pipes.erase(m_pipes.m_lower_pipes.begin());
// 			flag = !flag;
// 			std::generate_n(std::back_inserter(m_pipes.m_upper_pipes), 1, [&]() {
// 					return m_pipes.createPipe(
// 					m_pipes.m_upper_pipes[m_pipes.m_upper_pipes.size() - 1].DIST + (0.25f * 0.25f), 0.0f, 0.5f, 0.0f, upper_y, flag);
// 				});
// 			m_pipes.m_upper_pipes.erase(m_pipes.m_upper_pipes.begin());
// 			flag = !flag;
// 		}
// 	}
// }

void OpenGLWindow::scored() {
	if (m_pipes.m_lower_pipes[m_gameData.n_passed_pipes].m_translation.x < 0.04f) {
		m_gameData.score += 5;
		m_gameData.n_passed_pipes += 1;
	}
}

void OpenGLWindow::checkWinCondition() {
	if (m_gameData.n_passed_pipes == QTD) {
		m_gameData.m_state = State::Win;
		m_gameData.final_score = m_gameData.score;
		m_restartWaitTimer.restart();
		m_restartWaitTimer.restart();
	}
}