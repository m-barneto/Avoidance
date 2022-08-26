#pragma once
#include <SFML/Graphics.hpp>
#include "Agent.h"
using namespace sf;


const std::vector<Color> colors = {
    Color(26, 20, 35),
    Color(55, 37, 73),
    Color(119, 76, 96),
    Color(183, 93, 105),
    Color(234, 205, 194)
};

class GameArea {
private:
    int ticksPerSecond, width, height;
    double secondsPerTick;
    double timeIncrementer;
    Texture texture;
    Sprite sprite;
    std::vector<Agent> agents;
    std::vector<Uint8> pixels;

    Vector2i getPosWithDir(Agent& agent) {
        return getPosWithDir(agent, agent.dir);
    }
    Vector2i getPosWithDir(Agent& agent, int dir) {
        Vector2i offsetPos = Vector2i(agent.x, agent.y);
        switch (dir) {
        case 0:
            offsetPos.x += -1;
            offsetPos.y += -1;
            break;
        case 1:
            offsetPos.x += 0;
            offsetPos.y += -1;
            break;
        case 2:
            offsetPos.x += 1;
            offsetPos.y += -1;
            break;
        case 3:
            offsetPos.x += -1;
            offsetPos.y += 0;
            break;
        case 4:
            offsetPos.x += 1;
            offsetPos.y += 0;
            break;
        case 5:
            offsetPos.x += -1;
            offsetPos.y += 1;
            break;
        case 6:
            offsetPos.x += 0;
            offsetPos.y += 1;
            break;
        case 7:
            //printf("case 7\n");
            offsetPos.x += 1;
            offsetPos.y += 1;
            break;
        default:
            printf("Agent had invalid direction: %d\n", dir);
        }
        return offsetPos;
    }
    bool isValidPos(Image& img, Vector2i pos) {
        return !(pos.x < 0 || pos.x >= img.getSize().x || pos.y < 0 || pos.y >= img.getSize().y);
    }
public:
    GameArea(int ticksPerSecond, int width, int height) :
        ticksPerSecond{ ticksPerSecond },
        secondsPerTick{ 1.0 / ticksPerSecond },
        width{ width },
        height{ height },
        timeIncrementer{ 0.0 } {
        srand(time(NULL));
        RenderTexture spawnImage;
        spawnImage.create(width, height);
        CircleShape spawnCircle(width / 4, 1000);
        spawnCircle.setPosition(width / 4, height / 4);
        spawnCircle.setFillColor(Color::Black);
        spawnCircle.setOutlineThickness(1.f);
        spawnCircle.setOutlineColor(Color::White);

        spawnImage.clear(Color::Black);
        spawnImage.draw(spawnCircle);
        spawnImage.display();

        Image img = spawnImage.getTexture().copyToImage();
        // for each white pixel, set it to random color and add agent to vector
        const Uint8* pByteBuffer = img.getPixelsPtr();

        size_t numPixels = img.getSize().x * img.getSize().y;
        int white = 0;
        int black = 0;
        for (int i = 0; i < numPixels; ++i) {
            Uint8 red = pByteBuffer[4 * i];
            if (red == 255) {
                // white pixel, create agent for it
                Agent agent;
                agent.x = i / img.getSize().x;
                agent.y = i % img.getSize().y;
                agent.color = colors[rand() % colors.size()];
                agent.dir = rand() % 8;
                agents.push_back(agent);
            }
        }



        texture.loadFromImage(img);
        sprite.setTexture(texture);
    }

    void update(float deltaTime) {
        timeIncrementer += deltaTime;
        while (timeIncrementer >= secondsPerTick) {
            timeIncrementer -= secondsPerTick;

            // get the image from the texture
            Image img = texture.copyToImage();

            // move all agents
            // iterate backwards so we can erase in the same loop
            for (size_t i = agents.size() - 1; i > 0; i--) {
                bool skip = false;
                // collision/decision logic here
                Vector2i possiblePos = getPosWithDir(agents[i]);
                if (isValidPos(img, possiblePos) && img.getPixel(possiblePos.x, possiblePos.y) != Color::Black) {
                    // in trouble
                    bool turned = false;
                    Vector2i nextPos;
                    for (size_t r = 1; r < 4; r++) {
                        // try turning right/left 4 times
                        // check right
                        nextPos = getPosWithDir(agents[i], (agents[i].dir + r) % 8);

                        if (isValidPos(img, nextPos) && img.getPixel(nextPos.x, nextPos.y) == Color::Black) {

                            // do it!
                            agents[i].dir = (agents[i].dir + r) % 8;
                            turned = true;
                            skip = true;
                            break;
                        }
                        // check left
                        nextPos = getPosWithDir(agents[i], (agents[i].dir - r) % 8);
                        if (isValidPos(img, nextPos) && img.getPixel(nextPos.x, nextPos.y) == Color::Black) {
                            // do it!
                            agents[i].dir = (agents[i].dir - r) % 8;
                            turned = true;
                            skip = true;
                            break;
                        }
                    }
                } else {
                    // not in trouble :D
                }

                // movement here
                // directions 0-7
                // 
                //  012
                //  3*4
                //  567
                if (!skip) {
                    Vector2i posWithDir = getPosWithDir(agents[i]);
                    agents[i].x = posWithDir.x;
                    agents[i].y = posWithDir.y;
                }


                // Check if agents are in bounds of image
                if (skip || !isValidPos(img, Vector2i(agents[i].x, agents[i].y))) {
                    agents.erase(agents.begin() + i);
                }
            }

            // draw all agents
            for (size_t i = 0; i < agents.size(); i++) {
                img.setPixel(agents[i].x, agents[i].y, agents[i].color);
            }

            // copy image back to texture
            texture.loadFromImage(img);
        }
    }

    void render(RenderWindow& window) {
        window.draw(sprite);
    }
};