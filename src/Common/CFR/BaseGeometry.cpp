#include "BaseGeometry.hpp"
#include <cmath>

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
	if (element >= vertices.size()) {
		throw CFR::Exception("Element out of range.");
	}
	elements.push_back(element);
	if (elementMax < element) elementMax = element;
}

Uint32 CFR::BaseGeometry::addVertex(const CFR::Vertex &vertex)
{
	vertices.push_back(vertex);
	return static_cast<Uint32>(vertices.size() - 1);
}

Uint32 CFR::BaseGeometry::addVertex(
	float px, float py, float pz,
	float nx, float ny, float nz,
	float tx, float ty, float tz)
{
	return addVertex(CFR::Vertex(px, py, pz, nx, ny, nz, tx, ty, tz));
}

Uint32 CFR::BaseGeometry::addSimilarVertex(
	const Vertex &vertex,
	float error, size_type start, size_type end)
{
	if (end <= start) end = vertices.size();
	const Vertex& a = vertex;
	for (size_type v = start; v < end; v++) {
		const Vertex& b = vertices[v];
		if (std::abs(a.position[0] - b.position[0]) > error) continue;
		if (std::abs(a.position[1] - b.position[1]) > error) continue;
		if (std::abs(a.position[2] - b.position[2]) > error) continue;
		if (std::abs(a.normal[0]   - b.normal[0]  ) > error) continue;
		if (std::abs(a.normal[1]   - b.normal[1]  ) > error) continue;
		if (std::abs(a.normal[2]   - b.normal[2]  ) > error) continue;
		if (std::abs(a.texture[0]  - b.texture[0] ) > error) continue;
		if (std::abs(a.texture[1]  - b.texture[1] ) > error) continue;
		if (std::abs(a.texture[2]  - b.texture[2] ) > error) continue;
		return static_cast<Uint32>(v);
	}
	return addVertex(vertex);
}

Uint32 CFR::BaseGeometry::addSimilarVertex(
	float px, float py, float pz,
	float nx, float ny, float nz,
	float tx, float ty, float tz,
	float error, size_type start, size_type end)
{
	return addSimilarVertex(CFR::Vertex(px, py, pz, nx, ny, nz, tx, ty, tz), error, start, end);
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

bool CFR::BaseGeometry::empty() const
{
	return vertices.empty() && elements.empty();
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
