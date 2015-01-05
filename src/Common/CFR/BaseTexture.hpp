#pragma once
#ifndef _CFR_BASETEXTURE_HPP_
#define _CFR_BASETEXTURE_HPP_

#include "Common.hpp"
#include <vector>

namespace CFR {
	
	
	
	/* Base texture object to store pixels */
	class BaseTexture {
	public:
		
		/* Constructors */
		BaseTexture();
		BaseTexture(const BaseTexture &copy);
		BaseTexture(
			size_type width,
			size_type height,
			size_type depth = 1,
			size_type channels = 3,
			size_type bytes = 1
		);
		
		/* Virtual destructor */
		virtual ~BaseTexture();
		
		/* Raw pixels */
		void* getRawPixels();
		const void* getRawPixels() const;
		
		/* Number of bytes for raw pixels */
		size_type getRawSize() const;
		
		/* Offset in bytes of a single pixel */
		size_type getOffset(size_type x, size_type y, size_type z) const;
		
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
	
	
	
} // namespace CFR

#endif // _CFR_BASETEXTURE_HPP_
