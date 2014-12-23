#pragma once
#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>
#include "Texture/Base.hpp"
#include "Texture/CFRT.hpp"
#include "Popup.hpp"

std::string getSuffix(const std::string &str, char c);
std::string getPrefix(const std::string &str, char c);
std::string getOnlyFile(const std::string &path);

sf::Image textureToImage(const Texture::Base &from, Texture::size_type z = 0);
const char* getChannelName(Texture::size_type channels);
Texture::size_type findChannels(const sf::Image &image);

template <typename T>
const std::string to_string(const T &value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

#endif