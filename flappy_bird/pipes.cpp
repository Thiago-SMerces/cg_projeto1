#include "pipes.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Pipes::initializeGL(GLuint program, int quantity) {
    terminateGL();
    
    // Start pseudo-random number generator
    m_randomEngine.seed(
        std::chrono::steady_clock::now().time_since_epoch().count());
    
    m_program = program;
    m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
    m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
    m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
    m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

    // Create Pipes
    m_lower_pipes.clear();
    m_upper_pipes.clear();
    m_lower_pipes.resize(quantity);
    m_upper_pipes.resize(quantity);

    float DIST_PIPES = 1.0f;

    float lower_x = 0.0f;
    float upper_x = 0.5f;
    float lower_y = 4.f; // since scale is set to 0.25f
    float upper_y = 0.f;

    for (size_t i = 0; i < m_lower_pipes.size(); i++) {
            m_lower_pipes[i] = createPipe(DIST_PIPES, lower_x, upper_x, lower_y, upper_y, false);
            m_upper_pipes[i] = createPipe(DIST_PIPES, lower_x, upper_x, lower_y, upper_y, true);
            DIST_PIPES += 0.75f;
    }
}

void Pipes::paintGL() {
    abcg::glUseProgram(m_program);

    for (size_t i = 0; i < m_lower_pipes.size(); i++) {
        abcg::glBindVertexArray(m_lower_pipes[i].m_vao);
        abcg::glUniform4fv(m_colorLoc, 1, &m_lower_pipes[i].m_color.r);
        abcg::glUniform1f(m_scaleLoc, m_lower_pipes[i].m_scale);
        abcg::glUniform1f(m_rotationLoc, m_lower_pipes[i].m_rotation);
        abcg::glUniform2f(m_translationLoc, m_lower_pipes[i].m_translation.x, m_lower_pipes[i].m_translation.y);
        abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, m_lower_pipes[i].m_polygonSides + 2);
        abcg::glBindVertexArray(0);

        abcg::glBindVertexArray(m_upper_pipes[i].m_vao);
        abcg::glUniform4fv(m_colorLoc, 1, &m_upper_pipes[i].m_color.r);
        abcg::glUniform1f(m_scaleLoc, m_upper_pipes[i].m_scale);
        abcg::glUniform1f(m_rotationLoc, m_upper_pipes[i].m_rotation);
        abcg::glUniform2f(m_translationLoc, m_upper_pipes[i].m_translation.x, m_upper_pipes[i].m_translation.y);
        abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, m_upper_pipes[i].m_polygonSides + 2);
        abcg::glBindVertexArray(0);
    }
    
    abcg::glUseProgram(0);
}

void Pipes::terminateGL() {
    for (size_t i = 0; i < m_lower_pipes.size(); i++) {
        abcg::glDeleteBuffers(1, &m_lower_pipes[i].m_vbo);
        abcg::glDeleteVertexArrays(1, &m_lower_pipes[i].m_vao);
        abcg::glDeleteBuffers(1, &m_upper_pipes[i].m_vbo);
        abcg::glDeleteVertexArrays(1, &m_upper_pipes[i].m_vao);
    }
}

void Pipes::update(float deltaTime) {
    for (size_t i = 0; i < m_lower_pipes.size(); i++) {
        m_lower_pipes[i].m_translation.x -= 0.9 * deltaTime;
        m_upper_pipes[i].m_translation.x -= 0.9 * deltaTime;
    }
}

Pipes::Pipe Pipes::createPipe(float DIST_PIPES, float lower_x, float upper_x, float lower_y, float& upper_y, bool low_up, glm::vec2 translation, float scale) {
    Pipe pipe;

    auto &re{m_randomEngine}; // Shortcut

    // Randomly choose the number of sides
    pipe.m_polygonSides = 4;

    // Choose a color
    pipe.m_color = glm::vec4(33.0f / 255.0f, 172.0f / 255.0f, 33.0f / 255.0f, 1.0f);

    pipe.m_color.a = 1.0f;
    pipe.m_rotation = 0.0f;
    pipe.m_scale = scale;
    pipe.m_translation = translation;
    pipe.m_translation.x = DIST_PIPES;
    pipe.m_translation.y = 0;
    pipe.DIST = DIST_PIPES;

    if (low_up) {
        lower_y = 4.f;
        upper_y = static_cast<float>(1.5 + upper_y);
        pipe.upper_y = upper_y / 4;
    }
    else {
        std::uniform_real_distribution<float> randomUY(-3.f, 2.5f);
        upper_y = randomUY(re);
        pipe.upper_y = upper_y / 4;
        lower_y = -4.f;
    }
    // Create geometry
    std::vector<glm::vec2> positions {
        glm::vec2{lower_x, lower_y},
        glm::vec2{lower_x, upper_y},
        glm::vec2{upper_x, upper_y},
        glm::vec2{upper_x, lower_y},
        glm::vec2{lower_x, lower_y}
    };
    
    // Generate VBO
    abcg::glGenBuffers(1, &pipe.m_vbo);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, pipe.m_vbo);
    abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Get location of attributes in the program
    GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

    // Create VAO
    abcg::glGenVertexArrays(1, &pipe.m_vao);

    // Bind vertex attributes to current VAO
    abcg::glBindVertexArray(pipe.m_vao);

    abcg::glBindBuffer(GL_ARRAY_BUFFER, pipe.m_vbo);
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // End of binding to current VAO
    abcg::glBindVertexArray(0);

    return pipe;
}