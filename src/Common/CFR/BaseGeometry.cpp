#include "BaseGeometry.hpp"

using CFR::size_type;
using CFR::Uint32;

CFR::BaseGeometry::BaseGeometry()
: elementMax(0)
{}

CFR::BaseGeometry::BaseGeometry(const BaseGeometry &copy)
: vertices(copy.vertices), elements(copy.elements), elementMax(copy.elementMax)
{}

void CFR::BaseGeometry::addElement(Uint32 element)
{
	if (element <= vertices.size()) {
		throw CFR::Exception("Element out of range.");
	}
	elements.push_back(element);
	if (elementMax < element) elementMax = element;
}

void CFR::BaseGeometry::addVertex(const CFR::Vertex &vertex)
{
	vertices.push_back(vertex);
}

void CFR::BaseGeometry::addVertex(
	float px, float py, float pz,
	float nx, float ny, float nz,
	float tx, float ty, float tz)
{
	addVertex(CFR::Vertex(px, py, pz, nx, ny, nz, tx, ty, tz));
}

void CFR::BaseGeometry::reserveVertices(size_type count)
{
	vertices.reserve(count);
}

void CFR::BaseGeometry::reserveElements(size_type count)
{
	elements.reserve(count);
}

void CFR::BaseGeometry::clear()
{
	elements.clear();
	elementMax = 0;
	vertices.clear();
}

size_type CFR::BaseGeometry::getVertexCount() const
{
	return vertices.size();
}

size_type CFR::BaseGeometry::getElementCount() const
{
	return elements.size();
}

Uint32 CFR::BaseGeometry::getElementMax() const
{
	return elementMax;
}

const CFR::Vertex& CFR::BaseGeometry::getVertex(size_type index) const
{
	return vertices[index];
}

const Uint32& CFR::BaseGeometry::getElement(size_type index) const
{
	return elements[index];
}
