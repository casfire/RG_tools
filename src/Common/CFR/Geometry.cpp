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
using CFR::TYPE_BYTE;
using CFR::TYPE_UNSIGNED_BYTE;
using CFR::TYPE_SHORT;
using CFR::TYPE_UNSIGNED_SHORT;
using CFR::TYPE_FLOAT;
using CFR::TYPE_HALF_FLOAT;
using CFR::TYPE_DISABLE;



/* Float packing */

inline bool typeIsValid(Uint8 type) {
	switch (type) {
	case TYPE_BYTE:  case TYPE_UNSIGNED_BYTE:
	case TYPE_SHORT: case TYPE_UNSIGNED_SHORT:
	case TYPE_FLOAT: case TYPE_HALF_FLOAT:
	case TYPE_DISABLE:
		return true;
	default:
		return false;
	}
}

inline Uint32 typeGetSize(Uint8 type) {
	switch (type) {
	default:         case TYPE_DISABLE:        return 0;
	case TYPE_BYTE:  case TYPE_UNSIGNED_BYTE:  return 1;
	case TYPE_HALF_FLOAT:
	case TYPE_SHORT: case TYPE_UNSIGNED_SHORT: return 2;
	case TYPE_FLOAT:                           return 4;
	}
}

inline Uint32 packFloatFull  (float v) { return *reinterpret_cast<Uint32*>(&v); }
inline Uint16 packFloatHalf  (float v) { return glm::packHalf1x16 (v); }
inline Uint16 packFloatSShort(float v) { return glm::packSnorm1x16(v); }
inline Uint16 packFloatUShort(float v) { return glm::packUnorm1x16(v); }
inline Uint8  packFloatSByte (float v) { return glm::packSnorm1x8 (v); }
inline Uint8  packFloatUByte (float v) { return glm::packUnorm1x8 (v); }

inline float unpackFloatFull  (Uint32 v) { return *reinterpret_cast<float*>(&v); }
inline float unpackFloatHalf  (Uint16 v) { return glm::unpackHalf1x16 (v); }
inline float unpackFloatSShort(Uint16 v) { return glm::unpackSnorm1x16(v); }
inline float unpackFloatUShort(Uint16 v) { return glm::unpackUnorm1x16(v); }
inline float unpackFloatSByte (Uint8 v)  { return glm::unpackSnorm1x8 (v); }
inline float unpackFloatUByte (Uint8 v)  { return glm::unpackUnorm1x8 (v); }

inline Uint32 packFloat(float v, Uint8 type) {
	switch (type) {
	case TYPE_FLOAT:          return packFloatFull  (v);
	case TYPE_HALF_FLOAT:     return packFloatHalf  (v);
	case TYPE_SHORT:          return packFloatSShort(v);
	case TYPE_UNSIGNED_SHORT: return packFloatUShort(v);
	case TYPE_BYTE:           return packFloatSByte (v);
	case TYPE_UNSIGNED_BYTE:  return packFloatUByte (v);
	case TYPE_DISABLE:
	default: return 0;
	}
}

inline float unpackFloat(Uint32 v, Uint8 type) {
	switch (type) {
	case TYPE_FLOAT:          return unpackFloatFull  (v);
	case TYPE_HALF_FLOAT:     return unpackFloatHalf  (v);
	case TYPE_SHORT:          return unpackFloatSShort(v);
	case TYPE_UNSIGNED_SHORT: return unpackFloatUShort(v);
	case TYPE_BYTE:           return unpackFloatSByte (v);
	case TYPE_UNSIGNED_BYTE:  return unpackFloatUByte (v);
	default: return 0.f;
	}
}



/* CFR::Geometry */

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
		throw CFR::Exception("IO error: " + std::string(fail.what()));
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
		throw CFR::Exception("IO error: " + std::string(fail.what()));
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
	v.binormal.packX = packFloat(v.binormal.x, typeBinormal);
	v.binormal.packY = packFloat(v.binormal.y, typeBinormal);
	v.binormal.packZ = packFloat(v.binormal.z, typeBinormal);
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
		throw CFR::Exception("Invalid position type.");
	}
	typePosition = type;
}

