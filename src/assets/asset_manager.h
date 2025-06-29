#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>

class AssetManager
{
public:
    static AssetManager &get();

    sf::Texture &get_texture(const std::string &filename);

    sf::Font &get_font(const std::string &filename);

    // delete copy semantics
    AssetManager(AssetManager &am) = delete;
    AssetManager &operator=(const AssetManager &am) = delete;

private:
    AssetManager() = default;

    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> m_fonts;
};