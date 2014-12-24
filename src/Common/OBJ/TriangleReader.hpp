#pragma once
#ifndef _OBJ_TRIANGLEREADER_HPP_
#define _OBJ_TRIANGLEREADER_HPP_

#include "Parser.hpp"

namespace OBJ {
	
	
	
	struct TriangleVertex {
		float x = 0.f, y = 0.f, z = 0.f; // Geometry
		float u = 0.f, v = 0.f;          // Texture
		float i = 0.f, j = 0.f, k = 0.f; // Normal
		bool hasTexture = false;
		bool hasNormal  = false;
	};
	
	struct Triangle {
		TriangleVertex a, b, c;
	};
	
	
	
	/* Triangle reader for obj files */
	class TriangleReader : public virtual OBJParser {
	protected:
		
		/* Called for each triangle */
		virtual bool parse(Triangle &t);
		
		bool parse(Vertex::Geometry &v) override;
		bool parse(Vertex::Texture &v) override;
		bool parse(Vertex::Normal &v) override;
		bool parse(Element::Face &e) override;
		
	private:
		
		std::vector<Vertex::Geometry> geometry;
		std::vector<Vertex::Texture> texture;
		std::vector<Vertex::Normal> normal;
		
	};
	
	
	
} // namespace OBJ

#endif // _OBJ_TRIANGLEREADER_HPP_
