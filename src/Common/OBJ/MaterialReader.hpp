#pragma once
#ifndef _OBJ_MATERIALREADER_HPP_
#define _OBJ_MATERIALREADER_HPP_

#include "Objects.hpp"
#include "Parser.hpp"
#include <string>
#include <map>
#include <exception>

namespace OBJ {
	
	
	
	struct Material {
		std::string name;
		MTL::AmbientColor        ambient;
		MTL::DiffuseColor        diffuse;
		MTL::SpecularColor       specular;
		MTL::EmissionColor       emission;
		MTL::Dissolve            dissolve;
		MTL::IlluminationModel   illum;
		MTL::SpecularExponent    specularExponent;
		MTL::RefractionIndex     refractionIndex;
		MTL::TransmittionFilter  transmittionFilter;
		MTL::AmbientMap          mapAmbient;
		MTL::DiffuseMap          mapDiffuse;
		MTL::SpecularColorMap    mapSpecular;
		MTL::SpeculaHighlightMap mapSpecularHighlight;
		MTL::AlphaMap            mapAlpha;
		MTL::BumpMap             mapBump;
		bool hasAmbient              = false;
		bool hasDiffuse              = false;
		bool hasSpecular             = false;
		bool hasDissolve             = false;
		bool hasEmission             = false;
		bool hasIllum                = false;
		bool hasSpecularExponent     = false;
		bool hasRefractionIndex      = false;
		bool hasTransmittionFilter   = false;
		bool hasMapAmbient           = false;
		bool hasMapDiffuse           = false;
		bool hasMapSpecular          = false;
		bool hasMapSpecularHighlight = false;
		bool hasMapAlpha             = false;
		bool hasMapBump              = false;
	};
	
	
	
	/* Material reader for mtl files */
	class MaterialReader : public MTLParser {
	protected:
		
		/* Called for each material */
		virtual void parse(Material &m);
		
		bool parse(MTL::NewMaterial& m) override;
		bool parse(MTL::AmbientColor& m) override;
		bool parse(MTL::DiffuseColor& m) override;
		bool parse(MTL::SpecularColor& m) override;
		bool parse(MTL::EmissionColor& m) override;
		bool parse(MTL::Dissolve& m) override;
		bool parse(MTL::IlluminationModel& m) override;
		bool parse(MTL::SpecularExponent& m) override;
		bool parse(MTL::RefractionIndex& m) override;
		bool parse(MTL::TransmittionFilter& m) override;
		bool parse(MTL::AmbientMap& m) override;
		bool parse(MTL::DiffuseMap& m) override;
		bool parse(MTL::SpecularColorMap& m) override;
		bool parse(MTL::SpeculaHighlightMap& m) override;
		bool parse(MTL::AlphaMap& m) override;
		bool parse(MTL::BumpMap& m) override;
		void done() override;
		
	private:
		
		Material current;
		
	};
	
	
	
	/* Stores all parsed materials */
	class MaterialSaver : public MaterialReader {
	public:
		
		const Material& find(const std::string &name) const;
		void clear();
		
	protected:
		
		void parse(Material &m) override;
		std::map<std::string, Material> materials;
		
	};
	
	
	
	/* Thrown by MaterialSaver::find() */
	class MaterialNotFoundException : public std::exception {
	public:
		
		MaterialNotFoundException(const std::string &name);
		const char* what() const throw();
		
	private:
		
		std::string name;
		
	};
	
	
	
} // namespace OBJ

#endif // _OBJ_MATERIALREADER_HPP_
