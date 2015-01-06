#include "Common/Common.hpp"
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

sf::Image textureToImage(const CFR::BaseTexture &from, CFR::size_type z = 0);

int main(int argc, char* args[]) {
	
	/* Check arguments */
	if (argc != 2) {
		std::cerr << "Error: No input file.\n";
		return -1;
	}
	
	/* Load texture */
	CFR::Texture cfrt;
	try {
		cfrt.loadFromFile(args[1]);
	} catch (CFR::Exception &fail) {
		std::cerr << "Error: " << fail.what() << "\n";
		return -1;
	}
	
	/* Find suitable resolution */
	sf::Vector2u resolutionMax(
		sf::VideoMode::getDesktopMode().width * 0.9,
		sf::VideoMode::getDesktopMode().height * 0.9
	);
	sf::Vector2u resolutionMin(480, 120);
	sf::Vector2u resolution(cfrt.getWidth(), cfrt.getHeight());
	if (resolution.x > resolutionMax.x) resolution.x = resolutionMax.x;
	if (resolution.y > resolutionMax.y) resolution.y = resolutionMax.y;
	sf::Vector2f scale(
		resolution.x / (float) cfrt.getWidth(),
		resolution.y / (float) cfrt.getHeight()
	);
	if (scale.x < scale.y) {
		scale.y = scale.x;
		resolution.y = cfrt.getHeight() * scale.y;
	} else if (scale.y < scale.x) {
		scale.x = scale.y;
		resolution.x = cfrt.getWidth() * scale.x;
	}
	if (resolution.x < resolutionMin.x) resolution.x = resolutionMin.x;
	if (resolution.y < resolutionMin.y) resolution.y = resolutionMin.y;
	
	/* Create sprite */
	sf::Texture spriteTexture;
	spriteTexture.loadFromImage(textureToImage(cfrt));
	sf::Sprite sprite(spriteTexture);
	sprite.setOrigin(cfrt.getWidth() / 2.f, cfrt.getHeight() / 2.f);
	sprite.setPosition(resolution.x / 2.f, resolution.y / 2.f);
	
	/* Create window */
	std::string title = removePath(args[1])
		+ " - " + to_string(cfrt.getWidth())
		+ " x " + to_string(cfrt.getHeight())
		+ " x " + to_string(cfrt.getDepth())
		+ " "   + to_string(getChannelName(cfrt.getChannels()))
		+ " ("  + to_string(cfrt.getBytes() * 8) + "-bit)";
	sf::RenderWindow window(
		sf::VideoMode(resolution.x, resolution.y),
		title.c_str(),
		sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize
	);
	
	/* Main loop */
	while (window.isOpen()) {
		sprite.setScale(
			scale.x * resolution.x / (float) window.getSize().x,
			scale.y * resolution.y / (float) window.getSize().y
		);
		window.clear(sf::Color::Black);
		window.draw(sprite);
		window.display();
		sf::Event event;
		while (!window.waitEvent(event));
		switch (event.type) {
		default: break;
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::MouseWheelMoved:
			scale.x *= std::pow(1.2f, event.mouseWheel.delta);
			scale.y *= std::pow(1.2f, event.mouseWheel.delta);
			break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Space) {
				scale = sf::Vector2f(1, 1);
			}
			break;
		}
	}
	
	return 0;
}

sf::Image textureToImage(const CFR::BaseTexture &from, CFR::size_type z)
{
	std::vector<std::uint8_t> pixels(from.getWidth() * from.getHeight() * 4);
	for (std::size_t y = 0; y < from.getHeight(); y++) {
		for (std::size_t x = 0; x < from.getWidth(); x++) {
			std::size_t offset = 4 * x + 4 * y * from.getWidth();
			CFR::Pixel8 pixel = from.getPixel8(x, y, z);
			if (from.getChannels() == 1) {
				pixel.g = pixel.r;
				pixel.b = pixel.r;
			}
			pixels[offset + 0] = pixel.r;
			pixels[offset + 1] = pixel.g;
			pixels[offset + 2] = pixel.b;
			pixels[offset + 3] = pixel.a;
		}
	}
	const sf::Uint8 *data = reinterpret_cast<const sf::Uint8*>(pixels.data());
	sf::Image image;
	image.create(from.getWidth(), from.getHeight(), data);
	return image;
}
