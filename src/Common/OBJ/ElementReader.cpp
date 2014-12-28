#include "ElementReader.hpp"
#include <cstddef> // std::size_t

bool OBJ::ElementReader::parse(OBJ::Vertex::Geometry &v)
{
	geometry.push_back(v);
	return true;
}

bool OBJ::ElementReader::parse(OBJ::Vertex::Texture &v)
{
	texture.push_back(v);
	return true;
}

bool OBJ::ElementReader::parse(OBJ::Vertex::Normal &v)
{
	normal.push_back(v);
	return true;
}

bool OBJ::ElementReader::parse(OBJ::Triangle&)
{
	return false;
}

bool OBJ::ElementReader::parse(Line&)
{
	return false;
}

bool OBJ::ElementReader::parse(Point&)
{
	return false;
}

bool OBJ::ElementReader::convert(OBJ::Element::FaceVertex &f, OBJ::TriangleVertex &e)
{
	if (f.v < 0) f.v = 1 + geometry.size() + f.v;
	if (static_cast<std::size_t>(f.v) > geometry.size() || f.v == 0) {
		return false;
	}
	
	e.hasTexture = f.hasTexture;
	e.hasNormal  = f.hasNormal;
	e.position.x = geometry[f.v - 1].x;
	e.position.y = geometry[f.v - 1].y;
	e.position.z = geometry[f.v - 1].z;
	e.position.w = geometry[f.v - 1].w;
	
	if (f.hasTexture) {
		if (f.vt < 0) f.vt = 1 + texture.size() + f.vt;
		if (static_cast<std::size_t>(f.vt) > texture.size() || f.vt == 0) {
			return false;
		}
		e.texture.x = texture[f.vt - 1].u;
		e.texture.y = texture[f.vt - 1].v;
		e.texture.z = texture[f.vt - 1].w;
	}
	
	if (f.hasNormal) {
		if (f.vn < 0) f.vn = 1 + normal.size() + f.vn;
		if (static_cast<std::size_t>(f.vn) > normal.size() || f.vn == 0) {
			return false;
		}
		e.normal.x = normal[f.vn - 1].i;
		e.normal.y = normal[f.vn - 1].j;
		e.normal.z = normal[f.vn - 1].k;
	}
	
	return true;
}

bool OBJ::ElementReader::convert(Element::LineVertex &f, LineVertex &e)
{
	if (f.v < 0) f.v = 1 + geometry.size() + f.v;
	if (static_cast<std::size_t>(f.v) > geometry.size() || f.v == 0) {
		return false;
	}
	e.hasTexture = f.hasTexture;
	e.position.x = geometry[f.v - 1].x;
	e.position.y = geometry[f.v - 1].y;
	e.position.z = geometry[f.v - 1].z;
	e.position.w = geometry[f.v - 1].w;
	
	if (f.hasTexture) {
		if (f.vt < 0) f.vt = 1 + texture.size() + f.vt;
		if (static_cast<std::size_t>(f.vt) > texture.size() || f.vt == 0) {
			return false;
		}
		e.texture.x = texture[f.vt - 1].u;
		e.texture.y = texture[f.vt - 1].v;
		e.texture.z = texture[f.vt - 1].w;
	}
	
	return true;
}

bool OBJ::ElementReader::convert(Element::PointVertex &f, Point &e)
{
	if (f.v < 0) f.v = 1 + geometry.size() + f.v;
	if (static_cast<std::size_t>(f.v) > geometry.size() || f.v == 0) {
		return false;
	}
	e.position.x = geometry[f.v - 1].x;
	e.position.y = geometry[f.v - 1].y;
	e.position.z = geometry[f.v - 1].z;
	e.position.w = geometry[f.v - 1].w;
	return true;
}

bool OBJ::ElementReader::parse(Element::Face &e)
{
	bool status = true;
	for (std::size_t i = 2; i < e.size(); i++) {
		OBJ::Element::FaceVertex &a = e[0];
		OBJ::Element::FaceVertex &b = e[i];
		OBJ::Element::FaceVertex &c = e[i - 1];
		OBJ::Triangle t;
		if (convert(a, t.a) && convert(b, t.b) && convert(c, t.c)) {
			if (!parse(t)) status = false;
		} else {
			status = false;
		}
	}
	return status;
}

bool OBJ::ElementReader::parse(Element::Line &e)
{
	bool status = true;
	for (std::size_t i = 1; i < e.size(); i++) {
		OBJ::Element::LineVertex &a = e[i - 1];
		OBJ::Element::LineVertex &b = e[i];
		OBJ::Line l;
		if (convert(a, l.a) && convert(b, l.b)) {
			if (!parse(l)) status = false;
		} else {
			status = false;
		}
	}
	return status;
}

bool OBJ::ElementReader::parse(Element::Point &e)
{
	bool status = true;
	for (std::size_t i = 0; i < e.size(); i++) {
		OBJ::Point p;
		if (convert(e[i], p)) {
			if (!parse(p)) status = false;
		} else {
			status = false;
		}
	}
	return status;
}
