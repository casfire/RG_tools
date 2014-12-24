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
		Byte order: big endian
		
		uint32_t  magic   = 0x43465254; (CFRT)
		uint8_t   version = 1;
		uint16_t  width;    // Texture width in pixels
		uint16_t  height;   // Texture height in pixels
		uint16_t  depth;    // Texture depth in pixels
		uint8_t   channels; // Number of channels (1, 2, 3 or 4)
		uint8_t   bytes;    // Number of bytes per color (1, 2 or 4)
		uint8_t   pixels[width * height * depth * channels * bytes];
		
	*/
	
	
	
} // namespace CFR

#endif // _CFR_TEXTURE_HPP_
