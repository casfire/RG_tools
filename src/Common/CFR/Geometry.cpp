#include "Geometry.hpp"
#include <fstream>
#include <glm/gtc/packing.hpp>

using CFR::BaseGeometry;
using CFR::Geometry;
using CFR::size_type;
using CFR::Uint8;
using CFR::Uint16;
using CFR::Uint32;
using CFR::Vertex;
using CFR::Exception;
using CFR::TYPE_DISABLE;
using CFR::TYPE_FLOAT;
using CFR::TYPE_HALF_FLOAT;
using CFR::TYPE_SHORT;
using CFR::TYPE_UNSIGNED_SHORT;
using CFR::TYPE_BYTE;
using CFR::TYPE_UNSIGNED_BYTE;
using CFR::TYPE_NORM_SHORT;
using CFR::TYPE_NORM_UNSIGNED_SHORT;
using CFR::TYPE_NORM_BYTE;
using CFR::TYPE_NORM_UNSIGNED_BYTE;
typedef std::int8_t  Sint8;
typedef std::int16_t Sint16;
typedef std::int32_t Sint32;
using glm::round;
using glm::clamp;



/* Float packing */

inline bool typeIsValid(Uint8 type) {
	switch (type) {
	case TYPE_DISABLE:
	case TYPE_FLOAT:
	case TYPE_HALF_FLOAT:
	case TYPE_SHORT:
	case TYPE_UNSIGNED_SHORT:
	case TYPE_BYTE:
	case TYPE_UNSIGNED_BYTE:
		return true;
	default:
		return false;
	}
}

inline Uint32 typeGetSize(Uint8 type) {
	switch (type) {
	default:
	case TYPE_DISABLE:
		return 0;
	case TYPE_BYTE:
	case TYPE_UNSIGNED_BYTE:
		return 1;
	case TYPE_HALF_FLOAT:
	case TYPE_SHORT:
	case TYPE_UNSIGNED_SHORT:
		return 2;
	case TYPE_FLOAT:
		return 4;
	}
}

inline Uint32 packFull(float v) { return *reinterpret_cast<Uint32*>(&v); }
inline Uint16 packHalf(float v) { return glm::packHalf1x16 (v); }

inline float unpackFull(Uint32 v) { return *reinterpret_cast<float*>(&v); }
inline float unpackHalf(Uint16 v) { return glm::unpackHalf1x16 (v); }

inline Uint16 packNormSShort(float v) { return glm::packSnorm1x16(v); }
inline Uint16 packNormUShort(float v) { return glm::packUnorm1x16(v); }
inline Uint8  packNormSByte (float v) { return glm::packSnorm1x8 (v); }
inline Uint8  packNormUByte (float v) { return glm::packUnorm1x8 (v); }

inline float unpackNormSShort(Uint16 v) { return glm::unpackSnorm1x16(v); }
inline float unpackNormUShort(Uint16 v) { return glm::unpackUnorm1x16(v); }
inline float unpackNormSByte (Uint8 v)  { return glm::unpackSnorm1x8 (v); }
inline float unpackNormUByte (Uint8 v)  { return glm::unpackUnorm1x8 (v); }

inline Uint16 packSShort(float v) { return static_cast<Sint16>(round(clamp(v, -32767.f, 32767.f))); }
inline Uint16 packUShort(float v) { return static_cast<Uint16>(round(clamp(v, +0.f,     65535.f))); }
inline Uint8  packSByte (float v) { return static_cast<Sint8 >(round(clamp(v, -127.f,   127.f  ))); }
inline Uint8  packUByte (float v) { return static_cast<Uint8 >(round(clamp(v, +0.f,     255.f  ))); }

inline float unpackSShort(Sint16 v) { return clamp(static_cast<float>(v), -32767.f, 32767.f); }
inline float unpackUShort(Uint16 v) { return clamp(static_cast<float>(v), +0.f,     65535.f); }
inline float unpackSByte (Sint8  v) { return clamp(static_cast<float>(v), -127.f,   127.f  ); }
inline float unpackUByte (Uint8  v) { return clamp(static_cast<float>(v), +0.f,     255.f  ); }

