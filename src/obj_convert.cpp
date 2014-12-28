#include <SFML/Graphics.hpp>
#include "Common/Common.hpp"
#include <iostream>
#include <map>
#include <iomanip>
#include <glm/glm.hpp>

struct Converter : public OBJ::ElementReader {
	
	sf::Time reportInterval = sf::milliseconds(1000);
	
	sf::Clock clock;
	const std::size_t lines;
	CFR::Geometry &geometry;
	
	Converter(std::size_t lines, CFR::Geometry &geometry);
	bool parse(OBJ::Vertex::Geometry& v) override;
	bool parse(OBJ::Grouping::Groups& g) override;
	bool parse(OBJ::Grouping::Smoothing& g) override;
	bool parse(OBJ::Render::UseMaterial& r) override;
	bool parse(OBJ::Render::MaterialLib& r) override;
	bool parse(OBJ::Triangle &t) override;
	void report(bool force);
	void addNormal(OBJ::TriangleVertex &a, const OBJ::TriangleVertex &b, const OBJ::TriangleVertex &c);
	void addTangent(CFR::Vertex &v, const CFR::Vertex &b, const CFR::Vertex &c);
	
};

int main(int argc, char* args[]) {
	
	/* Check arguments */
	if (argc != 2) {
		Popup("Error!", "No input file.").show();
		return 0;
	}
	
	/* Find number of lines */
	std::cout << "Counting lines." << std::endl;
	std::size_t lines = countFileLines(args[1]);
	std::cout << "Total lines: " << lines << std::endl;
	
	/* Set float precision */
	std::cout << std::fixed << std::setprecision(2);
	
	/* Geometry */
	CFR::Geometry geometry;
	geometry.setTypePosition(CFR::TYPE_FLOAT);
	geometry.setTypeTexcoord(CFR::TYPE_FLOAT);
	geometry.setTypeNormal  (CFR::TYPE_FLOAT);
	geometry.setTypeTangent (CFR::TYPE_FLOAT);
	geometry.setTypeBinormal(CFR::TYPE_FLOAT);
	
	/* Read obj file */
	Converter c(lines, geometry);
	c.read(args[1], std::cout);
	
	/* Save geometry */
	try {
		std::string out = getPrefix(args[1], '.') + ".cfrg";
		std::cout << "Saving geometry to " << removePath(out) << std::endl;
		geometry.saveToFile(out);
	} catch (CFR::Exception &fail) {
		Popup("Error!", "Failed: " + to_string(fail.what())).show();
		return 0;
	}
	
	/* End message */
	Popup("CFRG Converter", "Converted.").show();
	
	return 0;
}

bool Converter::parse(OBJ::Triangle &t) {
	
	CFR::Vertex a(t.a.position.x, t.a.position.y, t.a.position.z);
	CFR::Vertex b(t.b.position.x, t.b.position.y, t.b.position.z);
	CFR::Vertex c(t.c.position.x, t.c.position.y, t.c.position.z);
	
	if (geometry.getTypeNormal() != CFR::TYPE_DISABLE) {
		addNormal(t.a, t.b, t.c);
		addNormal(t.b, t.c, t.a);
		addNormal(t.c, t.a, t.b);
		a.nx = t.a.normal.x; a.ny = t.a.normal.y; a.nz = t.a.normal.z;
		b.nx = t.b.normal.x; b.ny = t.b.normal.y; b.nz = t.b.normal.z;
		c.nx = t.c.normal.x; c.ny = t.c.normal.y; c.nz = t.c.normal.z;
	}
	
	bool hasUV = t.a.hasTexture && t.b.hasTexture && t.c.hasTexture;
	if (hasUV) {
		a.u = t.a.texture.x; a.v = t.a.texture.y;
		b.u = t.b.texture.x; b.v = t.b.texture.y;
		c.u = t.c.texture.x; c.v = t.c.texture.y;
	} else if (
		   geometry.getTypeTexcoord() != CFR::TYPE_DISABLE
		|| geometry.getTypeTangent()  != CFR::TYPE_DISABLE
		|| geometry.getTypeBinormal() != CFR::TYPE_DISABLE
	) {
		std::cout << "Disabling uv's, tangents and binormals.\n";
		geometry.setTypeTexcoord(CFR::TYPE_DISABLE);
		geometry.setTypeTangent (CFR::TYPE_DISABLE);
		geometry.setTypeBinormal(CFR::TYPE_DISABLE);
		std::cout << "Recalculating geometry.\n";
		CFR::size_type count = geometry.getVertexCount();
		geometry.recalculate();
		count -= geometry.getVertexCount();
		std::cout << count << " vertices removed.\n";
	}
	
	if (
		   geometry.getTypeTangent()  != CFR::TYPE_DISABLE
		|| geometry.getTypeBinormal() != CFR::TYPE_DISABLE
	) {
		addTangent(a, b, c);
		addTangent(b, c, a);
		addTangent(c, a, b);
	}
	
	CFR::Uint32 ea = geometry.addVertex(a);
	CFR::Uint32 eb = geometry.addVertex(b);
	CFR::Uint32 ec = geometry.addVertex(c);
	
	geometry.addElement(ea);
	geometry.addElement(eb);
	geometry.addElement(ec);
	
	report(false);
	return true;
}

