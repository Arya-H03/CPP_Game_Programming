
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-sfml.h"
#include <iostream>
#include <fstream>

class MyShape
{
    sf::Shape* m_shape = nullptr;
    std::string m_name;
    std::array<float, 2> m_size;       // {width, height}
    std::array<float, 2> m_speed; // {xSpeed, ySpeed}
    std::array<float, 2> m_pos;   // {xPos, yPos}
    std::array<int, 3> m_color;      // {r, g, b}

public:

    MyShape(std::string name, std::array<float, 2> size, std::array<float, 2> startSpeed, std::array<float, 2> startPos, std::array<int, 3> color)

        : m_name(name), m_size(size), m_speed(startSpeed), m_pos(startPos), m_color(color)
     {
        if (m_name == "Circle")
        {
            m_shape = new sf::CircleShape(m_size[0],32);
        }
        else if (m_name == "Rectangle")
        {
            m_shape = new sf::RectangleShape({size[0],size[1]});
        }
        else
        {
            m_shape = nullptr;
        }

     }

     sf::Shape* GetShape() const
     {
         if (m_shape != nullptr)
         {
             return m_shape;
         }
         else
         {
             std::cout << "Invalid shape was created with name " << m_name;
             return nullptr;
         }
        
     }

     std::array <float,2> GetSpeed()
     {
         return m_speed;
     }

     std::array <float, 2> GetSize()
     {
         return m_size;
     }

     std::array <float, 2> GetPosition()
     {
         return m_pos;
     }

     std::array <int, 3> GetColor()
     {
         return m_color;
     }

     std::string GetName()
     {
         return m_name;
     }

     void ChangeSize(std::array<float, 2> newSize)
     {
         m_size = newSize;
        
     }

     void ChangeSpeed(std::array<float, 2> newSpeed)
     {
         m_speed = newSpeed;
     }

     void ChangePosotion(std::array<float, 2> newPos)
     {
         m_pos = newPos;
         m_shape->setPosition({ m_pos[0],m_pos[1] });
     }

     void ChangeColor(std::array<int, 3> newColor)
     {
         m_color = newColor;
         m_shape->setFillColor(sf::Color(std::uint8_t(m_color[0] * 255), std::uint8_t(m_color[1] * 255), std::uint8_t(newColor[2] * 255)));
     }

     void MoveShape()
     {
         m_pos[0] += m_speed[0];
         m_pos[1] += m_speed[1];
         m_shape->setPosition({ m_pos[0], m_pos[1] });
     }

     void CheckForBounce(sf::RenderWindow *window, float* xSpeed, float* ySpeed)
     {
         if (m_shape->getGlobalBounds().getCenter().y - m_shape->getGlobalBounds().size.y / 2 <= 0)
        {
            *ySpeed *= -1;
        }
        if (m_shape->getGlobalBounds().getCenter().y + m_shape->getGlobalBounds().size.y / 2 >= window->getSize().y)
        {
            *ySpeed *= -1;
        }

        if (m_shape->getGlobalBounds().getCenter().x - m_shape->getGlobalBounds().size.x / 2 <= 0)
        {
            *xSpeed *= -1;
        }
        if (m_shape->getGlobalBounds().getCenter().x+ m_shape->getGlobalBounds().size.x / 2 >= window->getSize().x)
        {
            *xSpeed *= -1;
        }
     }

     ~MyShape()
     {
         delete m_shape; // Prevent memory leak
     }

};


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

 

    MyShape shape("Circle", { 100, 100 }, { 1, 1 }, { 50, 50 }, { 1, 0, 0 }); // Red rectangle

       /* const sf::Font font("arial.ttf");*/


    /*if (!font.openFromFile("file"))
    {
        std::cerr << "Font not found \n";
        exit(-1);

    }*/

    /*sf::Text text(font,"Sampletext", 24);*/
    
    

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
        ImGui::SliderFloat("Xspeed", &xSpeed, -100.0f, 100.0f);
        ImGui::SliderFloat("Yspeed", &ySpeed, -100.0f, 100.0f);
        ImGui::ColorEdit3("Color", c);
        //ImGui::InputText("Text", displayString, 255);
        ImGui::End();

        shape.ChangeSpeed({ xSpeed,ySpeed });

        shape.ChangeColor({ 0,1,1 });

        shape.MoveShape();


       /* text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Red);
        text.setPosition({ 100.0f,100.0f });*/

        

        shape.CheckForBounce(&window,&xSpeed,&ySpeed);
        
        

        window.clear();
        //if (drawCircle)
        //{
        //   /* window.draw(text);*/
        //    window.draw(circle);
        //}

        window.draw(*shape.GetShape());
       
        
        ImGui::SFML::Render(window);
        window.display();
    }
	return 0;
}