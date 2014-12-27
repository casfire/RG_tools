#pragma once
#ifndef _CFR_TEXTURE_HPP_
#define _CFR_TEXTURE_HPP_

#include "Common.hpp"
#include "BaseTexture.hpp"
#include <istream>
#include <ostream>
#include <string>

std::istream& operator>>(std::istream& in, CFR::Texture& obj);
std::ostream& operator<<(std::ostream& out, const CFR::Texture& obj);

namespace CFR {
	
	
	
	/* CFR Texture */
	class Texture : public BaseTexture {
	public:
		
		/* Constructors */
		Texture();
		Texture(const BaseTexture &copy);
		Texture(
			size_type width,
			size_type height,
			size_type depth = 1,
			size_type channels = 3,
			size_type bytes = 1
		);
		
		/* Load/Save texture - throws CFR::Exception */
		void loadFromFile(const std::string &file);
		void   saveToFile(const std::string &file) const;
		
		/* Stream insertion/extraction */
		friend std::istream& ::operator>>(std::istream&, Texture&);
		friend std::ostream& ::operator<<(std::ostream&, const Texture&);
		
	};
	
	
	
	/*
		CFR texture file format
		Byte order: little endian
		
		Uint32  magic   = 0x54524643; // CFRT
		Uint32  version = 1;
		Uint16  width;    // Texture width  in pixels
		Uint16  height;   // Texture height in pixels
		Uint16  depth;    // Texture depth  in pixels
		Uint8   channels; // Number of channels (1, 2, 3 or 4)
		Uint8   bytes;    // Number of bytes per color (1, 2 or 4)
		Uint8   pixels[width * height * depth * channels * bytes];
		
	*/
	
	
	
} // namespace CFR

#endif // _CFR_TEXTURE_HPP_
