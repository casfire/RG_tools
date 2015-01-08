#include "Model.hpp"
#include <fstream>
#include <map>

using CFR::size_type;
using CFR::ModelObject;
using CFR::Model;
using CFR::Exception;

inline CFR::Vec3 createVec(float x, float y, float z) {
	CFR::Vec3 vec;
	vec.x = x;
	vec.y = y;
	vec.z = z;
	return vec;
}

ModelObject::ModelObject()
{
	start = 0;
	end   = 0;
	diffuse      = createVec(1, 1, 1);
	diffuse_map  = "";
	normal_map   = "";
	specular     = createVec(0.5, 0.5, 0.5);
	specular_map = "";
	specular_exp = 0.f;
	mask_map     = "";
	emit         = createVec(0, 0, 0);
	emit_map     = "";
}

Model::Model(const std::string &geometry)
: geometry(geometry)
{}

void Model::setHeader(const std::string &header)
{
	this->header = header;
}

void Model::addObject(const ModelObject &obj)
{
	objects.push_back(obj);
}

void Model::saveToFile(const std::string &file) const
{
	try {
		std::ofstream stream;
		stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		stream.open(file, std::ios::binary);
		stream << *this;
		stream.close();
	} catch (std::ios::failure &fail) {
		throw Exception("IO error: " + std::string(fail.what()));
	}
}

bool hasSameMaterial(const ModelObject &a, const ModelObject &b) {
	if (a.diffuse_map.empty()) return false;
	if (b.diffuse_map.empty()) return false;
	return a.diffuse_map.compare(b.diffuse_map) == 0;
}

std::ostream& operator<<(std::ostream& out, const Model& obj)
{
	if (!obj.header.empty()) out << "#" << obj.header << "\n";
	out << "version 1\n";
	out << "geometry " << obj.geometry << "\n";
	out << "\n";
	
	std::vector<ModelObject> copy(obj.objects);
	for (std::size_t i = 0; i < copy.size(); i++) {
		const ModelObject& object = copy[i];
		if (object.end <= object.start) continue;
		if (!object.diffuse_map.empty())  out << "diffuse_map  " << object.diffuse_map << "\n";
		if (!object.normal_map.empty())   out << "normal_map  "  << object.normal_map << "\n";
		if (!object.specular_map.empty()) out << "specular_map " << object.specular_map << "\n";
		if (!object.mask_map.empty())     out << "mask_map     " << object.mask_map << "\n";
		if (!object.emit_map.empty())     out << "emit_map     " << object.emit_map << "\n";
		if (object.diffuse_map.empty())   out << "diffuse      " << object.diffuse.x  << " " << object.diffuse.y  << " " << object.diffuse.z  << "\n";
		if (object.specular_map.empty())  out << "specular     " << object.specular.x << " " << object.specular.y << " " << object.specular.z << "\n";
		if (object.emit_map.empty())      out << "emit         " << object.emit.x     << " " << object.emit.y     << " " << object.emit.z     << "\n";
		if (object.specular_exp > 0.01f)  out << "specular_exp " << object.specular_exp << "\n";
		out << "range " << object.start << " " << object.end << "\n";
		for (std::size_t j = i + 1; j < copy.size(); j++) {
			ModelObject& other = copy[j];
			if (other.end <= other.start) continue;
			if (hasSameMaterial(object, other)) {
				out << "range " << other.start << " " << other.end << "\n";
				other.start = 0; other.end = 0;
			}
		}
		out << "end\n\n";
	}
	
	return out;
}
