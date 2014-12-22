#pragma once
#ifndef _OBJ_TRIANGLEREADER_HPP_
#define _OBJ_TRIANGLEREADER_HPP_

#include "Reader.hpp"

namespace OBJ {
	
	
	
	struct TriangleVertex {
		float x, y, z; // Geometry
		float u, v;    // Texture
		float i, j, k; // Normal
		bool hasTexture, hasNormal;
	};
	
	struct Triangle {
		TriangleVertex a, b, c;
	};
	
	
	
	/* Triangle reader for obj files */
	class TriangleReader : public virtual ObjectReader {
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
