#ifndef ASTEROIDS_HPP_
#define ASTEROIDS_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "bird.hpp"

class OpenGlWindow;

class Pipes {
    public:
        void initializeGL(GLuint program, int quantity);
        void paintGL();
        void terminateGL();

        void update(float deltaTime);

    private:
        friend OpenGLWindow;

        GLuint m_program{};
        GLint m_colorLoc{};
        GLint m_rotationLoc{};
        GLint m_translationLoc{};
        GLint m_scaleLoc{};

        struct Pipe {
            GLuint m_vao{};
            GLuint m_vbo{};

            float m_angularVelocity{};
            glm::vec4 m_color{33.0f / 255.0f, 172.0f / 255.0f, 33.0f / 255.0f, 1.0f};
            bool m_hit{false};
            int m_polygonSides{4};
            float m_rotation{};
            float m_scale{};
            float upper_y = 0.0f;
            float DIST = 0.0f;
            glm::vec2 m_translation{glm::vec2(0, -1)};
            glm::vec2 m_velocity{glm::vec2(0)};
        };

        std::vector<Pipe> m_lower_pipes;
        std::vector<Pipe> m_upper_pipes;
        
        std::default_random_engine m_randomEngine;
        std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

        Pipes::Pipe createPipe(float DIST_PIPES, float lower_x, float upper_x, float lower_y, float& upper_y, 
                                bool low_up, glm::vec2 translation = glm::vec2(0), float scale = 0.25f);
};

#endif