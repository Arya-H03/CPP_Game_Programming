
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-sfml.h"
#include <iostream>
#include <fstream>
#include <sstream>

const std::string configPath = "./resources/config.txt";

unsigned int wWidth = 100;
unsigned int wHeight = 100;

std::string fontPath;
sf::Font font;
int fontSize = 14;
sf::Color fontColor(0, 0, 0);


void ErrorLog(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
}

template <typename T>
void DebugLog(const T& message) {
    std::ostringstream oss;
    oss << message;  // Convert message to string
    std::cerr << "Debug: " << oss.str() << std::endl;
}

class MyShape
{
    
    std::shared_ptr <sf::Shape> m_shape;
    std::string m_name, m_shapeType;
    sf::Vector2f m_size, m_velocity, m_position;
    sf::Color m_color;
    bool m_canDrawShape = true;
    float m_scale = 1.0f;
    sf::Text m_text;

public:

    MyShape(std::string name, std::string shapeType, sf::Vector2f size, sf::Vector2f startSpeed, sf::Vector2f startPos, sf::Color color)
        : m_name(name), m_shapeType(shapeType), m_size(size), m_velocity(startSpeed), m_position(startPos), m_color(color), m_text(font, m_name, fontSize)
    {
        if (m_shapeType == "Circle")
        {
            m_shape = std::make_shared<sf::CircleShape>(m_size.x, 32); 
            
        }
        else if (m_shapeType == "Rectangle")
        {
            m_shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(m_size.x, m_size.x)); 
           
        }
        else
        {
            m_shape = nullptr;
          
           
        }

        SetColor(m_color);
        m_text.setFont(font);
        m_text.setString(m_name);
        m_text.setCharacterSize(fontSize);
        m_text.setFillColor(fontColor);
     
    }

     const sf::Shape*GetShape() const { return m_shape.get(); }
     float GetScale() const { return m_scale; }
     bool CanDraw() const { return m_canDrawShape; }
     sf::Vector2f GetVelocity() const { return m_velocity; }
     sf::Vector2f GetSize() const { return m_size * m_scale; }
     sf::Vector2f GetPosition() const { return m_position; }
     sf::Color GetColor() const { return m_color; }
     const std::string& GetName() const { return m_name; }
     const sf::Text& GetText() const { return m_text; }

     void SetScale(float scale) { m_scale = scale; m_shape->setScale({ scale, scale }); }
     void SetCanDraw(bool value) { m_canDrawShape = value; }
     void SetName(std::string newName){ m_name = newName;   m_text.setString(m_name);}
     void SetSpeed(sf::Vector2f velocity) { m_velocity = velocity; }
     void SetPosition(sf::Vector2f pos) { m_position = pos; m_shape->setPosition(pos); }
     void SetColor(sf::Color color) { m_color = color; m_shape->setFillColor(color); }

     void MoveShape()
     {
         m_position+= m_velocity;
         m_shape->setPosition(m_position);
         m_text.setPosition({ m_shape->getGlobalBounds().getCenter().x - m_text.getGlobalBounds().size.x / 2,
                              m_shape->getGlobalBounds().getCenter().y - m_text.getGlobalBounds().size.y });
     }

     void CheckForBounce(sf::RenderWindow &window)
     {
         const auto bounds = m_shape->getGlobalBounds();
         const auto center = bounds.getCenter();
         const auto size = bounds.size;

         if (center.y - size.y / 2 <= 0 && m_velocity.y < 0) m_velocity.y *= -1; // up 
      
         if (center.y + size.y / 2 >= window.getSize().y && m_velocity.y > 0) m_velocity.y *= -1; //down
       
         if (center.x - size.x / 2 <= 0 && m_velocity.x < 0) m_velocity.x*= -1;//left
        
         if (center.x + size.x / 2 >= window.getSize().x && m_velocity.x > 0) m_velocity.x *= -1;//right
      
     }
};

