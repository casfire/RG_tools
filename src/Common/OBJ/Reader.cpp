#include "Reader.hpp"
#include <fstream>
#include <sstream>
#include <cstddef> // std::size_t



/* OBJ::TokenReader */

bool OBJ::TokenReader::read(const std::string& filename)
{
	std::ofstream sink;
	return read(filename, sink);
}

bool OBJ::TokenReader::read(const std::string& filename, std::ostream& log)
{
	bool status = false;
	std::ifstream file;
	log << "[" << prefix() << "] Opening " << filename << " for reading.\n";
	file.open(filename, std::ios::in);
	if (file.is_open()) {
		status = read(file, log);
		if (file.bad()) {
			log << "[" << prefix() << "] Error occured while reading " << filename << ".\n";
		}
		log << "[" << prefix() << "] Closing " << filename << ".\n";
		file.close();
	} else {
		log << "[" << prefix() << "] Failed to open " << filename << " for reading.\n";
	}
	log << std::flush;
	return status;
}

bool OBJ::TokenReader::read(std::istream& source)
{
	std::ofstream sink;
	return read(source, sink);
}

bool OBJ::TokenReader::read(std::istream& source, std::ostream& log)
{
	bool status = true;
	std::size_t count = 0;
	for (std::string line; std::getline(source, line); ) {
		count++;
		if (!this->line(line)) {
			log << "[" << prefix() << "] Invalid line " << count << ": " << line << "\n";
			status = false;
		}
	}
	done();
	return !source.bad() && status;
}

void OBJ::TokenReader::done()
{}

bool OBJ::TokenReader::line(const std::string& line)
{
	std::size_t length = static_cast<std::size_t>(line.size());
	
	/* Copy line */
	std::vector<char> copy(line.size() + 1);
	for (std::size_t i = 0; i < length; i++) copy[i] = line[i];
	copy[length] = '\0';
	char* s = static_cast<char*>(&copy[0]);
	
	/* Remove comment */
	for (std::size_t i = 0; i < length; i++) {
		if (s[i] == '#') {
			length = i;
			s[i] = '\0';
		}
	}
	
	std::size_t pos = 0;
	
	/* Skip spaces and non-printable characters */
	while (pos < length && !std::isgraph(s[pos])) pos++;
	
	/* Retreive first token */
	std::size_t start = pos;
	while (pos < length && std::isgraph(s[pos])) pos++;
	if (pos <= start) return true;
	std::string t(static_cast<const char*>(s + start), pos - start);
	
	/* Skip spaces and non-printable characters */
	while (pos < length && !std::isgraph(s[pos])) pos++;
	
	/* Create istream */
	std::istringstream in(std::string(static_cast<const char*>(s + pos), length - pos));
	
	/* Call virtual token method */
	return token(t, in);
}



/* OBJ::ObjectReader */

const char* OBJ::ObjectReader::prefix()
{
	return "OBJ";
}

template<typename T>
bool OBJ::ObjectReader::parser(std::istream& in)
{
	T obj;
	in >> obj;
	if (in.fail()) return false;
	return parse(obj);
}

bool OBJ::ObjectReader::token(const std::string& t, std::istream& in)
{
	/* Vertex data */
	if (t.compare("v")  == 0) return parser<OBJ::Vertex::Geometry>(in);
	if (t.compare("vn") == 0) return parser<OBJ::Vertex::Normal>(in);
	if (t.compare("vt") == 0) return parser<OBJ::Vertex::Texture>(in);
	
	/* Elements */
	if (t.compare("p")  == 0) return parser<OBJ::Element::Point>(in);
	if (t.compare("l")  == 0) return parser<OBJ::Element::Line>(in);
	if (t.compare("f")  == 0) return parser<OBJ::Element::Face>(in);
	
	/* Grouping */
	if (t.compare("g")  == 0) return parser<OBJ::Grouping::Groups>(in);
	if (t.compare("s")  == 0) return parser<OBJ::Grouping::Smoothing>(in);
	if (t.compare("mg") == 0) return parser<OBJ::Grouping::Merge>(in);
	if (t.compare("o")  == 0) return parser<OBJ::Grouping::Object>(in);
	
	/* Display/render attributes */
	if (t.compare("bevel")      == 0) return parser<OBJ::Render::Bevel>(in);
	if (t.compare("c_interp")   == 0) return parser<OBJ::Render::ColorInterpolation>(in);
	if (t.compare("d_interp")   == 0) return parser<OBJ::Render::DissolveInterpolation>(in);
	if (t.compare("lod")        == 0) return parser<OBJ::Render::LevelOfDetail>(in);
	if (t.compare("usemtl")     == 0) return parser<OBJ::Render::UseMaterial>(in);
	if (t.compare("mtllib")     == 0) return parser<OBJ::Render::MaterialLib>(in);
	if (t.compare("shadow_obj") == 0) return parser<OBJ::Render::ShadowObject>(in);
	if (t.compare("trace_obj")  == 0) return parser<OBJ::Render::TraceObject>(in);
	
	return false;
}