inline Uint32 packFloat(float v, Uint8 type) {
	switch (type) {
	case TYPE_DISABLE: default:    return 0;
	case TYPE_FLOAT:               return packFull  (v);
	case TYPE_HALF_FLOAT:          return packHalf  (v);
	case TYPE_SHORT:               return packSShort(v);
	case TYPE_UNSIGNED_SHORT:      return packUShort(v);
	case TYPE_BYTE:                return packSByte (v);
	case TYPE_UNSIGNED_BYTE:       return packUByte (v);
	case TYPE_NORM_SHORT:          return packNormSShort(v);
	case TYPE_NORM_UNSIGNED_SHORT: return packNormUShort(v);
	case TYPE_NORM_BYTE:           return packNormSByte (v);
	case TYPE_NORM_UNSIGNED_BYTE:  return packNormUByte (v);
	}
}



/* Geometry */

Geometry::Geometry()
: BaseGeometry()
{}

Geometry::Geometry(const BaseGeometry &copy)
: BaseGeometry(copy)
{}

void Geometry::loadFromFile(const std::string &file)
{
	try {
		std::ifstream stream;
		stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		stream.open(file, std::ios::binary);
		stream >> *this;
		stream.close();
	} catch (std::ios::failure &fail) {
		throw Exception("IO error: " + std::string(fail.what()));
	}
}

void Geometry::saveToFile(const std::string &file) const
{
	try {
		std::ofstream stream;
		stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		stream.open(file, std::ios::binary);
		stream << *this;
		stream.close();
	} catch (std::ios::failure &fail) {
		throw Exception("IO error: " + std::string(fail.what()));
	}
}

Vertex Geometry::compressVertex(Vertex v)
{
	v.position.packX = packFloat(v.position.x, typePosition);
	v.position.packY = packFloat(v.position.y, typePosition);
	v.position.packZ = packFloat(v.position.z, typePosition);
	v.texcoord.packX = packFloat(v.texcoord.x, typeTexcoord);
	v.texcoord.packY = packFloat(v.texcoord.y, typeTexcoord);
	v.normal.packX   = packFloat(v.normal.x,   typeNormal);
	v.normal.packY   = packFloat(v.normal.y,   typeNormal);
	v.normal.packZ   = packFloat(v.normal.z,   typeNormal);
	v.tangent.packX  = packFloat(v.tangent.x,  typeTangent);
	v.tangent.packY  = packFloat(v.tangent.y,  typeTangent);
	v.tangent.packZ  = packFloat(v.tangent.z,  typeTangent);
	v.tangent.packW  = packFloat(v.tangent.w,  typeTangent);
	return v;
}

Uint32 Geometry::pushVertex(const Vertex &v)
{
	return BaseGeometry::pushVertex(compressVertex(v));
}

Uint32 Geometry::addVertex(const Vertex &v)
{
	return BaseGeometry::addVertex(compressVertex(v));
}

void Geometry::setTypePosition(Uint8 type)
{
	if (!typeIsValid(type)) {
		throw Exception("Invalid position type.");
	}
	typePosition = type;
}

void Geometry::setTypeTexcoord(Uint8 type)
{
	if (!typeIsValid(type)) {
		throw Exception("Invalid texcoord type.");
	}
	typeTexcoord = type;
}

void Geometry::setTypeNormal(Uint8 type)
{
	if (!typeIsValid(type)) {
		throw Exception("Invalid normal type.");
	}
	typeNormal = type;
}

void Geometry::setTypeTangent(Uint8 type)
{
	if (!typeIsValid(type)) {
		throw Exception("Invalid tangent type.");
	}
	typeTangent = type;
}

Uint8 Geometry::getTypePosition() const
{
	return typePosition;
}

Uint8 Geometry::getTypeTexcoord() const
{
	return typeTexcoord;
}

Uint8 Geometry::getTypeNormal() const
{
	return typeNormal;
}

Uint8 Geometry::getTypeTangent() const
{
	return typeTangent;
}



/* Stream insertion/extraction */

inline uint8_t read8(std::istream &in) {
	return static_cast<uint8_t>(in.get());
}

inline uint16_t read16(std::istream &in) {
	uint16_t t = 0;
	t |= static_cast<uint16_t>(in.get()) << 0;
	t |= static_cast<uint16_t>(in.get()) << 8;
	return t;
}

inline uint32_t read32(std::istream &in) {
	uint32_t t = 0;
	t |= static_cast<uint32_t>(in.get()) << 0;
	t |= static_cast<uint32_t>(in.get()) << 8;
	t |= static_cast<uint32_t>(in.get()) << 16;
	t |= static_cast<uint32_t>(in.get()) << 24;
	return t;
}

