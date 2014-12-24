#pragma once
#ifndef _OBJ_PARSER_HPP_
#define _OBJ_PARSER_HPP_

#include "Objects.hpp"
#include <string>
#include <istream>
#include <ostream>
#include <cstddef> // std::size_t

namespace OBJ {
	
	
	
	class TokenParser;
	class OBJParser;
	class MTLParser;
	
	
	
	// See: http://paulbourke.net/dataformats/obj/
	// See: http://paulbourke.net/dataformats/mtl/
	
	
	
	/* Base token parser */
	class TokenParser {
	public:
		
		TokenParser();
		void read(const std::string &filename);
		void read(const std::string &filename, std::ostream &log);
		void read(std::istream &source);
		void read(std::istream &source, std::ostream &log);
		std::ostream& getLogger() const;
		std::size_t getLineNumber() const;
		
	protected:
		
		virtual void done();
		virtual bool token(const std::string &t, std::istream &in) = 0;
		
	private:
		
		bool parse(const std::string &line);
		std::string prefix;
		std::ostream *logger;
		std::size_t lineNumber;
		
	};
	
	
	
	/* Base obj parser */
	class OBJParser : public virtual TokenParser {
	protected:
		
		/* Vertex data */
		virtual bool parse(Vertex::Geometry& v);    // v
		virtual bool parse(Vertex::Texture& v);     // vt
		virtual bool parse(Vertex::Normal& v);      // vn
		
		/* Elements */
		virtual bool parse(Element::Point& e);      // p
		virtual bool parse(Element::Line& e);       // l
		virtual bool parse(Element::Face& e);       // f
		
		/* Grouping */
		virtual bool parse(Grouping::Groups& g);    // g
		virtual bool parse(Grouping::Smoothing& g); // s
		virtual bool parse(Grouping::Merge& g);     // mg
		virtual bool parse(Grouping::Object& g);    // o
		
		/* Display/render attributes */
		virtual bool parse(Render::Bevel& r);                 // bevel
		virtual bool parse(Render::ColorInterpolation& r);    // c_interp
		virtual bool parse(Render::DissolveInterpolation& r); // d_interp
		virtual bool parse(Render::LevelOfDetail& r);         // lod
		virtual bool parse(Render::UseMaterial& r);           // usemtl
		virtual bool parse(Render::MaterialLib& r);           // mtllib
		virtual bool parse(Render::ShadowObject& r);          // shadow_obj
		virtual bool parse(Render::TraceObject& r);           // trace_obj
		
	private:
		
		template<typename T>
		bool parser(std::istream& in);
		bool token(const std::string &t, std::istream &in) override;
		
	};
	
	
	
	/* Base mtl parser */
	class MTLParser : public virtual TokenParser {
	protected:
		
		virtual bool parse(MTL::NewMaterial& m);         // newmtl
		virtual bool parse(MTL::AmbientColor& m);        // Ka
		virtual bool parse(MTL::DiffuseColor& m);        // Kd
		virtual bool parse(MTL::SpecularColor& m);       // Ks
		virtual bool parse(MTL::EmissionColor& m);       // Ke
		virtual bool parse(MTL::Dissolve& m);            // d | Tr
		virtual bool parse(MTL::IlluminationModel& m);   // illum
		virtual bool parse(MTL::SpecularExponent& m);    // Ns
		virtual bool parse(MTL::RefractionIndex& m);     // Ni
		virtual bool parse(MTL::TransmittionFilter& m);  // Tf
		virtual bool parse(MTL::AmbientMap& m);          // map_Ka
		virtual bool parse(MTL::DiffuseMap& m);          // map_Kd
		virtual bool parse(MTL::SpecularColorMap& m);    // map_Ks
		virtual bool parse(MTL::SpeculaHighlightMap& m); // map_Ns
		virtual bool parse(MTL::AlphaMap& m);            // map_d
		virtual bool parse(MTL::BumpMap& m);             // map_bump | bump
		
	private:
		
		template<typename T>
		bool parser(std::istream& in);
		bool token(const std::string& t, std::istream& in) override;
		
	};
	
	
	
} // namespace OBJ

#endif // _OBJ_PARSER_HPP_
