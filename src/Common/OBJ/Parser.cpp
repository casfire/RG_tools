#include "Parser.hpp"
#include <sstream>
#include <fstream>

using OBJ::TokenParser;
using OBJ::OBJParser;
using OBJ::MTLParser;

struct nullbuf : std::streambuf {
	virtual int_type overflow(int_type c) {
		return traits_type::not_eof(c);
	}
};

nullbuf ostream_sink_buf;
std::ostream ostream_sink(&ostream_sink_buf);

inline std::string getTokenPrefix(const std::string &path)
{
	std::string::size_type n = path.rfind('/');
	if (n == std::string::npos) n = path.rfind('\\');
	if (n == path.size() || n == std::string::npos) {
		return path;
	} else {
		return path.substr(n + 1, std::string::npos);
	}
}

inline std::string& inl_ltrim(std::string &s) {
	std::size_t pos = 0;
	while (pos < s.size() && !std::isgraph(s[pos])) pos++;
	return s.erase(0, pos);
}

inline std::string& inl_rtrim(std::string &s) {
	std::size_t pos = s.size();
	while (pos > 0 && !std::isgraph(s[pos - 1])) pos--;
	return s.erase(pos);
}

inline std::string inl_ltoken(const std::string &s) {
	std::size_t pos = 0;
	while (pos < s.size() && std::isgraph(s[pos])) pos++;
	return s.substr(0, pos);
}



/* TokenParser */

TokenParser::TokenParser()
: logger(&ostream_sink), lineNumber(0)
{}

void TokenParser::read(const std::string& filename)
{
	read(filename, ostream_sink);
}

void TokenParser::read(const std::string& filename, std::ostream& log)
{
	prefix = getTokenPrefix(filename);
	std::ifstream file;
	log << "[" << prefix << "] Opening for reading.\n";
	file.open(filename, std::ios::in);
	if (file.is_open()) {
		read(file, log);
		if (file.bad()) log << "[" << prefix << "] IO Error.\n";
		log << "[" << prefix << "] Closing.\n";
		file.close();
	} else {
		log << "[" << prefix << "] Failed to open for reading.\n";
	}
	log << std::flush;
}

void TokenParser::read(std::istream& source)
{
	read(source, ostream_sink);
}

void TokenParser::read(std::istream& source, std::ostream& log)
{
	logger = &log;
	lineNumber = 0;
	for (std::string line; std::getline(source, line); ) {
		lineNumber++;
		if (parse(line)) continue;
		if (!prefix.empty()) log << "[" << prefix << "] ";
		log << "Invalid line " << lineNumber << ": " << line << "\n";
	}
	done();
	logger = &ostream_sink;
}

void TokenParser::done()
{}

std::ostream& TokenParser::getLogger() const
{
	return *logger;
}

std::size_t TokenParser::getLineNumber() const
{
	return lineNumber;
}

bool TokenParser::parse(const std::string& line)
{
	/* Copy line */
	std::string copy = line;
	
	/* Remove comment */
	std::string::size_type n = copy.find('#');
	if (n != std::string::npos) copy.erase(n);
	
	/* Left and right trim */
	copy = inl_ltrim(inl_rtrim(copy));
	
	/* Retreive key */
	std::string key = inl_ltoken(copy);
	copy.erase(0, key.size());
	
	/* Left trim */
	copy = inl_ltrim(copy);
	
	/* Nothing to parse */
	if (key.empty()) return true;
	
	/* Create istream */
	std::istringstream in(copy);
	
	/* Call virtual token method */
	return token(key, in);
}



/* OBJParser */

template<typename T>
bool OBJParser::parser(std::istream& in)
{
	T obj;
	in >> obj;
	if (in.fail()) return false;
	return parse(obj);
}

bool OBJParser::token(const std::string& t, std::istream& in)
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