void Converter::addNormal(OBJ::TriangleVertex &a, const OBJ::TriangleVertex &b, const OBJ::TriangleVertex &c) {
	if (a.hasNormal) return;
	glm::vec3 va(a.position.x, a.position.y, a.position.z);
	glm::vec3 vb(b.position.x, b.position.y, b.position.z);
	glm::vec3 vc(c.position.x, c.position.y, c.position.z);
	glm::vec3 normal = glm::normalize(glm::cross(vc - va, vb - va));
	a.hasNormal = true;
	a.normal.x = normal.x;
	a.normal.y = normal.y;
	a.normal.z = normal.z;
}

void Converter::addTangent(CFR::Vertex &a, const CFR::Vertex &b, const CFR::Vertex &c) {
	glm::vec3 posA(a.px, a.py, a.pz);
	glm::vec3 posB(b.px, b.py, b.pz);
	glm::vec3 posC(c.px, c.py, c.pz);
	glm::vec2 texA(a.u, a.v);
	glm::vec2 texB(b.u, b.v);
	glm::vec2 texC(c.u, c.v);
	glm::vec2 deltaTexB = texB - texA;
	glm::vec2 deltaTexC = texC - texA;
	glm::vec3 deltaPosB = posB - posA;
	glm::vec3 deltaPosC = posC - posA;
	float r = 1.0f / (deltaTexB.x * deltaTexC.y - deltaTexB.y * deltaTexC.x);
	glm::vec3 tangent = (deltaPosB * deltaTexC.y - deltaPosC * deltaTexB.y) * r;
	glm::vec3 binormal = (deltaPosC * deltaTexB.x  - deltaPosB * deltaTexC.x)*r;
	a.tx = tangent.x;
	a.ty = tangent.y;
	a.tz = tangent.z;
	a.bx = binormal.x;
	a.by = binormal.y;
	a.bz = binormal.z;
}

void Converter::report(bool force) {
	if (force || clock.getElapsedTime() > reportInterval) {
		clock.restart();
		float done = (100.f * getLineNumber()) / lines;
		std::cout << done << "%";
		std::cout << " Line " << getLineNumber();
		std::cout << " Elements " << geometry.getElementCount();
		std::cout << " Vertices " << geometry.getVertexCount();
		std::cout << std::endl;
	}
}

bool Converter::parse(OBJ::Vertex::Geometry &v)  { report(false); return ElementReader::parse(v); }
bool Converter::parse(OBJ::Grouping::Groups&)    { report(true);  return true; }
bool Converter::parse(OBJ::Grouping::Smoothing&) { report(false); return true; }
bool Converter::parse(OBJ::Render::UseMaterial&) { report(false); return true; }
bool Converter::parse(OBJ::Render::MaterialLib&) { report(false); return true; }
Converter::Converter(std::size_t lines, CFR::Geometry &geometry) : lines(lines), geometry(geometry) {}
