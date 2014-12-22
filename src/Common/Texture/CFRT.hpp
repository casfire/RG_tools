#pragma once
#ifndef _TEXTURE_CFRT_HPP_
#define _TEXTURE_CFRT_HPP_

#include "Base.hpp"
#include <istream>
#include <ostream>
#include <exception>
#include <string>

namespace Texture { class CFRT; }
std::istream& operator>>(std::istream& in, Texture::CFRT& obj);
std::ostream& operator<<(std::ostream& out, const Texture::CFRT& obj);

namespace Texture {
	
	
	
	class CFRT;
	class CFRTException;
	
	
	
	/* CFR Texture */
	class CFRT : public Base {
	public:
		
		/* Constructors */
		CFRT();
		CFRT(const Base &copy);
		CFRT(
			size_type width,
			size_type height,
			size_type depth = 1,
			size_type channels = 3,
			size_type bytes = 1
		);
		
		/* Load/Save texture - throws CFRTException */
		void loadFromFile(const std::string &file);
		void   saveToFile(const std::string &file) const;
		
		/* Stream insertion/extraction */
		friend std::istream& ::operator>>(std::istream&, CFRT&);
		friend std::ostream& ::operator<<(std::ostream&, const CFRT&);
		
	};
	
	
	
	/* Thrown when loading or saving fails */
	class CFRTException : public std::exception {
	public:
		
		CFRTException(const std::string &info);
		const char* what() const throw();
		
	private:
		
		const std::string exceptionInfo;
		
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
	
	
	
} // namespace Texture

#endif // _TEXTURE_CFRT_HPP_
