#include "Common/Common.hpp"
#include <iostream>
#include <FreeImage.h>

bool convert(const std::string &filename) {
	
	std::string file    = removePath(filename);
	std::string outFile = getPrefix(filename, '.') + ".cfrt";
	
	/* Retrieve file format */
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str(), 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filename.c_str());
	if (fif == FIF_UNKNOWN) {
		std::cout << "Unknown file format " << file << std::endl;
		return false;
	}
	
	/* Load image */
	FIBITMAP *dib = nullptr;
	if (FreeImage_FIFSupportsReading(fif)) dib = FreeImage_Load(fif, filename.c_str());
	if (!dib) {
		std::cout << "Failed to load " << file << std::endl;
		return false;
	}
	
	/* Convert to standard format if necessary */
	if (FreeImage_GetImageType(dib) != FIT_BITMAP) {
		FIBITMAP *tmp = dib;
		dib = FreeImage_ConvertToStandardType(dib);
		FreeImage_Unload(tmp);
		if (!dib) {
			std::cout << "Failed to convert " << file << " to standard type." << std::endl;
			return false;
		}
	}
	
	/* Convert bpp if needed */
	unsigned int bpp = FreeImage_GetBPP(dib);
	if (bpp <= 8) {
		FIBITMAP *tmp = dib;
		dib = FreeImage_ConvertToGreyscale(dib);
		FreeImage_Unload(tmp);
		bpp = 8;
		if (!dib || FreeImage_GetBPP(dib) != 8) bpp = 0;
	} else if (bpp > 32) {
		FIBITMAP *tmp = dib;
		dib = FreeImage_ConvertTo32Bits(dib);
		FreeImage_Unload(tmp);
		bpp = 32;
		if (!dib || FreeImage_GetBPP(dib) != 32) bpp = 0;
	}
	
	/* Get image information */
	unsigned int width  = FreeImage_GetWidth(dib);
	unsigned int height = FreeImage_GetHeight(dib);
	unsigned int bytes    = 1;
	unsigned int channels = 0;
	switch (bpp) {
		case 8:  channels = 1; break;
		case 24: channels = 3; break;
		case 32: channels = 4; break;
		default: bpp = 0;
	}
	
	/* Check if image is valid */
	if (!dib || bpp == 0 || width == 0 || height == 0 || !FreeImage_HasPixels(dib)) {
		if (dib) FreeImage_Unload(dib);
		std::cout << "Invalid image " << file << std::endl;
		return false;
	}
	
	std::cout << file << " Loaded. Converting.\n";
	
	/* Create CFR texture */
	CFR::Texture texture(
		width, height, 1,
		channels, bytes
	);
	
	/* Set texture pixels */
	for (unsigned int y = 0; y < height; y++) {
		BYTE* bits = FreeImage_GetScanLine(dib, height - y);
		for (unsigned int x = 0; x < width; x++) {
			CFR::Pixel8 pixel(0, 0, 0, 0);
			BYTE *p = bits + (channels * x);
			if (channels >= 1) pixel.r = p[FI_RGBA_RED];
			if (channels >= 2) pixel.g = p[FI_RGBA_GREEN];
			if (channels >= 3) pixel.b = p[FI_RGBA_BLUE];
			if (channels >= 4) pixel.a = p[FI_RGBA_ALPHA];
			texture.setPixel8(pixel, x, y, 0);
		}
	}
	
	/* Unload image */
	FreeImage_Unload(dib);
	
	/* Save texture */
	try {
		texture.saveToFile(outFile);
	} catch (CFR::Exception &fail) {
		std::cout << "Failed to save " << removePath(outFile) << ": " << fail.what() << std::endl;
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
	
	/* Initialize FreeImage */
	#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
	#endif
	std::cout << "Using FreeImage version " << FreeImage_GetVersion() << "\n";
	
	/* Convert all files */
	for (int i = 1; i < argc; i++) convert(args[i]);
	
	/* Deinitialize FreeImage */
	#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
	#endif
	std::cout << "\nFinished." << std::endl;
	
	/* Wait for input */
	std::cin.get();
	
	return 0;
}
