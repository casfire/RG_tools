#include <SFML/Graphics.hpp>
#include "Common/Common.hpp"
#include <iostream>
#include <map>

class Converter : public OBJ::TriangleReader {
public:
	
	CFR::Geometry geometry;
	bool hasTexture = true;
	bool hasNormal  = true;
	sf::Clock clock;
	
protected:
	
	bool parse(OBJ::Grouping::Groups& g) override;
	bool parse(OBJ::Grouping::Smoothing& g) override;
	bool parse(OBJ::Render::UseMaterial& r) override;
	bool parse(OBJ::Render::MaterialLib& r) override;
	bool parse(OBJ::Triangle &t) override;
	void report(bool force = false);
	
	CFR::size_type vertexStart = 0;
	CFR::size_type maxVertexPerObject = 8192;
};

int main(int argc, char* args[]) {
	
	/* Check arguments */
	if (argc != 2) {
		Popup("Error!", "No input file.").show();
		return 0;
	}
	
	/* Read obj file */
	Converter c;
	c.read(args[1], std::cout);
	
	/* Save geometry */
	try {
		c.geometry.setVertexType(3, c.hasNormal?3:0, c.hasTexture?2:0);
		c.geometry.setElementType(4);
		c.geometry.saveToFile(getPrefix(args[1], '.') + ".cfrg");
	} catch (CFR::Exception &fail) {
		Popup("Error!", "Failed: " + to_string(fail.what())).show();
		return 0;
	}
	
	/* End message */
	Popup("CFRG Converter", "Converted.").show();
	
	return 0;
}

bool Converter::parse(OBJ::Triangle &t)
{
	if (hasTexture && (!t.a.hasTexture || !t.b.hasTexture || !t.c.hasTexture)) {
		std::cout << "Turning off texture coordinates.\n";
		hasTexture = false;
	}
	if (hasNormal && (!t.a.hasNormal || !t.b.hasNormal || !t.c.hasNormal)) {
		std::cout << "Turning off normals.\n";
		hasNormal = false;
	}
	
	CFR::Uint32 a = geometry.addSimilarVertex(
		t.a.x, t.a.y, t.a.z,
		t.a.i, t.a.j, t.a.k,
		t.a.u, t.a.v, 0.f,
		0.001f, vertexStart
	);
	
	CFR::Uint32 b = geometry.addSimilarVertex(
		t.b.x, t.b.y, t.b.z,
		t.b.i, t.b.j, t.b.k,
		t.b.u, t.b.v, 0.f,
		0.001f, vertexStart
	);
	
	CFR::Uint32 c = geometry.addSimilarVertex(
		t.c.x, t.c.y, t.c.z,
		t.c.i, t.c.j, t.c.k,
		t.c.u, t.c.v, 0.f,
		0.001f, vertexStart
	);
	
	geometry.addElement(a);
	geometry.addElement(c);
	geometry.addElement(b);
	
	if (geometry.getVertexCount() - vertexStart > maxVertexPerObject) {
		vertexStart = geometry.getVertexCount();
	}
	
	report();
	return true;
}

bool Converter::parse(OBJ::Grouping::Groups&) {
	report(true);
	vertexStart = geometry.getVertexCount();
	return true;
}

void Converter::report(bool force) {
	if (force || clock.getElapsedTime().asMilliseconds() > 1000) {
		clock.restart();
		std::cout << "Line " << getLineNumber();
		std::cout << " Elements " << geometry.getElementCount();
		std::cout << " Vertices " << geometry.getVertexCount();
		std::cout << " Current " << (geometry.getVertexCount() - vertexStart);
		std::cout << "\n";
	}
}

bool Converter::parse(OBJ::Grouping::Smoothing&) { return true; }
bool Converter::parse(OBJ::Render::UseMaterial&) { return true; }
bool Converter::parse(OBJ::Render::MaterialLib&) { return true; }
