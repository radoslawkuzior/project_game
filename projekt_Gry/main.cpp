#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream> //TO TAKE DATA FROM A GAME ABOUT TIME AND PUT TO TO STATS.TXT BUT DOESNT WORK
#include <vector> //TO PUT DATA IN CORRECT WAY FROM HIGHER TO LOWER SCORE
#include <cmath> // FOR RANDOM STUFF ( COS AND SIN )
#include <ctime> // FOR RANDOM STUFF TOO

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float BALL_RADIUS = 10.0f;
const float PADDLE_THICKNESS = 10.0f;
const float PADDLE_LENGTH = 100.0f;
const float INITIAL_SPEED = 200.0f;
const float SPEED_INCREMENT = 10.0f;

enum Side { NONE, TOP, BOTTOM, LEFT, RIGHT };

struct ScoreEntry //for user name and his time but i dont know how to fix my issue

{

    string name;
    float time;

};

float randomAngle(float minimum_degree, float maximum_degree)

{
    float range = maximum_degree - minimum_degree;
    float randomDeg = minimum_degree + (rand() % 1000) / 1000.0f * range; // (rand() % 1000 ) give us some random number from 0 to 999
    float radians = randomDeg; // 1 degree give us 0.01745 radians
    return radians;
}



// function for our player name and time that he has
void drawText(sf::RenderWindow& window, string message, float x, float y, int size, sf::Color color)
{
    sf::Font font;
    if(!font.loadFromFile("arial.ttf"))
    {
        return;
    }
    sf::Text text;
    text.setFont(font);
    text.setString(message);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(x, y);
    window.draw(text);
}

void saveScore(const string& name, float timeSurvived) // i used some code from stackoverflow but still it doesn't work :(

{
    ofstream file("scores.txt", ios::app);

    if (file.is_open())
    {
        file << name << " " << timeSurvived << std::endl;
    }

}