inline float readFloat(std::istream &in, Uint8 type) {
	switch (type) {
	case TYPE_FLOAT:               return unpackFull      (read32(in));
	case TYPE_HALF_FLOAT:          return unpackHalf      (read16(in));
	case TYPE_SHORT:               return unpackSShort    (read16(in));
	case TYPE_UNSIGNED_SHORT:      return unpackUShort    (read16(in));
	case TYPE_BYTE:                return unpackSByte     (read8 (in));
	case TYPE_UNSIGNED_BYTE:       return unpackUByte     (read8 (in));
	case TYPE_NORM_SHORT:          return unpackNormSShort(read16(in));
	case TYPE_NORM_UNSIGNED_SHORT: return unpackNormUShort(read16(in));
	case TYPE_NORM_BYTE:           return unpackNormSByte (read8 (in));
	case TYPE_NORM_UNSIGNED_BYTE:  return unpackNormUByte (read8 (in));
	default: return 0.f;
	}
}

inline void write8(std::ostream &out, uint8_t v) {
	out.put(v);
}

inline void write16(std::ostream &out, uint16_t v) {
	write8(out, static_cast<uint8_t>((v >> 0) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 8) & 0xFF));
}

inline void write32(std::ostream &out, uint32_t v) {
	write8(out, static_cast<uint8_t>((v >> 0 ) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 8 ) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 16) & 0xFF));
	write8(out, static_cast<uint8_t>((v >> 24) & 0xFF));
}

inline void writeFloat(std::ostream &out, float v, Uint8 type) {
	switch (type) {
	case TYPE_FLOAT:               write32(out, packFull      (v)); break;
	case TYPE_HALF_FLOAT:          write16(out, packHalf      (v)); break;
	case TYPE_SHORT:               write16(out, packSShort    (v)); break;
	case TYPE_UNSIGNED_SHORT:      write16(out, packUShort    (v)); break;
	case TYPE_BYTE:                write8 (out, packSByte     (v)); break;
	case TYPE_UNSIGNED_BYTE:       write8 (out, packUByte     (v)); break;
	case TYPE_NORM_SHORT:          write16(out, packNormSShort(v)); break;
	case TYPE_NORM_UNSIGNED_SHORT: write16(out, packNormUShort(v)); break;
	case TYPE_NORM_BYTE:           write8 (out, packNormSByte (v)); break;
	case TYPE_NORM_UNSIGNED_BYTE:  write8 (out, packNormUByte (v)); break;
	}
}

std::istream& operator>>(std::istream& in, Geometry& obj)
{
	if (read32(in) != 0x47524643) {
		throw Exception("Invalid magic number.");
	} else if (read32(in) != 1) {
		throw Exception("Invalid version.");
	}
	
	Uint32 countElements   = read32(in);
	Uint32 countVertices   = read32(in);
	Uint8  bytesPerVertex  = read8 (in);
	Uint8  bytesPerElement = read8 (in);
	
	Uint8 offsetPosition = read8(in);
	Uint8   typePosition = read8(in);
	Uint8 offsetTexcoord = read8(in);
	Uint8   typeTexcoord = read8(in);
	Uint8 offsetNormal   = read8(in);
	Uint8   typeNormal   = read8(in);
	Uint8 offsetTangent  = read8(in);
	Uint8   typeTangent  = read8(in);
	in.ignore(4);
	
	if (bytesPerElement == 0 || bytesPerElement == 3 || bytesPerElement > 4) {
		throw Exception("Invalid bytes per element.");
	}
	
	obj.clear();
	obj.setTypePosition(typePosition);
	obj.setTypeTexcoord(typeTexcoord);
	obj.setTypeNormal  (typeNormal);
	obj.setTypeTangent (typeTangent);
	obj.reserveElements(countElements);
	obj.reserveVertices(countVertices);
	
	for (Uint32 i = 0; i < countVertices; i++) {
		Vertex vertex;
		vertex.position.x = readFloat(in, typePosition);
		vertex.position.y = readFloat(in, typePosition);
		vertex.position.z = readFloat(in, typePosition);
		vertex.texcoord.x = readFloat(in, typeTexcoord);
		vertex.texcoord.y = readFloat(in, typeTexcoord);
		vertex.normal.x   = readFloat(in, typeNormal);
		vertex.normal.y   = readFloat(in, typeNormal);
		vertex.normal.z   = readFloat(in, typeNormal);
		vertex.tangent.x  = readFloat(in, typeTangent);
		vertex.tangent.y  = readFloat(in, typeTangent);
		vertex.tangent.z  = readFloat(in, typeTangent);
		vertex.tangent.w  = readFloat(in, typeTangent);
		obj.pushVertex(vertex);
	}
	
	switch (bytesPerElement) {
	case 1:
		for (Uint32 i = 0; i < countElements; i++) obj.addElement(read8(in));
		break;
	case 2:
		for (Uint32 i = 0; i < countElements; i++) obj.addElement(read16(in));
		break;
	case 4:
		for (Uint32 i = 0; i < countElements; i++) obj.addElement(read32(in));
		break;
	}
	
	(void) bytesPerVertex;
	(void) offsetPosition;
	(void) offsetTexcoord;
	(void) offsetNormal;
	(void) offsetTangent;
	
	return in;
}