bool OBJParser::parse(OBJ::Vertex::Geometry&)              { return false; }
bool OBJParser::parse(OBJ::Vertex::Texture&)               { return false; }
bool OBJParser::parse(OBJ::Vertex::Normal&)                { return false; }
bool OBJParser::parse(OBJ::Element::Point&)                { return false; }
bool OBJParser::parse(OBJ::Element::Line&)                 { return false; }
bool OBJParser::parse(OBJ::Element::Face&)                 { return false; }
bool OBJParser::parse(OBJ::Grouping::Groups&)              { return false; }
bool OBJParser::parse(OBJ::Grouping::Smoothing&)           { return false; }
bool OBJParser::parse(OBJ::Grouping::Merge&)               { return false; }
bool OBJParser::parse(OBJ::Grouping::Object&)              { return false; }
bool OBJParser::parse(OBJ::Render::Bevel&)                 { return false; }
bool OBJParser::parse(OBJ::Render::ColorInterpolation&)    { return false; }
bool OBJParser::parse(OBJ::Render::DissolveInterpolation&) { return false; }
bool OBJParser::parse(OBJ::Render::LevelOfDetail&)         { return false; }
bool OBJParser::parse(OBJ::Render::UseMaterial&)           { return false; }
bool OBJParser::parse(OBJ::Render::MaterialLib&)           { return false; }
bool OBJParser::parse(OBJ::Render::ShadowObject&)          { return false; }
bool OBJParser::parse(OBJ::Render::TraceObject&)           { return false;}



/* MTLParser */

template<typename T>
bool MTLParser::parser(std::istream& in)
{
	T obj;
	in >> obj;
	if (in.fail()) return false;
	return parse(obj);
}

bool MTLParser::token(const std::string& t, std::istream& in)
{
	if (t.compare("newmtl")   == 0) return parser<OBJ::MTL::NewMaterial>(in);
	if (t.compare("Ka")       == 0) return parser<OBJ::MTL::AmbientColor>(in);
	if (t.compare("Kd")       == 0) return parser<OBJ::MTL::DiffuseColor>(in);
	if (t.compare("Ks")       == 0) return parser<OBJ::MTL::SpecularColor>(in);
	if (t.compare("Ke")       == 0) return parser<OBJ::MTL::EmissionColor>(in);
	if (t.compare("d")        == 0) return parser<OBJ::MTL::Dissolve>(in);
	if (t.compare("Tr")       == 0) return parser<OBJ::MTL::Dissolve>(in);
	if (t.compare("illum")    == 0) return parser<OBJ::MTL::IlluminationModel>(in);
	if (t.compare("Ns")       == 0) return parser<OBJ::MTL::SpecularExponent>(in);
	if (t.compare("Ni")       == 0) return parser<OBJ::MTL::RefractionIndex>(in);
	if (t.compare("Tf")       == 0) return parser<OBJ::MTL::TransmittionFilter>(in);
	if (t.compare("map_Ka")   == 0) return parser<OBJ::MTL::AmbientMap>(in);
	if (t.compare("map_Kd")   == 0) return parser<OBJ::MTL::DiffuseMap>(in);
	if (t.compare("map_Ks")   == 0) return parser<OBJ::MTL::SpecularColorMap>(in);
	if (t.compare("map_Ns")   == 0) return parser<OBJ::MTL::SpeculaHighlightMap>(in);
	if (t.compare("map_d")    == 0) return parser<OBJ::MTL::AlphaMap>(in);
	if (t.compare("map_bump") == 0) return parser<OBJ::MTL::BumpMap>(in);
	if (t.compare("bump")     == 0) return parser<OBJ::MTL::BumpMap>(in);
	return false;
}

bool MTLParser::parse(MTL::NewMaterial&)         { return false; }
bool MTLParser::parse(MTL::AmbientColor&)        { return false; }
bool MTLParser::parse(MTL::DiffuseColor&)        { return false; }
bool MTLParser::parse(MTL::SpecularColor&)       { return false; }
bool MTLParser::parse(MTL::EmissionColor&)       { return false; }
bool MTLParser::parse(MTL::Dissolve&)            { return false; }
bool MTLParser::parse(MTL::IlluminationModel&)   { return false; }
bool MTLParser::parse(MTL::SpecularExponent&)    { return false; }
bool MTLParser::parse(MTL::RefractionIndex&)     { return false; }
bool MTLParser::parse(MTL::TransmittionFilter&)  { return false; }
bool MTLParser::parse(MTL::AmbientMap&)          { return false; }
bool MTLParser::parse(MTL::DiffuseMap&)          { return false; }
bool MTLParser::parse(MTL::SpecularColorMap&)    { return false; }
bool MTLParser::parse(MTL::SpeculaHighlightMap&) { return false; }
bool MTLParser::parse(MTL::AlphaMap&)            { return false; }
bool MTLParser::parse(MTL::BumpMap&)             { return false; }
