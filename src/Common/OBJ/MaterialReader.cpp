#include "MaterialReader.hpp"

using OBJ::Material;
using OBJ::MaterialReader;
using OBJ::MaterialSaver;
typedef std::map<std::string, Material>::const_iterator MaterialConstIterator;



/* MaterialReader */

void MaterialReader::parse(Material&)
{}

bool MaterialReader::parse(MTL::NewMaterial& m)
{
	done();
	current = Material();
	current.name = m.name;
	return true;
}

bool MaterialReader::parse(MTL::AmbientColor& m)
{
	current.ambient = m;
	return current.hasAmbient = true;
}

bool MaterialReader::parse(MTL::DiffuseColor& m)
{
	current.diffuse = m;
	return current.hasDiffuse = true;
}

bool MaterialReader::parse(MTL::SpecularColor& m)
{
	current.specular = m;
	return current.hasSpecular = true;
}

bool MaterialReader::parse(MTL::EmissionColor& m)
{
	current.emission = m;
	return current.hasEmission = true;
}

bool MaterialReader::parse(MTL::Dissolve& m)
{
	current.dissolve = m;
	return current.hasDissolve = true;
}

bool MaterialReader::parse(MTL::IlluminationModel& m)
{
	current.illum = m;
	return current.hasIllum = true;
}

bool MaterialReader::parse(MTL::SpecularExponent& m)
{
	current.specularExponent = m;
	return current.hasSpecularExponent = true;
}

bool MaterialReader::parse(MTL::RefractionIndex& m)
{
	current.refractionIndex = m;
	return current.hasRefractionIndex = true;
}

bool MaterialReader::parse(MTL::TransmittionFilter& m)
{
	current.transmittionFilter = m;
	return current.hasTransmittionFilter = true;
}

bool MaterialReader::parse(MTL::AmbientMap& m)
{
	current.mapAmbient = m;
	return current.hasMapAmbient = true;
}

bool MaterialReader::parse(MTL::DiffuseMap& m)
{
	current.mapDiffuse = m;
	return current.hasMapDiffuse = true;
}

bool MaterialReader::parse(MTL::SpecularColorMap& m)
{
	current.mapSpecular = m;
	return current.hasMapSpecular = true;
}

bool MaterialReader::parse(MTL::SpeculaHighlightMap& m)
{
	current.mapSpecularHighlight = m;
	return current.hasMapSpecularHighlight = true;
}

bool MaterialReader::parse(MTL::AlphaMap& m)
{
	current.mapAlpha = m;
	return current.hasMapAlpha = true;
}

bool MaterialReader::parse(MTL::BumpMap& m)
{
	current.mapBump = m;
	return current.hasMapBump = true;
}

void MaterialReader::done()
{
	if (!current.name.empty()) parse(current);
}



/* MaterialSaver */

void MaterialSaver::parse(Material &m)
{
	materials[m.name] = m;
}

const Material& MaterialSaver::find(const std::string &name) const
{
	return materials.at(name);
}

bool MaterialSaver::exists(const std::string &name) const
{
	return materials.find(name) != materials.end();
}

void MaterialSaver::clear()
{
	materials.clear();
}

std::size_t MaterialSaver::size() const
{
	return materials.size();
}
