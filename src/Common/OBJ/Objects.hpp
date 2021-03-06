#pragma once
#ifndef _OBJ_OBJECTS_HPP_
#define _OBJ_OBJECTS_HPP_

#include <vector>
#include <string>
#include <istream>



/* Vertex data */

namespace OBJ { namespace Vertex {
	
	struct Geometry {
		float x, y, z, w = 1.f;
	};
	
	struct Texture {
		float u, v = 0.f, w = 0.f;
	};
	
	struct Normal {
		float i, j, k;
	};
	
}} // namespace OBJ::Vertex



/* Elements */

namespace OBJ { namespace Element {
	
	struct PointVertex {
		int v;
	};
	
	typedef std::vector<PointVertex> Point;
	
	struct LineVertex {
		int v, vt = 0;
		bool hasTexture;
	};
	
	typedef std::vector<LineVertex> Line;
	
	struct FaceVertex {
		int v, vt = 0, vn = 0;
		bool hasTexture, hasNormal;
	};
	
	typedef std::vector<FaceVertex> Face;
	
}} // namespace OBJ::Element



/* Grouping */

namespace OBJ { namespace Grouping {
	
	struct Groups {
		std::vector<std::string> names;
	};
	
	struct Smoothing {
		int group_number;
	};
	
	struct Merge {
		int group_number, res;
	};
	
	struct Object {
		std::string name;
	};
	
}} // namespace OBJ::Grouping



/* Display/render attributes */

namespace OBJ { namespace Render {
	
	struct Bevel {
		bool enabled;
	};
	
	struct ColorInterpolation {
		bool enabled;
	};
	
	struct DissolveInterpolation {
		bool enabled;
	};
	
	struct LevelOfDetail {
		int level;
	};
	
	struct UseMaterial {
		std::string name;
	};
	
	struct MaterialLib {
		std::vector<std::string> files;
	};
	
	struct ShadowObject {
		std::string file;
	};
	
	struct TraceObject {
		std::string file;
	};
	
}} // namespace OBJ::Render



/* Material objects */

namespace OBJ { namespace MTL {
	
	struct NewMaterial {
		std::string name;
	};
	
	struct AmbientColor {
		float r, g, b;
	};
	
	struct DiffuseColor {
		float r, g, b;
	};
	
	struct SpecularColor {
		float r, g, b;
	};
	
	struct EmissionColor {
		float r, g, b;
	};
	
	struct Dissolve {
		float value;
	};
	
	struct IlluminationModel {
		int model;
	};
	
	struct SpecularExponent {
		float value;
	};
	
	struct RefractionIndex {
		float value;
	};
	
	struct TransmittionFilter {
		float r, g, b;
	};
	
	struct AmbientMap {
		std::string file;
	};
	
	struct DiffuseMap {
		std::string file;
	};
	
	struct SpecularColorMap {
		std::string file;
	};
	
	struct SpeculaHighlightMap {
		std::string file;
	};
	
	struct AlphaMap {
		std::string file;
	};
	
	struct BumpMap {
		std::string file;
	};
	
}} // namespace OBJ::MTL



std::istream& operator>>(std::istream& in, OBJ::Vertex::Geometry& obj);
std::istream& operator>>(std::istream& in, OBJ::Vertex::Texture& obj);
std::istream& operator>>(std::istream& in, OBJ::Vertex::Normal& obj);

std::istream& operator>>(std::istream& in, OBJ::Element::PointVertex& obj);
std::istream& operator>>(std::istream& in, OBJ::Element::Point& obj);
std::istream& operator>>(std::istream& in, OBJ::Element::LineVertex& obj);
std::istream& operator>>(std::istream& in, OBJ::Element::Line& obj);
std::istream& operator>>(std::istream& in, OBJ::Element::FaceVertex& obj);
std::istream& operator>>(std::istream& in, OBJ::Element::Face& obj);

std::istream& operator>>(std::istream& in, OBJ::Grouping::Groups& obj);
std::istream& operator>>(std::istream& in, OBJ::Grouping::Smoothing& obj);
std::istream& operator>>(std::istream& in, OBJ::Grouping::Merge& obj);
std::istream& operator>>(std::istream& in, OBJ::Grouping::Object& obj);

std::istream& operator>>(std::istream& in, OBJ::Render::Bevel& obj);
std::istream& operator>>(std::istream& in, OBJ::Render::ColorInterpolation& obj);
std::istream& operator>>(std::istream& in, OBJ::Render::DissolveInterpolation& obj);
std::istream& operator>>(std::istream& in, OBJ::Render::LevelOfDetail& obj);
std::istream& operator>>(std::istream& in, OBJ::Render::UseMaterial& obj);
std::istream& operator>>(std::istream& in, OBJ::Render::MaterialLib& obj);
std::istream& operator>>(std::istream& in, OBJ::Render::ShadowObject& obj);
std::istream& operator>>(std::istream& in, OBJ::Render::TraceObject& obj);

std::istream& operator>>(std::istream& in, OBJ::MTL::NewMaterial& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::AmbientColor& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::DiffuseColor& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::SpecularColor& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::EmissionColor& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::Dissolve& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::IlluminationModel& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::SpecularExponent& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::RefractionIndex& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::TransmittionFilter& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::AmbientMap& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::DiffuseMap& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::SpecularColorMap& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::SpeculaHighlightMap& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::AlphaMap& obj);
std::istream& operator>>(std::istream& in, OBJ::MTL::BumpMap& obj);



#endif // _OBJ_OBJECTS_HPP_
