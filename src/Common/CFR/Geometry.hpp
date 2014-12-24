#pragma once
#ifndef _CFR_GEOMETRY_HPP_
#define _CFR_GEOMETRY_HPP_

#include "Common.hpp"
#include "BaseGeometry.hpp"
#include <vector>
#include <string>
#include <istream>
#include <ostream>

std::istream& operator>>(std::istream& in, CFR::Geometry& obj);
std::ostream& operator<<(std::ostream& out, const CFR::Geometry& obj);

namespace CFR {
	
	
	
	class Geometry : public BaseGeometry {
	public:
		
		/* Create */
		Geometry(Uint8 vertexType = 0, Uint8 elementType = 4);
		Geometry(const Geometry &copy);
		
		/* Get types */
		Uint8 getVertexType() const;
		Uint8 getElementType() const;
		
		/* Set types */
		void setVertexType (Uint8 type);
		void setVertexType (
			Uint8 positionCount,
			Uint8 normalCount,
			Uint8 textureCount
		);
		void setElementType(Uint8 type);
		
		/* Load/Save geometry - throws CFR::Exception */
		void loadFromFile(const std::string &file);
		void   saveToFile(const std::string &file) const;
		
		/* Stream insertion/extraction */
		friend std::istream& ::operator>>(std::istream&, Geometry&);
		friend std::ostream& ::operator<<(std::ostream&, const Geometry&);
		
	private:
		
		Uint8 vertexType, elementType;
		
	};
	
	
	
	/*
		CFR geometry file format
		Byte order: little endian
		
		uint32_t  magic = 0x43465247;
		uint8_t   version = 1;
		uint32_t  elementCount; // Number of elements
		uint8_t   elementBytes; // Number of bytes per element (1, 2 or 4)
		uint8_t   elementType;  // Type of elements
		uint32_t  vertexCount;  // Number of vertices
		uint8_t   vertexBytes;  // Number of bytes per vertex
		uint8_t   vertexType;   // Type of vertices
		float     vertices[vertexCount  * vertexBytes / 4];
		uint8_t   elements[elementCount * elementBytes];
		
		Vertex type:
			(vertexType & 0b00000011) >> 0 = Number of floats for position
			(vertexType & 0b00001100) >> 2 = Number of floats for normal
			(vertexType & 0b00110000) >> 4 = Number of floats for texture
			(vertexType & 0b11000000) >> 6 = Unused
			
			Common types:
			47 - 0b00101111 - position[3] + normal [3] + texture[2]
			15 - 0b00001111 - position[3] + normal [3]
			35 - 0b00100011 - position[3] + texture[2]
			3  - 0b00000011 - position[3]
			
			vertexBytes can be calculated from vertexType, but not vice versa
		
		Element type:
			0 - points         (GL_POINTS)
			1 - lines          (GL_LINES)
			2 - line loop      (GL_LINE_LOOP)
			3 - line strip     (GL_LINE_STRIP)
			4 - triangles      (GL_TRIANGLES)
			5 - triangle strip (GL_TRIANGLE_STRIP)
			6 - triangle fan   (GL_TRIANGLE_FAN)
		
	*/
	
	
	
} // namespace CFR

#endif // _CFR_GEOMETRY_HPP_