void Geometry::setTypeTexcoord(Uint8 type)
{
	if (!typeIsValid(type)) {
		throw CFR::Exception("Invalid texcoord type.");
	}
	typeTexcoord = type;
}

void Geometry::setTypeNormal(Uint8 type)
{
	if (!typeIsValid(type)) {
		throw CFR::Exception("Invalid normal type.");
	}
	typeNormal = type;
}

void Geometry::setTypeTangent(Uint8 type)
{
	if (!typeIsValid(type)) {
		throw CFR::Exception("Invalid tangent type.");
	}
	typeTangent = type;
}

void Geometry::setTypeBinormal(Uint8 type)
{
	if (!typeIsValid(type)) {
		throw CFR::Exception("Invalid binormal type.");
	}
	typeBinormal = type;
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

Uint8 Geometry::getTypeBinormal() const
{
	return typeBinormal;
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
	switch (typeGetSize(type)) {
	case 1: return unpackFloat(read8 (in), type);
	case 2: return unpackFloat(read16(in), type);
	case 4: return unpackFloat(read32(in), type);
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
	switch (typeGetSize(type)) {
	case 1: write8 (out, packFloat(v, type));
	case 2: write16(out, packFloat(v, type));
	case 4: write32(out, packFloat(v, type));
	}
}

std::istream& operator>>(std::istream& in, CFR::Geometry& obj)
{
	if (read32(in) != 0x47524643) {
		throw CFR::Exception("Invalid magic number.");
	} else if (read32(in) != 1) {
		throw CFR::Exception("Invalid version.");
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
	Uint8 offsetBinormal = read8(in);
	Uint8   typeBinormal = read8(in);
	in.ignore(4);
	
	if (bytesPerElement == 0 || bytesPerElement == 3 || bytesPerElement > 4) {
		throw CFR::Exception("Invalid bytes per element.");
	}
	
	obj.clear();
	obj.setTypePosition(typePosition);
	obj.setTypeTexcoord(typeTexcoord);
	obj.setTypeNormal  (typeNormal);
	obj.setTypeTangent (typeTangent);
	obj.setTypeBinormal(typeBinormal);
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
		vertex.binormal.x = readFloat(in, typeBinormal);
		vertex.binormal.y = readFloat(in, typeBinormal);
		vertex.binormal.z = readFloat(in, typeBinormal);
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
	(void) offsetBinormal;
	
	return in;
}

std::ostream& operator<<(std::ostream& out, const CFR::Geometry& obj)
{
	if (obj.getVertexCount() > 0xFFFFFFFF) {
		throw CFR::Exception("Too many vertices.");
	} else if (obj.getElementCount() > 0xFFFFFFFF) {
		throw CFR::Exception("Too many elements.");
	}
	
	Uint8 sizePosition = 3 * typeGetSize(obj.typePosition);
	Uint8 sizeTexcoord = 2 * typeGetSize(obj.typeTexcoord);
	Uint8 sizeNormal   = 3 * typeGetSize(obj.typeNormal);
	Uint8 sizeTangent  = 3 * typeGetSize(obj.typeTangent);
	Uint8 sizeBinormal = 3 * typeGetSize(obj.typeBinormal);
	
	Uint8 offsetPosition = 0;
	Uint8 offsetTexcoord = offsetPosition + sizePosition;
	Uint8 offsetNormal   = offsetTexcoord + sizeTexcoord;
	Uint8 offsetTangent  = offsetNormal   + sizeNormal;
	Uint8 offsetBinormal = offsetTangent  + sizeBinormal;
	
	Uint32 countElements   = obj.getElementCount();
	Uint32 countVertices   = obj.getVertexCount();
	Uint8  bytesPerVertex =
		sizePosition + sizeTexcoord +
		sizeNormal   + sizeTangent  + sizeBinormal;
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
	write8 (out,   offsetBinormal);
	write8 (out, obj.typeBinormal);
	write32(out, 0);
	
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
		writeFloat(out, vertex.binormal.x, obj.typeBinormal);
		writeFloat(out, vertex.binormal.y, obj.typeBinormal);
		writeFloat(out, vertex.binormal.z, obj.typeBinormal);
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
