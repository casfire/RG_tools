#pragma once
#ifndef _TEXTURE_BASE_HPP_
#define _TEXTURE_BASE_HPP_

#include <vector>
#include <cstddef> // std::size_t
#include <cstdint> // std::uintX_t

namespace Texture {
	
	
	
	typedef std::size_t   size_type;
	typedef std::uint8_t  Uint8;
	typedef std::uint16_t Uint16;
	typedef std::uint32_t Uint32;
	struct Pixel8;
	struct Pixel16;
	struct Pixel32;
	class  Base;
	
	
	
	/* Base texture object to store pixels */
	class Base {
	public:
		
		/* Constructors */
		Base();
		Base(const Base &copy);
		Base(
			size_type width,
			size_type height,
			size_type depth = 1,
			size_type channels = 3,
			size_type bytes = 1
		);
		
		/* Virtual destructor */
		virtual ~Base();
		
		/* Raw pixels */
		void* getRawPixels();
		const void* getRawPixels() const;
		
		/* Number of bytes for raw pixels */
		size_type getRawSize() const;
		
		/* Dimensions */
		size_type getWidth() const;
		size_type getHeight() const;
		size_type getDepth() const;
		
		/* Number of channels (1, 2, 3 or 4) */
		size_type getChannels() const;
		
		/* Number of bytes per color (1, 2 or 4) */
		size_type getBytes() const;
		
		/* Resize */
		void resize(
			size_type width,
			size_type height,
			size_type depth = 1,
			size_type channels = 3,
			size_type bytes = 1
		);
		
		/* Get a single pixel */
		Uint32  getPixel  (size_type x, size_type y, size_type z = 0) const;
		Pixel8  getPixel8 (size_type x, size_type y, size_type z = 0) const;
		Pixel16 getPixel16(size_type x, size_type y, size_type z = 0) const;
		Pixel32 getPixel32(size_type x, size_type y, size_type z = 0) const;
		
		/* Set a single pixel */
		void setPixel  (Uint32  p, size_type x, size_type y, size_type z = 0);
		void setPixel8 (Pixel8  p, size_type x, size_type y, size_type z = 0);
		void setPixel16(Pixel16 p, size_type x, size_type y, size_type z = 0);
		void setPixel32(Pixel32 p, size_type x, size_type y, size_type z = 0);
		
	private:
		
		size_type width, height, depth;
		size_type channels, bytes;
		std::vector<Uint8> pixels;
		
	};
	
	
	
	/* 8-bit pixel */
	struct Pixel8 {
		Uint8 r, g, b, a;
		Pixel8(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF);
		Pixel8(Uint32 pixel);
		Uint32  pixel()   const;
		Pixel16 pixel16() const;
		Pixel32 pixel32() const;
	};
	
	/* 16-bit pixel */
	struct Pixel16 {
		Uint16 r, g, b, a;
		Pixel16(Uint16 r, Uint16 g, Uint16 b, Uint16 a = 0xFFFF);
		Pixel16(Uint32 pixel);
		Uint32  pixel()   const;
		Pixel8  pixel8()  const;
		Pixel32 pixel32() const;
	};
	
	/* 32-bit pixel */
	struct Pixel32 {
		Uint32 r, g, b, a;
		Pixel32(Uint32 r, Uint32 g, Uint32 b, Uint32 a = 0xFFFFFFFF);
		Pixel32(Uint32 pixel);
		Uint32  pixel()   const;
		Pixel8  pixel8()  const;
		Pixel16 pixel16() const;
	};
	
	
	
};

#endif // _TEXTURE_BASE_HPP_
