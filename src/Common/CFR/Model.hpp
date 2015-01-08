#pragma once
#ifndef _CFR_MODEL_HPP_
#define _CFR_MODEL_HPP_

#include "Common.hpp"
#include <ostream>
#include <string>
#include <vector>

std::ostream& operator<<(std::ostream& out, const CFR::Model& obj);

namespace CFR {
	
	
	
	/* Model object */
	struct ModelObject {
		size_type   start, end;
		Vec3        diffuse;
		std::string diffuse_map;
		float       emit = 0.f;
		float       specular_exp = 0.f;
		float       specular = 0.5f;
		std::string specular_map;
		std::string mask_map;
	};
	
	
	
	/* CFR Model */
	class Model {
	public:
		
		Model(const std::string &geometry);
		void setHeader (const std::string &header);
		void addObject (const ModelObject &obj);
		void saveToFile(const std::string &file) const;
		
	private:
		
		const std::string geometry;
		std::string header;
		std::vector<ModelObject> objects;
		friend std::ostream& ::operator<<(std::ostream&, const Model&);
	};
	
	
	
} // namespace CFR

#endif // _CFR_MODEL_HPP_
