#ifndef BIRD_HPP_
#define BIRD_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class Pipes;
class OpenGLWindow;
class StarLayers;

class Bird {
    public:
        void initializeGL(GLuint program);
        void paintGL(const GameData &gameData);
        void terminateGL();

        void update(const GameData &gameData, float deltaTime);
        void setRotation(float rotation) {m_rotation = rotation;};

        private:
            friend Pipes;
            friend OpenGLWindow;
            friend StarLayers;

            GLuint m_program{};
            GLint m_translationLoc{};
            GLint m_colorLoc{};
            GLint m_scaleLoc{};
            GLint m_rotationLoc{};

            GLuint m_vao{};
            GLuint m_vbo{};
            GLuint m_ebo{};

            glm::vec4 m_color{213.0f / 255.0f, 51.0f / 255.0f, 83.0f / 255.0f, 1.0f};
            float m_rotation{};
            float m_scale{0.045f};
            glm::vec2 m_translation{glm::vec2(0)};
            glm::vec2 m_velocity{glm::vec2(0)};

            float y_pos = 0;

            abcg::ElapsedTimer m_trailBlinkTimer;
};
#endif