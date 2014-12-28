#pragma once
#ifndef _CFR_BASEGEOMETRY_HPP_
#define _CFR_BASEGEOMETRY_HPP_

#include "Common.hpp"
#include <vector>
#include <unordered_map>

namespace CFR {
	
	
	
	class BaseGeometry {
	public:
		
		/* Create */
		BaseGeometry();
		BaseGeometry(const BaseGeometry &copy);
		
		/* Add element - throws CFR::Exception */
		void addElement(Uint32 element);
		
		/* Add vertex and return its element */
		virtual Uint32 pushVertex(const Vertex &v);
		
		/* Either add or find a similar vertex and return its element */
		virtual Uint32 addVertex(const Vertex &v);
		
		/* Reserve space */
		void reserveVertices(size_type count);
		void reserveElements(size_type count);
		
		/* Delete everything */
		void clear();
		
		/* Remove duplicate vertices */
		void recalculate();
		
		/* Getters */
		bool empty() const;
		size_type getVertexCount()  const;
		size_type getElementCount() const;
		Uint32    getElementMax()   const;
		const Vertex& getVertex (size_type index) const;
		const Uint32& getElement(size_type index) const;
		
	private:
		
		std::vector<Vertex> vertices;
		std::unordered_map<Vertex, Uint32> vertexElements; 
		std::vector<Uint32> elements;
		Uint32 elementMax;
		
	};
	
	
	
} // namespace CFR

#endif // _CFR_BASEGEOMETRY_HPP_
