#pragma once
#include<SFML/Graphics/Font.hpp>
class AssetManager
{
public:

	static AssetManager& Instance();
	void Initialize(const std::string& fontPath);
	const sf::Font& GetFont();

private:

	AssetManager() = default;
	void LoadFont(const std::string& fontPath);

	sf::Font m_font;
};

