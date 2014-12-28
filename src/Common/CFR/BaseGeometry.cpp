#include "BaseGeometry.hpp"

using CFR::BaseGeometry;
using CFR::size_type;
using CFR::Uint8;
using CFR::Uint16;
using CFR::Uint32;
using CFR::Vertex;

BaseGeometry::BaseGeometry()
: elementMax(0)
{}

BaseGeometry::BaseGeometry(const BaseGeometry &copy)
: vertices(copy.vertices), vertexElements(copy.vertexElements), 
  elements(copy.elements), elementMax(copy.elementMax)
{}

void BaseGeometry::addElement(Uint32 element)
{
	if (element >= vertices.size()) {
		throw Exception("Element out of range.");
	}
	elements.push_back(element);
	if (elementMax < element) elementMax = element;
}

Uint32 BaseGeometry::pushVertex(const Vertex &v)
{
	Uint32 index = static_cast<Uint32>(vertices.size());
	vertexElements[v] = index;
	vertices.push_back(v);
	return index;
}

Uint32 BaseGeometry::addVertex(const Vertex &v)
{
	std::unordered_map<Vertex, Uint32>::iterator i
		= vertexElements.find(v);
	if (i != vertexElements.end()) return i->second;
	return pushVertex(v);
}

void BaseGeometry::reserveVertices(size_type count)
{
	vertexElements.reserve(count);
	vertices.reserve(count);
}

void BaseGeometry::reserveElements(size_type count)
{
	elements.reserve(count);
}

void BaseGeometry::clear()
{
	elements.clear();
	elementMax = 0;
	vertices.clear();
	vertexElements.clear();
}

void BaseGeometry::recalculate()
{
	std::vector<Vertex> vertexCopy (vertices);
	std::vector<Uint32> elementCopy(elements);
	clear();
	reserveVertices(vertexCopy.size());
	reserveElements(elementCopy.size());
	for (std::size_t i = 0; i < elementCopy.size(); i++) {
		addElement(addVertex(vertexCopy[elementCopy[i]]));
	}
}

bool BaseGeometry::empty() const
{
	return vertices.empty() && elements.empty();
}

size_type BaseGeometry::getVertexCount() const
{
	return vertices.size();
}

size_type BaseGeometry::getElementCount() const
{
	return elements.size();
}

Uint32 BaseGeometry::getElementMax() const
{
	return elementMax;
}

const Vertex& BaseGeometry::getVertex(size_type index) const
{
	return vertices[index];
}

const Uint32& BaseGeometry::getElement(size_type index) const
{
	return elements[index];
}
