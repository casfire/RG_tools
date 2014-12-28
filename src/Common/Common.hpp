#pragma once
#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include "CFR/Texture.hpp"
#include "CFR/Geometry.hpp"
#include "OBJ/ElementReader.hpp"
#include "OBJ/MaterialReader.hpp"
#include "Popup.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <cstddef> // std::size_t
#include <sstream>

std::string getSuffix     (const std::string &str, char c);
std::string getPrefix     (const std::string &str, char c);
std::string removePath    (const std::string &path);
std::size_t countFileLines(const std::string &file);

sf::Image      textureToImage (const CFR::BaseTexture &from, CFR::size_type z = 0);
const char*    getChannelName (CFR::size_type channels);
CFR::size_type findChannels   (const sf::Image &image);

template <typename T>
const std::string to_string(const T &value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

#endif