std::ostream& operator<<(std::ostream& out, const Geometry& obj)
{
	if (obj.getVertexCount() > 0xFFFFFFFF) {
		throw Exception("Too many vertices.");
	} else if (obj.getElementCount() > 0xFFFFFFFF) {
		throw Exception("Too many elements.");
	}
	
	Uint8 sizePosition = 3 * typeGetSize(obj.typePosition);
	Uint8 sizeTexcoord = 2 * typeGetSize(obj.typeTexcoord);
	Uint8 sizeNormal   = 3 * typeGetSize(obj.typeNormal);
	Uint8 sizeTangent  = 4 * typeGetSize(obj.typeTangent);
	
	Uint8 offsetPosition = 0;
	Uint8 offsetTexcoord = offsetPosition + sizePosition;
	Uint8 offsetNormal   = offsetTexcoord + sizeTexcoord;
	Uint8 offsetTangent  = offsetNormal   + sizeNormal;
	
	Uint32 countElements   = obj.getElementCount();
	Uint32 countVertices   = obj.getVertexCount();
	Uint8  bytesPerVertex =
		sizePosition + sizeTexcoord +
		sizeNormal   + sizeTangent;
	Uint8  bytesPerElement = 4;
	
	if (obj.getElementMax() <= 0xFF )  bytesPerElement = 1;
	if (obj.getElementMax() <= 0xFFFF) bytesPerElement = 2;
	
	write32(out, 0x47524643);
	write32(out, 1);
	write32(out, countElements);
	write32(out, countVertices);
	write8 (out, bytesPerVertex);
	write8 (out, bytesPerElement);
	
	write8 (out,   offsetPosition);
	write8 (out, obj.typePosition);
	write8 (out,   offsetTexcoord);
	write8 (out, obj.typeTexcoord);
	write8 (out,   offsetNormal);
	write8 (out, obj.typeNormal);
	write8 (out,   offsetTangent);
	write8 (out, obj.typeTangent);
	for (int i = 0; i < 6; i++) write8 (out, 0);
	
	size_type vertexCount = obj.getVertexCount();
	for (size_type i = 0; i < vertexCount; i++) {
		const Vertex& vertex = obj.getVertex(i);
		writeFloat(out, vertex.position.x, obj.typePosition);
		writeFloat(out, vertex.position.y, obj.typePosition);
		writeFloat(out, vertex.position.z, obj.typePosition);
		writeFloat(out, vertex.texcoord.x, obj.typeTexcoord);
		writeFloat(out, vertex.texcoord.y, obj.typeTexcoord);
		writeFloat(out, vertex.normal.x,   obj.typeNormal);
		writeFloat(out, vertex.normal.y,   obj.typeNormal);
		writeFloat(out, vertex.normal.z,   obj.typeNormal);
		writeFloat(out, vertex.tangent.x,  obj.typeTangent);
		writeFloat(out, vertex.tangent.y,  obj.typeTangent);
		writeFloat(out, vertex.tangent.z,  obj.typeTangent);
		writeFloat(out, vertex.tangent.w,  obj.typeTangent);
	}
	
	size_type elementCount = obj.getElementCount();
	switch (bytesPerElement) {
	case 1:
		for (size_type i = 0; i < elementCount; i++) {
			write8(out, obj.getElement(i));
		}
		break;
	case 2:
		for (size_type i = 0; i < elementCount; i++) {
			write16(out, obj.getElement(i));
		}
		break;
	case 4:
		for (size_type i = 0; i < elementCount; i++) {
			write32(out, obj.getElement(i));
		}
		break;
	}
	
	return out;
}
