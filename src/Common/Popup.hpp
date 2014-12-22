#pragma once
#ifndef _POPUP_HPP_
#define _POPUP_HPP_

#include <string>
#include <SFML/Graphics.hpp>
#include "font.h"

class Popup {
public:
	
	Popup(
		const std::string &title,
		const std::string &message,
		sf::Vector2u size = sf::Vector2u(480, 120),
		sf::Color msgColor = sf::Color::Black,
		sf::Color bgColor = sf::Color::White
	);
	
	void show() const;
	
	void setMessage(const std::string &text);
	void setTitle  (const std::string &text);
	void setMessageColor   (sf::Color color);
	void setBackgroundColor(sf::Color color);
	void setSize(sf::Vector2u size);
	
private:
	
	std::string title, message;
	sf::Vector2u size;
	sf::Color msgColor, bgColor;
	
};

#endif
