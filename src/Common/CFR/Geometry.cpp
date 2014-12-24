#include "Geometry.hpp"
#include <fstream>

using CFR::size_type;
using CFR::Uint8;
using CFR::Uint16;
using CFR::Uint32;
using std::uint32_t;
using std::uint16_t;
using std::uint8_t;


/* CFR::Geometry */

CFR::Geometry::Geometry(Uint8 vertexType, Uint8 elementType)
: CFR::BaseGeometry(), vertexType(vertexType), elementType(elementType)
{}

CFR::Geometry::Geometry(const Geometry &copy)
: CFR::BaseGeometry(copy),
  vertexType(copy.vertexType), elementType(copy.elementType)
{}

Uint8 CFR::Geometry::getVertexType() const
{
	return vertexType;
}

Uint8 CFR::Geometry::getElementType() const
{
	return elementType;
}

void CFR::Geometry::setVertexType(Uint8 type)
{
	vertexType = type;
}

void CFR::Geometry::setElementType(Uint8 type)
{
	elementType = type;
}

void CFR::Geometry::setVertexType(
	Uint8 positionCount,
	Uint8 normalCount,
	Uint8 textureCount)
{
	if (positionCount > 3) throw CFR::Exception("Invalid position count.");
	if (normalCount   > 3) throw CFR::Exception("Invalid normal count.");
	if (textureCount  > 3) throw CFR::Exception("Invalid texture count.");
	vertexType = 
		  (positionCount & 0b00000011) << 0
		| (normalCount   & 0b00000011) << 2
		| (textureCount  & 0b00000011) << 4;
}

void CFR::Geometry::loadFromFile(const std::string &file)
{
	try {
		std::ifstream stream;
		stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		stream.open(file, std::ios::binary);
		stream >> *this;
		stream.close();
	} catch (std::ios::failure &fail) {
		throw CFR::Exception("IO error: " + std::string(fail.what()));
	}
}

void CFR::Geometry::saveToFile(const std::string &file) const
{
	try {
		std::ofstream stream;
		stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		stream.open(file, std::ios::binary);
		stream << *this;
		stream.close();
	} catch (std::ios::failure &fail) {
		throw CFR::Exception("IO error: " + std::string(fail.what()));
	}
}



/* Stream insertion/extraction */

inline uint8_t read8(std::istream &in) {
	return static_cast<uint8_t>(in.get());
}

inline uint16_t read16(std::istream &in) {
	uint16_t t = 0;
	t |= static_cast<uint16_t>(in.get()) << 8;
	t |= static_cast<uint16_t>(in.get());
	return t;
}

inline uint32_t read32(std::istream &in) {
	uint32_t t = 0;
	t |= static_cast<uint32_t>(in.get()) << 24;
	t |= static_cast<uint32_t>(in.get()) << 16;
	t |= static_cast<uint32_t>(in.get()) << 8;
	t |= static_cast<uint32_t>(in.get());
	return t;
}

inline float readFloat(std::istream &in) {
	uint32_t t = read32(in);
	return *reinterpret_cast<float*>(&t);
}

inline void write8(std::ostream &out, uint8_t v) {
	out.put(v);
}

inline void write16(std::ostream &out, uint16_t v) {
	write8(out, static_cast<uint8_t>((v >> 8) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 0) & 0xFF));
}

inline void write32(std::ostream &out, uint32_t v) {
	write8(out, static_cast<uint8_t>((v >> 24) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 16) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 8 ) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 0 ) & 0xFF));
}

inline void writeFloat(std::ostream &out, float v) {
	uint32_t t = *reinterpret_cast<uint32_t*>(&v);
	write32(out, t);
}

