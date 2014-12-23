#include "MaterialReader.hpp"



/* OBJ::MaterialReader */

void OBJ::MaterialReader::parse(Material&)
{}

bool OBJ::MaterialReader::parse(MTL::NewMaterial& m)
{
	done();
	current = OBJ::Material();
	current.name = m.name;
	return true;
}

bool OBJ::MaterialReader::parse(MTL::AmbientColor& m)
{
	current.ambient = m;
	return current.hasAmbient = true;
}

bool OBJ::MaterialReader::parse(MTL::DiffuseColor& m)
{
	current.diffuse = m;
	return current.hasDiffuse = true;
}

bool OBJ::MaterialReader::parse(MTL::SpecularColor& m)
{
	current.specular = m;
	return current.hasSpecular = true;
}

bool OBJ::MaterialReader::parse(MTL::EmissionColor& m)
{
	current.emission = m;
	return current.hasEmission = true;
}

bool OBJ::MaterialReader::parse(MTL::Dissolve& m)
{
	current.dissolve = m;
	return current.hasDissolve = true;
}

bool OBJ::MaterialReader::parse(MTL::IlluminationModel& m)
{
	current.illum = m;
	return current.hasIllum = true;
}

bool OBJ::MaterialReader::parse(MTL::SpecularExponent& m)
{
	current.specularExponent = m;
	return current.hasSpecularExponent = true;
}

bool OBJ::MaterialReader::parse(MTL::RefractionIndex& m)
{
	current.refractionIndex = m;
	return current.hasRefractionIndex = true;
}

bool OBJ::MaterialReader::parse(MTL::TransmittionFilter& m)
{
	current.transmittionFilter = m;
	return current.hasTransmittionFilter = true;
}

bool OBJ::MaterialReader::parse(MTL::AmbientMap& m)
{
	current.mapAmbient = m;
	return current.hasMapAmbient = true;
}

bool OBJ::MaterialReader::parse(MTL::DiffuseMap& m)
{
	current.mapDiffuse = m;
	return current.hasMapDiffuse = true;
}

bool OBJ::MaterialReader::parse(MTL::SpecularColorMap& m)
{
	current.mapSpecular = m;
	return current.hasMapSpecular = true;
}

bool OBJ::MaterialReader::parse(MTL::SpeculaHighlightMap& m)
{
	current.mapSpecularHighlight = m;
	return current.hasMapSpecularHighlight = true;
}

bool OBJ::MaterialReader::parse(MTL::AlphaMap& m)
{
	current.mapAlpha = m;
	return current.hasMapAlpha = true;
}

bool OBJ::MaterialReader::parse(MTL::BumpMap& m)
{
	current.mapBump = m;
	return current.hasMapBump = true;
}

void OBJ::MaterialReader::done()
{
	if (!current.name.empty()) parse(current);
}



/* OBJ::MaterialSaver */

typedef std::map<std::string, OBJ::Material>::const_iterator MaterialConstIterator;

void OBJ::MaterialSaver::parse(OBJ::Material &m)
{
	materials[m.name] = m;
}

const OBJ::Material& OBJ::MaterialSaver::find(const std::string &name) const
{
	MaterialConstIterator i = materials.find(name);
	if (i != materials.end()) return i->second;
	throw OBJ::MaterialNotFoundException(name);
}

void OBJ::MaterialSaver::clear()
{
	materials.clear();
}



/* OBJ::MaterialNotFoundException */

OBJ::MaterialNotFoundException::MaterialNotFoundException(const std::string &name)
: name(name)
{}

const char* OBJ::MaterialNotFoundException::what() const throw()
{
	return name.c_str();
}
