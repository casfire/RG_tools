#include "TriangleReader.hpp"
#include <cstddef> // std::size_t

bool OBJ::TriangleReader::parse(OBJ::Vertex::Geometry &v)
{
	geometry.push_back(v);
	return true;
}

bool OBJ::TriangleReader::parse(OBJ::Vertex::Texture &v)
{
	texture.push_back(v);
	return true;
}

bool OBJ::TriangleReader::parse(OBJ::Vertex::Normal &v)
{
	normal.push_back(v);
	return true;
}

bool OBJ::TriangleReader::parse(OBJ::Triangle&)
{
	return false;
}
bool OBJ::TriangleReader::parse(OBJ::Element::Face &e)
{
	bool status = true;
	for (std::size_t i = 2; i < e.size(); i++) {
		
		OBJ::Element::FaceVertex &a = e[0];
		OBJ::Element::FaceVertex &b = e[i - 1];
		OBJ::Element::FaceVertex &c = e[i];
		
		OBJ::Triangle t;
		
		if (a.v < 0) a.v = 1 + geometry.size() + a.v;
		if (static_cast<std::size_t>(a.v) > geometry.size() || a.v == 0) {
			status = false;
			continue;
		}
		t.a.x = geometry[a.v - 1].x;
		t.a.y = geometry[a.v - 1].y;
		t.a.z = geometry[a.v - 1].z;
		t.a.hasTexture = a.hasTexture;
		t.a.hasNormal = a.hasNormal;
		if (a.hasTexture) {
			if (a.vt < 0) a.vt = 1 + texture.size() + a.vt;
			if (static_cast<std::size_t>(a.vt) > texture.size() || a.vt == 0) {
				status = false;
				continue;
			}
			t.a.u = texture[a.vt - 1].u;
			t.a.v = texture[a.vt - 1].v;
		}
		if (a.hasNormal) {
			if (a.vn < 0) a.vn = 1 + normal.size() + a.vn;
			if (static_cast<std::size_t>(a.vn) > normal.size() || a.vn == 0) {
				status = false;
				continue;
			}
			t.a.i = normal[a.vn - 1].i;
			t.a.j = normal[a.vn - 1].j;
			t.a.k = normal[a.vn - 1].k;
		}
		
		if (b.v < 0) b.v = 1 + geometry.size() + b.v;
		if (static_cast<std::size_t>(b.v) > geometry.size() || b.v == 0) {
			status = false;
			continue;
		}
		t.b.x = geometry[b.v - 1].x;
		t.b.y = geometry[b.v - 1].y;
		t.b.z = geometry[b.v - 1].z;
		t.b.hasTexture = b.hasTexture;
		t.b.hasNormal = b.hasNormal;
		if (b.hasTexture) {
			if (b.vt < 0) b.vt = 1 + texture.size() + b.vt;
			if (static_cast<std::size_t>(b.vt) > texture.size() || b.vt == 0) {
				status = false;
				continue;
			}
			t.b.u = texture[b.vt - 1].u;
			t.b.v = texture[b.vt - 1].v;
		}
		if (b.hasNormal) {
			if (b.vn < 0) b.vn = 1 + normal.size() + b.vn;
			if (static_cast<std::size_t>(b.vn) > normal.size() || b.vn == 0) {
				status = false;
				continue;
			}
			t.b.i = normal[b.vn - 1].i;
			t.b.j = normal[b.vn - 1].j;
			t.b.k = normal[b.vn - 1].k;
		}
		
		if (c.v < 0) c.v = 1 + geometry.size() + c.v;
		if (static_cast<std::size_t>(c.v) > geometry.size() || c.v == 0) {
			status = false;
			continue;
		}
		t.c.x = geometry[c.v - 1].x;
		t.c.y = geometry[c.v - 1].y;
		t.c.z = geometry[c.v - 1].z;
		t.c.hasTexture = c.hasTexture;
		t.c.hasNormal = c.hasNormal;
		if (c.hasTexture) {
			if (c.vt < 0) c.vt = 1 + texture.size() + c.vt;
			if (static_cast<std::size_t>(c.vt) > texture.size() || c.vt == 0) {
				status = false;
				continue;
			}
			t.c.u = texture[c.vt - 1].u;
			t.c.v = texture[c.vt - 1].v;
		}
		if (c.hasNormal) {
			if (c.vn < 0) c.vn = 1 + normal.size() + c.vn;
			if (static_cast<std::size_t>(c.vn) > normal.size() || c.vn == 0) {
				status = false;
				continue;
			}
			t.c.i = normal[c.vn - 1].i;
			t.c.j = normal[c.vn - 1].j;
			t.c.k = normal[c.vn - 1].k;
		}
		
		if (!parse(t)) status = false;
	}
	
	return status;
}