void ReadFromFile(const std::string& filename, std::vector<MyShape> &shapes)
{
    std::string temp;
    std::ifstream fin(filename);

    if (!fin)
    {
        ErrorLog("Could not open file: " + filename);
    }

    std::string shapeName;
    sf::Vector2f pos, speed, size;
    sf::Color color;
    float radius;


    while (fin >> temp)
    {
        if (temp == "Window") {
            if (!(fin >> wWidth >> wHeight))
            {
                ErrorLog("Invalid format for window size.");             
                return;                
            }
        }
        else if (temp == "Font")
        {
            int r, g, b;
            if (!(fin >> fontPath >> fontSize >> r >> g >> b))
            {
                ErrorLog("Invalid format for font.");
                return;
            }
            else fontColor = sf::Color(r, g, b);
           
           
            
        }
        else if (temp == "Circle")
        {
            int r, g, b;
            if (!(fin >> shapeName >> pos.x >> pos.y >> speed.x >> speed.y >> r >> g >> b >> radius))
            {
                ErrorLog("Invalid format for Circle.");
                return;
            }
            else
            {
                color = sf::Color(r, g, b);
                shapes.push_back(MyShape(shapeName, temp, { radius, radius }, speed, pos, color));
            }
        }
        else if(temp == "Rectangle")
        {
            int r, g, b;
            if (!(fin >> shapeName >> pos.x >> pos.y >> speed.x >> speed.y >> r >> g >> b >> size.x >> size.y))
            {
                ErrorLog("Invalid format for Rectangle.");
                return;
            }
            else
            {
                color = sf::Color(r, g, b);               
                shapes.push_back(MyShape(shapeName, temp, size, speed, pos, color));
            }
        }
    }
}

void UpdateImGui(MyShape& shape, bool &currentCanDraw, std::string& currentShapeName, sf::Vector2f& currentVelocity, float& currentScale, sf::Color& currentColor)
{
    currentShapeName = shape.GetName();
    currentVelocity = shape.GetVelocity();
    currentScale = shape.GetScale();
    currentColor = shape.GetColor();
    currentCanDraw = shape.CanDraw();
}

void UpdateCurrentShape(MyShape& currentShape, bool &currentCanDraw, std::string& currentShapeName, sf::Vector2f& currentVelocity, float& currentScale, sf::Color& currentColor)
{
    currentShape.SetCanDraw(currentCanDraw);
    currentShape.SetScale(currentScale);
    currentShape.SetSpeed(currentVelocity);
    currentShape.SetColor(currentColor);
}


int main()
{
    std::vector < MyShape> shapes;

    ReadFromFile(configPath, shapes);
    if (shapes.empty()) 
    {
        ErrorLog("No shapes loaded.");
        return -1;
    }

    if (!font.openFromFile(fontPath)) {
        ErrorLog("Failed to load font.");
        return -1;
    }

   
    sf::RenderWindow window(sf::VideoMode({ wWidth, wHeight }), "Bounding Shapes!");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    sf::Clock deltaclock;

    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    std::string currentShapeName;
    sf::Vector2f currentVelocity;
    float currentScale;
    sf::Color currentColor;
    bool currentCanDraw;
    MyShape* currentShape = &shapes[0];

    UpdateImGui(*currentShape, currentCanDraw, currentShapeName, currentVelocity, currentScale, currentColor);

    ImGuiComboFlags flags = 0;
    int item_selected_idx = 0; 

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
       
        UpdateCurrentShape(*currentShape, currentCanDraw, currentShapeName, currentVelocity, currentScale, currentColor);

        //Movement & Bounce
        for (auto &shape : shapes)
        {
            if (shape.CanDraw())
            {
                shape.MoveShape();
                shape.CheckForBounce(window);
            }          
        }
        //

        UpdateImGui(*currentShape, currentCanDraw, currentShapeName, currentVelocity, currentScale, currentColor);
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
                    UpdateImGui(*currentShape, currentCanDraw, currentShapeName, currentVelocity, currentScale, currentColor);
                }


                
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        //

        //CanDraw CheckBox
        ImGui::Checkbox("Can Draw", &currentCanDraw);

        //Scale Slider
        ImGui::SliderFloat("Scale", &currentScale, 0.5f, 5.0f);

        //Velocity Slider
        float velocity[2] = { currentVelocity.x, currentVelocity.y };
        if (ImGui::SliderFloat2("Velocity", velocity, -10.0f, 10.0f))
        {
            currentVelocity.x = velocity[0];
            currentVelocity.y = velocity[1];
        }
        //
        
        //Color Props
        float color[3] = { currentColor.r / 255.0f, currentColor.g / 255.0f, currentColor.b / 255.0f };
        if (ImGui::ColorEdit3("Color", color))
        {
            currentColor.r = std::uint8_t(color[0] * 255);
            currentColor.g = std::uint8_t(color[1] * 255);
            currentColor.b = std::uint8_t(color[2] * 255);
        }
        ImGui::End();
        //

        window.clear();
        

        //Drawing
        for (auto& shape : shapes)
        {
            if (shape.CanDraw())
            {
                window.draw(*shape.GetShape());
                window.draw(shape.GetText());
            }
           
        }
        //
    
        ImGui::SFML::Render(window);
        window.display();


    }

    
    return 0;
}