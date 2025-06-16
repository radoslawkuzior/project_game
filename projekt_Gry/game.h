#ifndef GAME_H
#define GAME_H
#endif // GAME_H


#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum Side { NONE, TOP, BOTTOM, LEFT, RIGHT };

struct ScoreEntry {
    std::string name;
    float time;
};

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void handleInput(sf::Keyboard::Key key);
    void checkCollision();
    void endGame();
    void drawText(const std::string& str, float x, float y, int size = 35, sf::Color color = sf::Color::White);
    float randomAngle(float minDeg, float maxDeg);
    void saveScore(const std::string& name, float timeSurvived);
    std::vector<ScoreEntry> loadScores();

    sf::RenderWindow window;
    sf::CircleShape ball;
    sf::RectangleShape paddle;
    sf::Vector2f velocity;

    sf::Clock clock;
    float speed;
    float timeSurvived;
    float paddlePosition;
    bool paddleActive;

    std::string playerName;
    int red, green, blue;
    Side chosenSide;

    // Constants
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const float BALL_RADIUS = 10.0f;
    const float PADDLE_THICKNESS = 10.0f;
    const float PADDLE_LENGTH = 100.0f;
    const float INITIAL_SPEED = 200.0f;
    const float SPEED_INCREMENT = 10.0f;
};
