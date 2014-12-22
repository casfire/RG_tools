#include <SFML/Graphics.hpp>
#include "Common/Common.hpp"

bool convert(const std::string &path) {
	
	std::string file = getSuffix(getSuffix(path, '\\'), '/');
	std::string cfrtFile = getPrefix(path, '.') + ".cfrt";
	
	/* Load image with SFML */
	sf::Image image;
	if (!image.loadFromFile(path)) {
		Popup("Error!", "Failed to load " + to_string(file) + " as image.").show();
		return false;
	}
	
	/* Create CFRT */
	Texture::CFRT cfrt(
		image.getSize().x, image.getSize().y, 1,
		findChannels(image), 1
	);
	
	/* Set pixels */
	sf::Vector2u size = image.getSize();
	for (unsigned y = 0; y < size.y; y++) {
		for (unsigned x = 0; x < size.x; x++) {
			sf::Color pixel = image.getPixel(x, y);
			cfrt.setPixel8(
				Texture::Pixel8(pixel.r, pixel.g, pixel.b, pixel.a),
				x, y, 0
			);
		}
	}
	
	/* Save */
	try {
		cfrt.saveToFile(cfrtFile);
	} catch (Texture::CFRTException &fail) {
		Popup(
			"Error!",
			"Failed to save " + getOnlyFile(cfrtFile)
			+ ": " + to_string(fail.what())
		).show();
		return false;
	}
	
	return true;
}

int main(int argc, char* args[]) {
	
	/* Check arguments */
	if (argc <= 1) {
		Popup("Error!", "No input files.").show();
		return 0;
	}
	
	/* Convert all files */
	bool status = true;
	for (int i = 1; i < argc; i++) {
		if (!convert(args[i])) status = false;
	}
	
	/* End message */
	if (status) {
		if (argc == 2) {
			Popup("CFRT Converter", "Converted.").show();
		} else {
			Popup("CFRT Converter", "All files converted.").show();
		}
	}
	
	return 0;
}