std::istream& operator>>(std::istream& in, CFR::Geometry& obj)
{
	if (read32(in) != 0x43465247) {
		throw CFR::Exception("Invalid magic number.");
	} else if (read8(in) != 1) {
		throw CFR::Exception("Invalid version.");
	}
	uint32_t elementCount = read32(in);
	uint8_t  elementBytes = read8 (in);
	uint8_t  elementType  = read8 (in);
	uint32_t vertexCount  = read32(in);
	uint8_t  vertexBytes  = read8 (in);
	uint8_t  vertexType   = read8 (in);
	if (elementBytes == 0 || elementBytes == 3 || elementBytes > 4) {
		throw CFR::Exception("Invalid number of bytes per element.");
	} else if (elementType > 6) {
		throw CFR::Exception("Invalid element type.");
	}
	uint8_t   positionCount = (vertexType & 0b00000011) >> 0;
	uint8_t   normalCount   = (vertexType & 0b00001100) >> 2;
	uint8_t   textureCount  = (vertexType & 0b00110000) >> 4;
	if (4 * (positionCount + normalCount + textureCount) != vertexBytes) {
		throw CFR::Exception("Invalid number of bytes per vertex.");
	}
	
	obj.clear();
	obj.setVertexType(vertexType);
	obj.setElementType(elementType);
	obj.reserveVertices(vertexCount);
	obj.reserveElements(elementCount);
	
	for (uint32_t i = 0; i < vertexCount; i++) {
		CFR::Vertex vertex;
		for (uint8_t c = 0; c < positionCount; c++) vertex.position[c] = readFloat(in);
		for (uint8_t c = 0; c < normalCount;   c++) vertex.normal[c]   = readFloat(in);
		for (uint8_t c = 0; c < textureCount;  c++) vertex.texture[c]  = readFloat(in);
		obj.addVertex(vertex);
	}
	
	switch (elementBytes) {
	case 1: for (uint32_t i = 0; i < elementCount; i++) obj.addElement(read8 (in)); break;
	case 2: for (uint32_t i = 0; i < elementCount; i++) obj.addElement(read16(in)); break;
	case 4: for (uint32_t i = 0; i < elementCount; i++) obj.addElement(read32(in)); break;
	}
	
	return in;
}

std::ostream& operator<<(std::ostream& out, const CFR::Geometry& obj)
{
	size_type elementCount = obj.getElementCount();
	uint8_t   elementType  = obj.getElementType();
	uint8_t   elementBytes = 4;
	if (obj.getElementMax() <= 0xFF  ) elementBytes = 1;
	if (obj.getElementMax() <= 0xFFFF) elementBytes = 2;
	
	size_type vertexCount = obj.getVertexCount();
	uint8_t   vertexType  = obj.getVertexType() & 0b00111111;
	uint8_t   positionCount = (vertexType & 0b00000011) >> 0;
	uint8_t   normalCount   = (vertexType & 0b00001100) >> 2;
	uint8_t   textureCount  = (vertexType & 0b00110000) >> 4;
	uint8_t   vertexBytes = 4 * (positionCount + normalCount + textureCount);
	
	if (elementCount > 0xFFFFFFFF) {
		throw CFR::Exception("Number of elements is too big.");
	} else if (vertexCount > 0xFFFFFFFF) {
		throw CFR::Exception("Number of vertices is too big.");
	} else if (elementType > 6) {
		throw CFR::Exception("Invalid element type.");
	} else if (vertexBytes == 0) {
		throw CFR::Exception("There are no bytes per vertex.");
	}
	
	write32(out, 0x43465247);
	write8 (out, 1);
	write32(out, elementCount);
	write8 (out, elementBytes);
	write8 (out, elementType);
	write32(out, vertexCount);
	write8 (out, vertexBytes);
	write8 (out, vertexType);
	
	for (size_type i = 0; i < vertexCount; i++) {
		const CFR::Vertex& vertex = obj.getVertex(i);
		for (uint8_t c = 0; c < positionCount; c++) writeFloat(out, vertex.position[c]);
		for (uint8_t c = 0; c < normalCount;   c++) writeFloat(out, vertex.normal[c]  );
		for (uint8_t c = 0; c < textureCount;  c++) writeFloat(out, vertex.texture[c] );
	}
	
	switch (elementBytes) {
	case 1: for (size_type i = 0; i < elementCount; i++) write8 (out, obj.getElement(i)); break;
	case 2: for (size_type i = 0; i < elementCount; i++) write16(out, obj.getElement(i)); break;
	case 4: for (size_type i = 0; i < elementCount; i++) write32(out, obj.getElement(i)); break;
	}
	
	return out;
}
