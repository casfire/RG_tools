#include "Popup.hpp"
#include "font.h"

Popup::Popup(
	const std::string &title,
	const std::string &message,
	sf::Vector2u size,
	sf::Color msgColor,
	sf::Color bgColor)
: title(title), message(message), size(size), msgColor(msgColor), bgColor(bgColor)
{}

void Popup::setMessage(const std::string &text)
{
	message = text;
}

void Popup::setTitle(const std::string &text)
{
	title = text;
}

void Popup::setMessageColor(sf::Color color)
{
	msgColor = color;
}

void Popup::setBackgroundColor(sf::Color color)
{
	bgColor = color;
}

void Popup::setSize(sf::Vector2u size)
{
	this->size = size;
}

void Popup::show() const
{
	sf::Font font;
	font.loadFromMemory(FONT_TTF, sizeof(FONT_TTF));
	
	sf::Text text(message, font, 31);
	text.setColor(msgColor);
	sf::FloatRect bounds = text.getLocalBounds();
	sf::Vector2u resolution = size;
	if (resolution.x < bounds.width  * 1.2) resolution.x = bounds.width  * 1.2;
	if (resolution.y < bounds.height * 1.2) resolution.y = bounds.height * 1.2;
	text.setPosition(
		(resolution.x - bounds.width) / 2.f, 
		(resolution.y - bounds.height) / 2.f
	);
	
	sf::RenderWindow window(
		sf::VideoMode(resolution.x, resolution.y),
		title.c_str(),
		sf::Style::Titlebar | sf::Style::Close
	);
	
	while (window.isOpen()) {
		window.clear(bgColor);
		window.draw(text);
		window.display();
		sf::Event event;
		if (window.waitEvent(event) && event.type == sf::Event::Closed) {
			window.close();
		}
	}
}