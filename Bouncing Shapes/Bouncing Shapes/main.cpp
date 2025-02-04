
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
    std::array<float, 3> m_color;        // {r, g, b}
    bool m_canDrawShape = true;
    float m_scale = 1;

    sf::Text m_text;

public:

    MyShape(std::string name, std::string shapeType, std::array<float, 2> size, std::array<float, 2> startSpeed, std::array<float, 2> startPos, std::array<float, 3> color)
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

        SetColor(m_color);
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

     float GetScale()
     {
         return m_scale;
     }
     bool GetCanDrawShape()
     {
         return m_canDrawShape;
     }
     std::array <float,2> GetSpeed()
     {
         return m_speed;
     }

     std::array <float, 2> GetSize()
     {
         return { m_size[0] * m_scale,m_size[1] * m_scale };
     }

     std::array <float, 2> GetPosition()
     {
         return m_pos;
     }

     std::array <float, 3> GetColor()
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

     void SetScale(float newScale)
     {
         m_scale = newScale;
         m_shape->setScale({ m_scale, m_scale });
     }
     void SetCanDrawShape(bool value)
     {
         m_canDrawShape = value;
     }
     void SetName(std::string newName)
     {
         m_name = newName;
         m_text.setString(m_name);
     }

     void SetSize(std::array<float, 2> newSize)
     {
         m_size = newSize;
        
     }

     void SetSpeed(std::array<float, 2> newSpeed)
     {
         m_speed = newSpeed;
     }

     void SetPosition(std::array<float, 2> newPos)
     {
         m_pos = newPos;
         m_shape->setPosition({ m_pos[0],m_pos[1] });
     }

     void SetColor(std::array<float, 3> newColor)
     {
         m_color = newColor;
         m_shape->setFillColor(sf::Color(std::uint8_t(m_color[0] * 255), std::uint8_t(m_color[1] * 255 ), std::uint8_t(newColor[2] * 255)));
     }

     void MoveShape()
     {
         m_pos[0] += m_speed[0];
         m_pos[1] += m_speed[1];
         m_shape->setPosition({ m_pos[0], m_pos[1] });

         m_text.setPosition({ m_shape->getGlobalBounds().getCenter().x - m_text.getGlobalBounds().size.x / 2,m_shape->getGlobalBounds().getCenter().y - m_text.getGlobalBounds().size.y });
     }  

     void CheckForBounce(sf::RenderWindow *window)
     {
         if (m_shape->getGlobalBounds().getCenter().y - m_shape->getGlobalBounds().size.y / 2 <= 0) // up
        {
             
             if (m_speed[1] < 0)
             {
                 m_speed[1] *= -1;
             }
           
        }
        if (m_shape->getGlobalBounds().getCenter().y + m_shape->getGlobalBounds().size.y / 2 >= window->getSize().y)// down
        {
            if (m_speed[1] > 0)
            {
                m_speed[1] *= -1;
            }
           
        }

        if (m_shape->getGlobalBounds().getCenter().x - m_shape->getGlobalBounds().size.x / 2 <= 0)//left
        {
            if (m_speed[0] < 0)
            {
                m_speed[0] *= -1;
            }

            
        }
        if (m_shape->getGlobalBounds().getCenter().x+ m_shape->getGlobalBounds().size.x / 2 >= window->getSize().x)//right
        {
            if (m_speed[0] > 0)
            {
                m_speed[0] *= -1;
            }
        }
     }
};

std::string currentShapeName;
std::array<float,3> currentColor;
std::array<float,2> currentVelocity;
float currentScale;
bool  currentCanDraw = true;

void ReadFromFile(const std::string& filename, std::vector<MyShape> *shapes)
{
    std::string temp;
    std::ifstream fin(filename);

    std::string shapeName;
    std::array<float, 2> pos;
    std::array<float, 2> speed;
    std::array<float, 2> size;
    std::array<float, 3> color;
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
                color[0] /= 255.0f;
                color[1] /= 255.0f;
                color[2] /= 255.0f;
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
                color[0] /= 255.0f;
                color[1] /= 255.0f;
                color[2] /= 255.0f;
               
                shapes->push_back(MyShape(shapeName, temp, { size[0], size[1] }, speed, pos, color));
            }
        }

    }
}

void UpdateImGuiVariables(MyShape* newShape)
{
    currentShapeName = newShape->GetName();
    currentScale = newShape->GetScale();
    currentColor = newShape->GetColor();
    currentShapeName = newShape->GetName();
    currentVelocity = newShape->GetSpeed();

}

int main()
{
    std::vector < MyShape> shapes;
    ReadFromFile(configPath, &shapes);
    MyShape* currentShape = &shapes[0];

    UpdateImGuiVariables(currentShape);
   

    sf::RenderWindow window(sf::VideoMode({ wWidth, wHeight }), "Bounding Shapes!");
    window.setFramerateLimit(60);
    font.openFromFile(fontPath);

    ImGui::SFML::Init(window);
    sf::Clock deltaclock;

    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;



    static ImGuiComboFlags flags = 0;
    static int item_selected_idx = 0; 

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
       
        currentShape->SetCanDrawShape(currentCanDraw);
        currentShape->SetScale(currentScale);
        currentShape->SetSpeed(currentVelocity);
        currentShape->SetColor(currentColor);

        for (MyShape& shape : shapes)
        {
            if (shape.GetCanDrawShape())
            {
                shape.MoveShape();
                shape.CheckForBounce(&window);
            }

            
        }

        UpdateImGuiVariables(currentShape);
        ImGui::SFML::Update(window, deltaclock.restart());
        
        ImGui::Begin("Shape Properties");

        //Shape Checkbox
        std::string selected_name = shapes[item_selected_idx].GetName();
        const char* combo_preview_value = selected_name.c_str();

        if (ImGui::BeginCombo("Shapes", combo_preview_value, flags))
        {
            for (int n = 0; n < shapes.size(); n++)
            {
                const bool is_selected = (item_selected_idx == n);
                if (ImGui::Selectable(shapes[n].GetName().c_str(), is_selected))
                {
                    item_selected_idx = n;
                    currentShape = &shapes[n];
                    UpdateImGuiVariables(currentShape);
                }


                
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }


        ImGui::Checkbox("Can Draw", &currentCanDraw);
        ImGui::SliderFloat("Scale", &currentScale, 0.5f, 5.0f);
        ImGui::SliderFloat2("Velocity", currentVelocity.data(), -10.0f, 10.0f);
        ImGui::ColorEdit3("Color",  currentColor.data());
        ImGui::End();

        window.clear();
        
        for (MyShape& shape : shapes)
        {
            if (shape.GetCanDrawShape())
            {
                window.draw(*shape.GetShape());
                window.draw(*shape.GetText());
            }
           
        }
    
        ImGui::SFML::Render(window);
        window.display();


    }

    
    return 0;
}