vector<ScoreEntry> loadScores() // same thing
{
    vector<ScoreEntry> scores;

    ifstream file("scores.txt");

    if (file.is_open())
    {
        string name;
        float time;

        while (file >> name >> time)
        {
            ScoreEntry entry;
            entry.name = name;
            entry.time = time;
            scores.push_back(entry);
        }

        file.close();
    }
    return scores;
}
int main() {

    sf::Texture paddleTexture;
    if(!paddleTexture.loadFromFile("paddle_texture.jpg"))
    {
    }

    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "smth game");
    window.setFramerateLimit(60);

    string playerName;
    cout << "Enter your name: "; // doesnt work
    cin >> playerName;

    //here we create a ball

    sf::CircleShape ball(BALL_RADIUS);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(BALL_RADIUS, BALL_RADIUS);
    ball.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    //here we decide how ball should move

    sf::Vector2f velocity;
    float angle = (float)rand() / RAND_MAX;
    velocity.x = cos(angle);
    velocity.y = sin(angle);
    float speed = INITIAL_SPEED;

    //create our paddle

    sf::RectangleShape paddle;
    paddle.setTexture(&paddleTexture);
    paddle.setFillColor(sf::Color::White);
    float paddlePosition = 0.0f;

    //our timing

    sf::Clock clock;
    float timeSurvived = 0.0f;

    //our background color that we can change

    int red = 0, green = 0, blue = 0;

    Side chosenSide = TOP; // we set up our localization of paddle on top we can change it i

    bool paddleActive = true;
    paddle.setSize({ PADDLE_LENGTH, PADDLE_THICKNESS });
    paddlePosition = WINDOW_WIDTH / 2 + PADDLE_LENGTH / 2;
    paddle.setPosition(paddlePosition, 0);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::W:
                    chosenSide = TOP;
                    paddle.setSize({ PADDLE_LENGTH, PADDLE_THICKNESS });
                    paddlePosition = WINDOW_WIDTH / 2 - PADDLE_LENGTH / 2;
                    paddleActive = true;
                    break;
                case sf::Keyboard::S:
                    chosenSide = BOTTOM;
                    paddle.setSize({ PADDLE_LENGTH, PADDLE_THICKNESS });
                    paddlePosition = WINDOW_WIDTH / 2 - PADDLE_LENGTH / 2;
                    paddleActive = true;
                    break;
                case sf::Keyboard::A:
                    chosenSide = LEFT;
                    paddle.setSize({ PADDLE_THICKNESS, PADDLE_LENGTH });
                    paddlePosition = WINDOW_HEIGHT / 2 - PADDLE_LENGTH / 2;
                    paddleActive = true;
                    break;
                case sf::Keyboard::D:
                    chosenSide = RIGHT;
                    paddle.setSize({ PADDLE_THICKNESS, PADDLE_LENGTH });
                    paddlePosition = WINDOW_HEIGHT / 2 - PADDLE_LENGTH / 2;
                    paddleActive = true;
                    break;
                default:
                    break;
                }
            }
        }

        float dt = clock.restart().asSeconds();
        timeSurvived += dt;
        speed += SPEED_INCREMENT * dt;

        sf::Vector2f pos = ball.getPosition();
        pos += velocity * speed * dt;
        ball.setPosition(pos);

        if (paddleActive)
        {

            if (chosenSide == LEFT || chosenSide == RIGHT)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    paddlePosition = max(0.0f, paddlePosition - 300 * dt);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    paddlePosition = min(WINDOW_HEIGHT - PADDLE_LENGTH, paddlePosition + 300 * dt);

            } else

            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                    paddlePosition = max(0.0f, paddlePosition - 300 * dt);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    paddlePosition = min(WINDOW_WIDTH - PADDLE_LENGTH, paddlePosition + 300 * dt);
            }
        }

        if (paddleActive) {
            sf::FloatRect ballBounds = ball.getGlobalBounds();
            sf::FloatRect paddleBounds;

            switch (chosenSide) {
            case TOP: //TO MOVE PADDLE too different sites
                paddle.setPosition(paddlePosition, 0);
                paddleBounds = paddle.getGlobalBounds();

                if (ballBounds.intersects(paddleBounds)) {
                    float angle = randomAngle(90, 270);
                    velocity.x = cos(angle);
                    velocity.y = sin(angle);
                }
                break;

            case BOTTOM:
                // Place paddle at the bottom
                paddle.setPosition(paddlePosition, WINDOW_HEIGHT - PADDLE_THICKNESS);
                paddleBounds = paddle.getGlobalBounds();

                if (ballBounds.intersects(paddleBounds)) {
                    float angle = randomAngle(-90, 90);
                    velocity.x = cos(angle);
                    velocity.y = sin(angle);
                }
                break;

            case LEFT:
                paddle.setPosition(0, paddlePosition);
                paddleBounds = paddle.getGlobalBounds();
                if (ballBounds.intersects(paddleBounds)) {
                    float angle = randomAngle(-45, 45);
                    velocity.x = cos(angle);
                    velocity.y = sin(angle);
                }
                break;
            case RIGHT:
                paddle.setPosition(WINDOW_WIDTH - PADDLE_THICKNESS, paddlePosition);
                paddleBounds = paddle.getGlobalBounds();

                if (ballBounds.intersects(paddleBounds)) {
                    float angle = randomAngle(135, 225);
                    velocity.x = cos(angle);
                    velocity.y = sin(angle);
                }
                break;

            }
        }
        if (pos.x < 0 || pos.x > WINDOW_WIDTH || pos.y < 0 || pos.y > WINDOW_HEIGHT) {
            saveScore(playerName, timeSurvived);
            cout << "Game Over! Time survived: " << timeSurvived << " seconds\n";

            auto scores = loadScores();
            cout << "Score list:\n";
            for (const auto& s : scores)
                cout << s.name << " - " << s.time << " s\n";

            // Reset ball
            ball.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
            angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159265f;
            velocity = { cos(angle), sin(angle) };
            speed = INITIAL_SPEED;
            timeSurvived = 0.f;

            // Optional: Reset paddle
            paddleActive = false;
            chosenSide = NONE;
        }

        red = static_cast<int>((red + 30 * dt)) % 256;
        green = static_cast<int>((green + 60 * dt)) % 256;
        blue = static_cast<int>((blue + 90 * dt)) % 256;

        window.clear(sf::Color(red, green, blue));
        window.draw(ball);
        if (paddleActive)
            window.draw(paddle);

        drawText(window, "Time: " + to_string((int)timeSurvived), 10, 10, 25, sf::Color::White);
        drawText(window, "Player: " + playerName, 10, 40, 25, sf::Color::White);
        drawText(window, "", 10, 40, 25, sf::Color::White);


        window.display();
    }

    return 0;
}
