
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-sfml.h"
#include <iostream>
#include <fstream>

const std::string configPath = "./resources/config.txt";


unsigned int wWidth = 100;
unsigned int wHeight = 100;


std::string fontPath = "";
sf::Font font;
int fontSize = 14;
int fontColor[3] = { 0,0,0 };






class MyShape
{
    
    std::shared_ptr <sf::Shape> m_shape;
    std::string m_name;
    std::string m_shapeType;
    std::array<float, 2> m_size;       // {width, height}
    std::array<float, 2> m_speed;      // {xSpeed, ySpeed}
    std::array<float, 2> m_pos;        // {xPos, yPos}
    std::array<int, 3> m_color;        // {r, g, b}

    sf::Text m_text;

public:

    MyShape(std::string name, std::string shapeType, std::array<float, 2> size, std::array<float, 2> startSpeed, std::array<float, 2> startPos, std::array<int, 3> color)
        : m_name(name), m_shapeType(shapeType), m_size(size), m_speed(startSpeed), m_pos(startPos), m_color(color), m_text(font, m_name, fontSize)
    {
        if (m_shapeType == "Circle")
        {
            m_shape = std::make_shared<sf::CircleShape>(m_size[0], 32); // Use shared_ptr
            std::cout << "Circle was created with name " << m_name << std::endl;
        }
        else if (m_shapeType == "Rectangle")
        {
            m_shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(m_size[0], m_size[1])); // Use shared_ptr
            std::cout << "Rect was created with name " << m_name << std::endl;
        }
        else
        {
            m_shape = nullptr;
            std::cout << "Shape was not created" << std::endl;
        }

        ChangeColor(m_color);
        // Initialize m_text after the font is loaded
        m_text.setFont(font);
        m_text.setString(m_name);
        m_text.setCharacterSize(fontSize);
        m_text.setFillColor(sf::Color(fontColor[0], fontColor[1], fontColor[2]));
    }

    // Other methods...


     std::shared_ptr <sf::Shape> GetShape() const
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

     sf::Text* GetText()
     {
         return &m_text;
     }

     void ChangeName(std::string newName)
     {
         m_name = newName;
         m_text.setString(m_name);
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
         m_shape->setFillColor(sf::Color(std::uint8_t(m_color[0]), std::uint8_t(m_color[1] ), std::uint8_t(newColor[2] )));
     }

     void MoveShape()
     {
         m_pos[0] += m_speed[0];
         m_pos[1] += m_speed[1];
         m_shape->setPosition({ m_pos[0], m_pos[1] });

         m_text.setPosition({ m_shape->getGlobalBounds().getCenter().x - m_text.getGlobalBounds().size.x / 2,m_shape->getGlobalBounds().getCenter().y - m_text.getGlobalBounds().size.y });
     }  

     void CheckForBounce(sf::RenderWindow *window, float* xSpeed, float* ySpeed)
     {
         if (m_shape->getGlobalBounds().getCenter().y - m_shape->getGlobalBounds().size.y / 2 <= 0)
        {
            m_speed[1] *= -1;
        }
        if (m_shape->getGlobalBounds().getCenter().y + m_shape->getGlobalBounds().size.y / 2 >= window->getSize().y)
        {
            m_speed[1] *= -1;
        }

        if (m_shape->getGlobalBounds().getCenter().x - m_shape->getGlobalBounds().size.x / 2 <= 0)
        {
            m_speed[0] *= -1;
        }
        if (m_shape->getGlobalBounds().getCenter().x+ m_shape->getGlobalBounds().size.x / 2 >= window->getSize().x)
        {
            m_speed[0] *= -1;
        }
     }

     //~MyShape()
     //{
     //    if (m_shape != nullptr)
     //    {
     //        delete m_shape;
     //        m_shape = nullptr; // Prevent double delete
     //    }
     //}

};

void ReadFromFile(const std::string& filename, std::vector<MyShape> *shapes)
{
    std::string temp;
    std::ifstream fin(filename);

    std::string shapeName;
    std::array<float, 2> pos;
    std::array<float, 2> speed;
    std::array<float, 2> size;
    std::array<int, 3> color;
    float radius;


    while (fin >> temp)
    {
        if (temp == "Window") {
            if (!(fin >> wWidth >> wHeight)) {
                std::cerr << "Error: Invalid format for window size." << std::endl;
                return;
                
            }
        }
        else if (temp == "Font")
        {
            if (!(fin >> fontPath >> fontSize >> fontColor[0] >> fontColor[1] >> fontColor[2]))
            {
                std::cerr << "Error: Invalid format for font.";
                return;
            }
        }
        else if (temp == "Circle")
        {
            if (!(fin >> shapeName >> pos[0] >> pos[1] >> speed[0] >> speed[1] >> color[0] >> color[1] >> color[2] >> radius))
            {
                std::cerr << "Error: Invalid format for Circle.";
                return;


            }
            else
            {

                shapes->push_back(MyShape(shapeName, temp, { radius, radius }, speed, pos, color));
            }
        }
        else if(temp == "Rectangle")
        {
            if (!(fin >> shapeName >> pos[0] >> pos[1] >> speed[0] >> speed[1] >> color[0] >> color[1] >> color[2] >> size[0]>>size[1]))
            {
                std::cerr << "Error: Invalid format for Rectangle.";
                return;


            }
            else
            {

                shapes->push_back(MyShape(shapeName, temp, { size[0], size[1] }, speed, pos, color));
            }
        }

    }
}
int main()
{
    std::vector < MyShape> shapes;

    ReadFromFile(configPath, &shapes);

    /*std::cerr << "Name: " << shapes[4].GetName() << "\n"
        << "Position: (" << shapes[4].GetPosition()[0] << ", " << shapes[4].GetPosition()[1] << ")\n"
        << "Speed: (" << shapes[4].GetSpeed()[0] << ", " << shapes[4].GetSpeed()[1] << ")\n"
        << "Color: (" << shapes[4].GetColor()[0] << ", " << shapes[4].GetColor()[1] << ", " << shapes[4].GetColor()[2] << ")\n"
        << "Size: " << shapes[4].GetSize()[0] << shapes[4].GetSize()[1]<< "\n"
        << "Shape Pointer: " << shapes[4].GetShape() << std::endl;*/

    sf::RenderWindow window(sf::VideoMode({ wWidth, wHeight }), "Bounding Shapes!");
    window.setFramerateLimit(60);
    font.openFromFile(fontPath);

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
        ImGui::SliderFloat("Xspeed", &xSpeed, -10.0f, 10.0f);

        ImGui::SliderFloat("Yspeed", &ySpeed, -10.0f, 10.0f);
        ImGui::ColorEdit3("Color", c);
        //ImGui::InputText("Text", displayString, 255);
        ImGui::End();

        for (MyShape& shape : shapes)
        {
            shape.MoveShape();
            shape.CheckForBounce(&window, &xSpeed, &ySpeed);
        }
        

        window.clear();
        
        for (MyShape& shape : shapes)
        {
            window.draw(*shape.GetShape());
            window.draw(*shape.GetText());
        }

    
        ImGui::SFML::Render(window);
        window.display();
    }
	return 0;
}