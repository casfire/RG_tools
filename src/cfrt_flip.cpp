#include "Common/Common.hpp"
#include <iostream>

bool flip(std::string filename) {
	
	/* Load */
	CFR::Texture cfrt;
	try {
		cfrt.loadFromFile(filename);
	} catch (CFR::Exception &fail) {
		std::cerr << "Error: " << fail.what() << "\n";
		std::cin.get();
		return false;
	}
	std::cout << removePath(filename) << " loaded. Flipping.\n";
	
	/* Flip */
	CFR::Texture flipped(cfrt);
	for (CFR::size_type y = 0; y < cfrt.getHeight(); y++) {
		for (CFR::size_type x = 0; x < cfrt.getWidth(); x++) {
			for (CFR::size_type z = 0; z < cfrt.getDepth(); z++) {
				flipped.setPixel32(cfrt.getPixel32(x, y, z), x, cfrt.getHeight() - y - 1, z);
			}
		}
	}
	std::cout << removePath(filename) << " flipped. Saving.\n";
	
	/* Save */
	try {
		flipped.saveToFile(filename);
	} catch (CFR::Exception &fail) {
		std::cerr << "Error: " << fail.what() << "\n";
		std::cin.get();
		return false;
	}
	
	return true;
}

int main(int argc, char* args[]) {
	
	/* Check arguments */
	if (argc <= 1) {
		std::cerr << "Error: No input files.\n";
		std::cin.get();
		return -1;
	}
	
	/* Flip all files */
	for (int i = 1; i < argc; i++) flip(args[i]);
	
	/* Wait for input */
	std::cout << "\nFinished." << std::endl;
	std::cin.get();
	
	return 0;
}
