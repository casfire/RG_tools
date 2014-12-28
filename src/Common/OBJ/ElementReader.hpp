#pragma once
#ifndef _OBJ_ELEMENTREADER_HPP_
#define _OBJ_ELEMENTREADER_HPP_

#include "Parser.hpp"

namespace OBJ {
	
	
	
	struct Vec1               { float x = 0.f; };
	struct Vec2 : public Vec1 { float y = 0.f; };
	struct Vec3 : public Vec2 { float z = 0.f; };
	struct Vec4 : public Vec3 { float w = 1.f; };
	
	struct Point {
		Vec4 position;
	};
	
	struct LineVertex {
		Vec4 position;
		Vec3 texture;
		bool hasTexture = false;
	};
	
	struct TriangleVertex {
		Vec4 position;
		Vec3 texture;
		Vec3 normal;
		bool hasTexture = false;
		bool hasNormal  = false;
	};
	
	struct Triangle {
		TriangleVertex a, b, c;
	};
	
	struct Line {
		LineVertex a, b;
	};
	
	
	
	/* Element reader for obj files */
	class ElementReader : public OBJParser {
	protected:
		
		/* Called for each element */
		virtual bool parse(Triangle &e);
		virtual bool parse(Line &e);
		virtual bool parse(Point &e);
		
		bool parse(Vertex::Geometry &v) override;
		bool parse(Vertex::Texture &v) override;
		bool parse(Vertex::Normal &v) override;
		bool parse(Element::Face &e) override;
		bool parse(Element::Line &e) override;
		bool parse(Element::Point &e) override;
		
	private:
		
		std::vector<Vertex::Geometry> geometry;
		std::vector<Vertex::Texture> texture;
		std::vector<Vertex::Normal> normal;
		
		bool convert(Element::FaceVertex &f, TriangleVertex &e);
		bool convert(Element::LineVertex &f, LineVertex &e);
		bool convert(Element::PointVertex &f, Point &e);
		
	};
	
	
	
} // namespace OBJ

#endif // _OBJ_ELEMENTREADER_HPP_
