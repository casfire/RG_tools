#pragma once
#ifndef _CFR_GEOMETRY_HPP_
#define _CFR_GEOMETRY_HPP_

#include "Common.hpp"
#include "BaseGeometry.hpp"
#include <string>
#include <istream>
#include <ostream>

std::istream& operator>>(std::istream& in, CFR::Geometry& obj);
std::ostream& operator<<(std::ostream& out, const CFR::Geometry& obj);

namespace CFR {
	
	
	
	/* Attribute export types */
	static const Uint8 TYPE_DISABLE             = 0b11111111;
	static const Uint8 TYPE_FLOAT               = 0b00000110;
	static const Uint8 TYPE_HALF_FLOAT          = 0b00001011;
	static const Uint8 TYPE_SHORT               = 0b00000010;
	static const Uint8 TYPE_UNSIGNED_SHORT      = 0b00000011;
	static const Uint8 TYPE_BYTE                = 0b00000000;
	static const Uint8 TYPE_UNSIGNED_BYTE       = 0b00000001;
	static const Uint8 TYPE_NORM_SHORT          = 0b10000010;
	static const Uint8 TYPE_NORM_UNSIGNED_SHORT = 0b10000011;
	static const Uint8 TYPE_NORM_BYTE           = 0b10000000;
	static const Uint8 TYPE_NORM_UNSIGNED_BYTE  = 0b10000001;
	
	
	
	/* CFR Geometry */
	class Geometry : public BaseGeometry {
	public:
		
		/* Create */
		Geometry();
		Geometry(const BaseGeometry &copy);
		
		/* Load/Save geometry - throws CFR::Exception */
		void loadFromFile(const std::string &file);
		void   saveToFile(const std::string &file) const;
		
		/* Override vertex insertion */
		Uint32 pushVertex(const Vertex &v) override;
		Uint32 addVertex (const Vertex &v) override;
		
		/* Set attribute export type */
		void setTypePosition(Uint8 type);
		void setTypeTexcoord(Uint8 type);
		void setTypeNormal  (Uint8 type);
		void setTypeTangent (Uint8 type);
		
		/* Get attribute export type */
		Uint8 getTypePosition() const;
		Uint8 getTypeTexcoord() const;
		Uint8 getTypeNormal()   const;
		Uint8 getTypeTangent()  const;
		
		/* Stream insertion/extraction */
		friend std::istream& ::operator>>(std::istream&, Geometry &obj);
		friend std::ostream& ::operator<<(std::ostream&, const Geometry &obj);
		
	private:
		
		Uint8 typePosition = TYPE_FLOAT;
		Uint8 typeTexcoord = TYPE_HALF_FLOAT;
		Uint8 typeNormal   = TYPE_HALF_FLOAT;
		Uint8 typeTangent  = TYPE_HALF_FLOAT;
		Uint8 typeBinormal = TYPE_HALF_FLOAT;
		
		Vertex compressVertex(Vertex v);
		
	};
	
	
	
	/*
		CFR Geometry file format
		Byte order: little endian
		
		Uint32 magic = 0x47524643; // CFRG
		Uint32 version = 1;
		Uint32 countElements;     // Number of elements
		Uint32 countVertices;     // Number of vertices
		Uint8  bytesPerVertex;    // Bytes per vertex
		Uint8  bytesPerElement;   // Bytes per element (1, 2 or 4)
		Uint8  attribPosition[2]; // Vertex position (3 dimensions)
		Uint8  attribTexCoord[2]; // Vertex UV       (2 dimensions)
		Uint8  attribNormal  [2]; // Vertex normal   (3 dimensions)
		Uint8  attribTangent [2]; // Vertex tangent  (4 dimensions)
		Uint8  unused[6];
		Uint8  vertices[countVertices * bytesPerVertex ];
		Uint8  elements[countElements * bytesPerElement];
		
		Attributes:
			attribute[0] = Byte offset within a vertex
			attribute[1] = Attribute type
			If offset or type is 0xFF, the attribute isn't used
		
		Attribute type:
			Normalize     = type & 0b10000000
			Variable type = type & 0b01111111
		
		Attribute variable types:
			0  - Signed   byte  (GL_BYTE)
			1  - Unsigned byte  (GL_UNSIGNED_BYTE)
			2  - Signed   short (GL_SHORT)
			3  - Unsigned short (GL_UNSIGNED_SHORT)
			4  - Signed   int   (GL_INT)
			5  - Unsigned int   (GL_UNSIGNED_INT)
			6  - Float          (GL_FLOAT)
			10 - Double         (GL_DOUBLE)
			11 - Half float     (GL_HALF_FLOAT)
		
		Tangent space:
			Fourth tangent dimension is either 1 or -1
			Binormal = cross(tangent.xyz, normal) * tangent.w
		
	*/
	
	
	
} // namespace CFR

#endif // _CFR_GEOMETRY_HPP_
