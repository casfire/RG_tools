#pragma once
#ifndef _CFR_BASEGEOMETRY_HPP_
#define _CFR_BASEGEOMETRY_HPP_

#include "Common.hpp"
#include <vector>

namespace CFR {
	
	
	
	class BaseGeometry {
	public:
		
		/* Create */
		BaseGeometry();
		BaseGeometry(const BaseGeometry &copy);
		
		/* Add element - throws CFR::Exception */
		void addElement(Uint32 element);
		
		/* Add vertex and return its element */
		Uint32 addVertex(const Vertex &vertex);
		Uint32 addVertex(
			float px = 0.f, float py = 0.f, float pz = 0.f,
			float nx = 0.f, float ny = 0.f, float nz = 0.f,
			float tx = 0.f, float ty = 0.f, float tz = 0.f
		);
		
		/* Either add or find a similar vertex and return its element */
		Uint32 addSimilarVertex(
			const Vertex &vertex,
			float error = 0.001, size_type start = 0, size_type end = 0
		);
		Uint32 addSimilarVertex(
			float px = 0.f, float py = 0.f, float pz = 0.f,
			float nx = 0.f, float ny = 0.f, float nz = 0.f,
			float tx = 0.f, float ty = 0.f, float tz = 0.f,
			float error = 0.001, size_type start = 0, size_type end = 0
		);
		
		/* Reserve space */
		void reserveVertices(size_type count);
		void reserveElements(size_type count);
		
		/* Delete everything */
		void clear();
		
		/* Getters */
		bool empty() const;
		size_type getVertexCount()  const;
		size_type getElementCount() const;
		Uint32    getElementMax()   const;
		const Vertex& getVertex (size_type index) const;
		const Uint32& getElement(size_type index) const;
		
	private:
		
		std::vector<Vertex> vertices;
		std::vector<Uint32> elements;
		Uint32 elementMax;
		
	};
	
	
	
} // namespace CFR

#endif // _CFR_BASEGEOMETRY_HPP_
