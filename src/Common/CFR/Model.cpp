#include "Model.hpp"
#include <fstream>
#include <map>

using CFR::size_type;
using CFR::ModelObject;
using CFR::Model;
using CFR::Exception;

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
		if (!object.diffuse_map.empty()) {
			out << "diffuse_map " << object.diffuse_map << "\n";
		} else {
			out << "diffuse " << object.diffuse.x << " " << object.diffuse.y << " " << object.diffuse.z << "\n";
		}
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
