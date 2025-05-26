#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 550), "Arkanoid", sf::Style::Close);
    window.setFramerateLimit(120);

    sf::Texture windowTexture;
    windowTexture.loadFromFile("Res\\phon.png");

    sf::Sprite phon;
    phon.setTexture(windowTexture);
    phon.setColor(sf::Color(255, 255, 255, 52));

    sf::SoundBuffer buffer1;
    buffer1.loadFromFile("Res\\re-zero-return-by-death.mp3");
    sf::Sound sound1;
    sound1.setBuffer(buffer1);

    sf::SoundBuffer buffer2;
    buffer2.loadFromFile("Res\\victory.mp3");
    sf::Sound sound2;
    sound2.setBuffer(buffer2);

    sf::SoundBuffer buffer3;
    buffer3.loadFromFile("Res\\amet.mp3");
    sf::Sound sound3;
    sound3.setBuffer(buffer3);

    sf::Texture blockTexture;
    blockTexture.loadFromFile("Res\\block3.jfif");
    int col = 0;
    sf::RectangleShape block[24];
    for (int j = 1; j < 4; j++) {
        for (int i = 0; i < 8; i++) {
            block[col].setSize(sf::Vector2f(50, 50));
            block[col].setTexture(&blockTexture);
            block[col].setPosition(i * 50, j * 50);
            col += 1;
        }
    }

    sf::Texture playerTexture;
    playerTexture.loadFromFile("Res\\block2.png");
    sf::RectangleShape player(sf::Vector2f(100, 10));
    player.setFillColor(sf::Color::White);
    player.setPosition(150, 530);
    player.setTexture(&playerTexture);

    double moved = 5;
    double moved_b = 2.5;

    sf::CircleShape ball(10.f);
    sf::Texture B_T;
    B_T.loadFromFile("Res\\purpure.png");
    ball.setFillColor(sf::Color::Red);
    ball.setPosition(190, 510);
    ball.setTexture(&B_T);

    bool start = 0;
    double kx = -1, ky = -1;
    int win = 0;

    bool is_left = 0;
    bool is_right = 0;

    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2f pos = player.getPosition();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            start = 1;
        }
        if (start == 1 && win <= 24) {
            ball.move(moved_b * kx, moved_b * ky);
            sf::FloatRect ballBounds = ball.getGlobalBounds();
            if (ballBounds.left <= 0 || ballBounds.left + ballBounds.width >= 400) {
                sound3.play();
                kx = -kx;
            }
            if (ballBounds.top <= 0) {
                sound3.play();
                ky = -ky;
            }
            if (ballBounds.left + ballBounds.width >= pos.x && ballBounds.left <= pos.x + 100 && ballBounds.top == 510) {
                sound3.play();
                ky = -ky;
                if (is_left == 1) {
                    if (kx == 1.5) {
                        kx = -1;
                    }
                    else {
                        kx = -1.5;
                    }
                }
                else if (is_right == 1 && ky != 1) {
                    if (kx == -1.5) {
                        kx = 1;
                    }
                    else {
                        kx = 1.5;
                    }
                }

            }
            if (ballBounds.top > 560) {
                start = 0;
                player.setPosition(150, 530);
                ball.setPosition(190, 510);
                kx = -1;
                ky = -1;
                col = 0;
                for (int j = 1; j < 4; j++) {
                    for (int i = 0; i < 8; i++) {
                        block[col].setPosition(i * 50, j * 50);
                        col += 1;
                    }
                }
                sound1.play();
                win = 0;
            }
        }
        if (((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) &&
            (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))) ||
            (!(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) &&
                !(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))) {
            is_left = 0;
            is_right = 0;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (pos.x > 0) {
                player.move(-moved, 0);
                if (start == 0) {
                    ball.move(-moved, 0);
                }
                is_left = 1;
                is_right = 0;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (pos.x < 300) {
                player.move(moved, 0);
                if (start == 0) {
                    ball.move(moved, 0);
                }
                is_right = 1;
                is_left = 0;
            }
        }

        sf::Vector2f bl[24];
        for (int i = 0; i < 24; i++) {

            if (block[i].getPosition().y == 1000) {
                continue;
            }

            sf::FloatRect ballBounds = ball.getGlobalBounds();
            sf::FloatRect blockBounds = block[i].getGlobalBounds();

            if (ballBounds.intersects(blockBounds)) {

                float Left = ballBounds.left + ballBounds.width - blockBounds.left;
                float Right = blockBounds.left + blockBounds.width - ballBounds.left;
                float Top = ballBounds.top + ballBounds.height - blockBounds.top;
                float Bottom = blockBounds.top + blockBounds.height - ballBounds.top;

                float min = std::min({ Left, Right, Top, Bottom });

                if (min == Left || min == Right) {
                    sound3.play();
                    kx = -kx;
                }
                else {
                    sound3.play();
                    ky = -ky;
                }

                block[i].setPosition(1000, 0);
                win += 1;
                break;
            }
        }
        if (win == 24) {
            sound2.play();
            win += 1;
        }

        window.clear(sf::Color::Black);
        window.draw(phon);
        for (int i = 0; i < 24; i++) {
            window.draw(block[i]);
        }
        window.draw(player);
        window.draw(ball);
        window.display();
    }

    return 0;
}