bool OBJ::ObjectReader::parse(OBJ::Vertex::Geometry&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Vertex::Texture&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Vertex::Normal&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Element::Point&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Element::Line&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Element::Face&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Grouping::Groups&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Grouping::Smoothing&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Grouping::Merge&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Grouping::Object&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Render::Bevel&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Render::ColorInterpolation&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Render::DissolveInterpolation&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Render::LevelOfDetail&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Render::UseMaterial&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Render::MaterialLib&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Render::ShadowObject&) { return false; }
bool OBJ::ObjectReader::parse(OBJ::Render::TraceObject&) { return false;}



/* OBJ::MaterialReader */

const char* OBJ::MaterialReader::prefix()
{
	return "MTL";
}

template<typename T>
bool OBJ::MaterialReader::parser(std::istream& in)
{
	T obj;
	in >> obj;
	if (in.fail()) return false;
	return parse(obj);
}

bool OBJ::MaterialReader::token(const std::string& t, std::istream& in)
{
	if (t.compare("newmtl")   == 0) return parser<OBJ::Material::NewMaterial>(in);
	if (t.compare("Ka")       == 0) return parser<OBJ::Material::AmbientColor>(in);
	if (t.compare("Kd")       == 0) return parser<OBJ::Material::DiffuseColor>(in);
	if (t.compare("Ks")       == 0) return parser<OBJ::Material::SpecularColor>(in);
	if (t.compare("d")        == 0) return parser<OBJ::Material::Dissolve>(in);
	if (t.compare("illum")    == 0) return parser<OBJ::Material::IlluminationModel>(in);
	if (t.compare("map_Ka")   == 0) return parser<OBJ::Material::AmbientMap>(in);
	if (t.compare("map_Kd")   == 0) return parser<OBJ::Material::DiffuseMap>(in);
	if (t.compare("map_Ks")   == 0) return parser<OBJ::Material::SpecularColorMap>(in);
	if (t.compare("map_Ns")   == 0) return parser<OBJ::Material::SpeculaHighlightMap>(in);
	if (t.compare("map_d")    == 0) return parser<OBJ::Material::AlphaMap>(in);
	if (t.compare("map_bump") == 0) return parser<OBJ::Material::BumpMap>(in);
	return false;
}

bool OBJ::MaterialReader::parse(Material::NewMaterial&) { return false; }
bool OBJ::MaterialReader::parse(Material::AmbientColor&) { return false; }
bool OBJ::MaterialReader::parse(Material::DiffuseColor&) { return false; }
bool OBJ::MaterialReader::parse(Material::SpecularColor&) { return false; }
bool OBJ::MaterialReader::parse(Material::Dissolve&) { return false; }
bool OBJ::MaterialReader::parse(Material::IlluminationModel&) { return false; }
bool OBJ::MaterialReader::parse(Material::AmbientMap&) { return false; }
bool OBJ::MaterialReader::parse(Material::DiffuseMap&) { return false; }
bool OBJ::MaterialReader::parse(Material::SpecularColorMap&) { return false; }
bool OBJ::MaterialReader::parse(Material::SpeculaHighlightMap&) { return false; }
bool OBJ::MaterialReader::parse(Material::AlphaMap&) { return false; }
bool OBJ::MaterialReader::parse(Material::BumpMap&) { return false; }
