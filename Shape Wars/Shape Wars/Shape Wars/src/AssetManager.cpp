#include "AssetManager.h"

AssetManager& AssetManager::Instance()
{
    static AssetManager instance;
    return instance;
}

void AssetManager::Initialize(const std::string& fontPath)
{
    LoadFont(fontPath);
}

void AssetManager::LoadFont(const std::string& fontPath)
{
    if (!m_font.openFromFile(fontPath)) throw std::runtime_error("Failed to load font at" + fontPath);
}

const sf::Font& AssetManager::GetFont()
{
    return m_font;
}
