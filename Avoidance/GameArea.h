#pragma once
#include <SFML/Graphics.hpp>
#include "Agent.h"
using namespace sf;

class GameArea {
private:
    int ticksPerSecond, width, height;
    double secondsPerTick;
    double timeIncrementer;
    Texture renderTexture;
    Sprite renderSprite;
    std::vector<Agent> agents;
    std::vector<Uint8> pixels;
public:
    GameArea(int ticksPerSecond, int width, int height) : 
        ticksPerSecond{ ticksPerSecond }, 
        secondsPerTick{1.0 / ticksPerSecond}, 
        width{ width }, 
        height{ height },
        timeIncrementer{ 0.0 } {
        
        RenderTexture spawnImage;
        spawnImage.create(width, height);
        CircleShape spawnCircle(width / 4, 1000);
        spawnCircle.setPosition(width / 4, height / 4);
        spawnCircle.setFillColor(Color::Black);
        spawnCircle.setOutlineThickness(1.f);
        spawnImage.draw(spawnCircle);
        spawnImage.clear(Color::White);
        spawnImage.display();

        pixels = std::vector<Uint8>();
        pixels.resize(width * height * 4);
        const Uint8* spawnPixels = spawnImage.getTexture().copyToImage().getPixelsPtr();
        memcpy(&pixels[0], spawnPixels, width * height * 4);


        renderTexture = Texture();
        renderTexture.create(width, height);
        renderTexture.update(&pixels[0]);
        renderSprite.setTexture(renderTexture);

        for (int i = 0; i < 1000; i++) {

        }
    }

    void update(float deltaTime) {
        timeIncrementer += deltaTime;
        while (timeIncrementer >= secondsPerTick) {
            timeIncrementer -= secondsPerTick;
            printf("Tick\n");
        }
    }

    void render(RenderWindow& window) {
        window.draw(renderSprite);
    }
};