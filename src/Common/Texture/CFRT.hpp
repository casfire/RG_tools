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
	
	
	
} // namespace Texture

#endif // _TEXTURE_CFRT_HPP_
