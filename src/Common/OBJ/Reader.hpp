#pragma once
#ifndef _OBJ_READER_HPP_
#define _OBJ_READER_HPP_

#include "Objects.hpp"
#include <string>
#include <istream>
#include <ostream>

namespace OBJ {
	
	
	
	class TokenReader;
	class ObjectReader;
	class MaterialReader;
	
	
	
	// See: http://paulbourke.net/dataformats/obj/
	// See: http://paulbourke.net/dataformats/mtl/
	
	
	
	/* Base reader */
	class TokenReader {
	public:
		
		bool read(const std::string &filename);
		bool read(const std::string &filename, std::ostream &log);
		bool read(std::istream &source);
		bool read(std::istream &source, std::ostream &log);
		
	protected:
		
		virtual void done();
		virtual const char* prefix() = 0;
		virtual bool token(const std::string &t, std::istream &in) = 0;
		
	private:
		
		bool line(const std::string &line);
		
	};
	
	
	
	/* Base obj reader */
	class ObjectReader : public virtual TokenReader {
	protected:
		
		/* Vertex data */
		virtual bool parse(Vertex::Geometry& v);
		virtual bool parse(Vertex::Texture& v);
		virtual bool parse(Vertex::Normal& v);
		
		/* Elements */
		virtual bool parse(Element::Point& e);
		virtual bool parse(Element::Line& e);
		virtual bool parse(Element::Face& e);
		
		/* Grouping */
		virtual bool parse(Grouping::Groups& g);
		virtual bool parse(Grouping::Smoothing& g);
		virtual bool parse(Grouping::Merge& g);
		virtual bool parse(Grouping::Object& g);
		
		/* Display/render attributes */
		virtual bool parse(Render::Bevel& r);
		virtual bool parse(Render::ColorInterpolation& r);
		virtual bool parse(Render::DissolveInterpolation& r);
		virtual bool parse(Render::LevelOfDetail& r);
		virtual bool parse(Render::UseMaterial& r);
		virtual bool parse(Render::MaterialLib& r);
		virtual bool parse(Render::ShadowObject& r);
		virtual bool parse(Render::TraceObject& r);
		
	private:
		
		template<typename T>
		bool parser(std::istream& in);
		bool token(const std::string &t, std::istream &in) override;
		const char* prefix() override;
		
	};
	
	
	
	/* Base mtl reader */
	class MaterialReader : public virtual TokenReader {
	protected:
		
		virtual bool parse(Material::NewMaterial& m);
		virtual bool parse(Material::AmbientColor& m);
		virtual bool parse(Material::DiffuseColor& m);
		virtual bool parse(Material::SpecularColor& m);
		virtual bool parse(Material::Dissolve& m);
		virtual bool parse(Material::IlluminationModel& m);
		virtual bool parse(Material::AmbientMap& m);
		virtual bool parse(Material::DiffuseMap& m);
		virtual bool parse(Material::SpecularColorMap& m);
		virtual bool parse(Material::SpeculaHighlightMap& m);
		virtual bool parse(Material::AlphaMap& m);
		virtual bool parse(Material::BumpMap& m);
		
	private:
		
		template<typename T>
		bool parser(std::istream& in);
		bool token(const std::string& t, std::istream& in) override;
		const char* prefix() override;
		
	};
	
	
	
} // namespace OBJ

#endif // _OBJ_READER_HPP_
