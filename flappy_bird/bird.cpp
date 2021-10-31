#include "bird.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <cppitertools/itertools.hpp>

void Bird::initializeGL(GLuint program) {
    terminateGL();

    m_program = program;
    m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
    m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
    m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
    m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

    m_rotation = 0.0f;
    m_translation = glm::vec2(0);
    m_velocity = glm::vec2(0);

    std::vector<glm::vec2> positions(0);
    positions.emplace_back(0, 0);
    const auto sides{20};
    const auto step{M_PI * 4 / sides};
    for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
        positions.emplace_back(std::cos(angle), std::sin(angle));
    }
    positions.push_back(positions.at(1));

    // Generate VBO
    abcg::glGenBuffers(1, &m_vbo);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), 
                        positions.data(), GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Get location of attributes in the program
    GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

    // Create VAO
    abcg::glGenVertexArrays(1, &m_vao);

    // Bind vertex attributes to current VAO
    abcg::glBindVertexArray(m_vao);

    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                                nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // End of binding to current VAO
    abcg::glBindVertexArray(0);
}

void Bird::paintGL(const GameData &gameData) {
    if (gameData.m_state == State::GameOver) return;

    abcg::glUseProgram(m_program);

    abcg::glBindVertexArray(m_vao);

    abcg::glUniform1f(m_scaleLoc, m_scale);
    abcg::glUniform1f(m_rotationLoc, m_rotation);  
    abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

    abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 12);

    abcg::glBindVertexArray(0);

    abcg::glUseProgram(0);
}

void Bird::terminateGL() {
    abcg::glDeleteBuffers(1, &m_vbo);
    abcg::glDeleteBuffers(1, &m_ebo);
    abcg::glDeleteVertexArrays(1, &m_vao);
}

void Bird::update(const GameData &gameData, float deltaTime) {
    // Fly up/down
    if (gameData.m_input[static_cast<size_t>(Input::Up)] && 
        (gameData.m_state == State::Rest || gameData.m_state == State::Playing)) {
            m_translation.y += 1.7f*deltaTime;
        }
    else if (gameData.m_state == State::Playing) {
        if (gameData.m_input[static_cast<size_t>(Input::Down)]) {
            m_translation.y -= 1.7f*deltaTime;
        }
		else {
			deltaTime += 0.01f;
            m_translation.y -= 0.09*deltaTime;
        }
    }
}