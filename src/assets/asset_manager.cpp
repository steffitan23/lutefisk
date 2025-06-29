#include "asset_manager.h"
#include <iostream>

AssetManager &AssetManager::get()
{
    static AssetManager instance;
    return instance;
}

sf::Texture &AssetManager::get_texture(const std::string &filename)
{
    std::cout << filename << std::endl;
    auto &map = m_textures;
    if (map.find(filename) == map.end())
    {
        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(filename))
            throw std::runtime_error("Failed to load texture: " + filename);
        map[filename] = std::move(texture);
    }

    return *map[filename];
}

sf::Font &AssetManager::get_font(const std::string &filename)
{
    auto &map = m_fonts;
    if (map.find(filename) == map.end())
    {
        auto font = std::make_unique<sf::Font>(filename);
        map[filename] = std::move(font);
    }

    return *map[filename];
}