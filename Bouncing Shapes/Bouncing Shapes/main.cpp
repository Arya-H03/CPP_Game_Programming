
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-sfml.h"
#include <iostream>
#include <fstream>




int main()
{
    const int wWidth = 1280;
    const int wHeight= 720;
    sf::RenderWindow window(sf::VideoMode({ wWidth, wHeight }), "SFML works!");
    window.setFramerateLimit(60);


    ImGui::SFML::Init(window);
    sf::Clock deltaclock;

    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    float c[3] = { 0.0f,1.0f,1.0f };

    float radius = 50;
    int segments = 32;
    float xSpeed = 1.0f;
    float ySpeed = 0.5f;
    bool drawCircle = true;
    bool drawText = true;

    sf::CircleShape circle(radius, segments);
    circle.setPosition({10.0f, 10.0f});

    sf::Font font;

    /*if (!font.openFromFile("file"))
    {
        std::cerr << "Font not found \n";
        exit(-1);

    }*/

    //sf::Text text("Sampletext", font, 24);

    while (window.isOpen())
    {
       

        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaclock.restart());
        
        ImGui::Begin("Window Title");
        ImGui::Text("Window Text");
        ImGui::Checkbox("Draw Circle", &drawCircle);
        ImGui::SameLine();
        ImGui::Checkbox("Draw Text", &drawText);
        ImGui::SliderFloat("Radius", &radius, 0.0f, 100.0f);
        ImGui::SliderInt("Sides", &segments, 3, 64);
        ImGui::ColorEdit3("Color", c);
        //ImGui::InputText("Text", displayString, 255);
        ImGui::End();

        circle.setPointCount(segments);
        circle.setRadius(radius);

        circle.setFillColor(sf::Color(std::uint8_t(c[0] * 255), std::uint8_t(c[1] * 255), std::uint8_t(c[2] * 255)));

        circle.setPosition({ circle.getPosition().x + xSpeed, circle.getPosition().y + ySpeed });

        window.clear();
        if (drawCircle)
        {
            window.draw(circle);
        }
       
        
        ImGui::SFML::Render(window);
        window.display();
    }
	return 0;
}