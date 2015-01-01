#include <SFML/Graphics.hpp>
#include "Common/Common.hpp"
#include <iostream>
#include <map>
#include <iomanip>
#include <glm/glm.hpp>
#include <fstream>

struct Converter : public OBJ::ElementReader {
	
	sf::Time reportInterval = sf::milliseconds(1000);
	sf::Clock clock;
	OBJ::MaterialSaver materials;
	OBJ::Material material;
	
	CFR::Geometry &geometry;
	std::ostream &model;
	CFR::size_type lastElements = 0;
	
	std::size_t lines;
	Converter(CFR::Geometry &geometry, std::ostream &model);
	
	bool parse(OBJ::Vertex::Geometry& v) override;
	bool parse(OBJ::Grouping::Groups& g) override;
	bool parse(OBJ::Grouping::Smoothing& g) override;
	bool parse(OBJ::Render::UseMaterial& r) override;
	bool parse(OBJ::Render::MaterialLib& r) override;
	bool parse(OBJ::Triangle &t) override;
	void done() override;
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
	
	/* Output files */
	std::string fileModel    = getPrefix(args[1], '.') + ".cfrm";
	std::string fileGeometry = getPrefix(args[1], '.') + ".cfrg";
	
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
	
	/* Model */
	std::ofstream model(fileModel);
	model << "#CFR Model generated from " << removePath(args[1]) << "\n";
	model << "version 1\n";
	model << "geometry " << removePath(fileGeometry) << "\n\n";
	
	/* Read obj file */
	Converter c(geometry, model);
	c.lines = lines;
	c.read(args[1], std::cout);
	
	/* Save geometry */
	try {
		std::cout << "Saving geometry to " << removePath(fileGeometry) << std::endl;
		geometry.saveToFile(fileGeometry);
	} catch (CFR::Exception &fail) {
		Popup("Error!", "Failed: " + to_string(fail.what())).show();
		return 0;
	}
	
	/* Close model */
	std::cout << "Saving model to " << removePath(fileModel) << std::endl;
	model.close();
	
	/* End message */
	Popup("CFRG Converter", "Converted.").show();
	
	return 0;
}

bool Converter::parse(OBJ::Triangle &t) {
	
	CFR::Vertex a, b, c;
	a.position = CFR::Vec3(t.a.position.x, t.a.position.y, t.a.position.z);
	b.position = CFR::Vec3(t.b.position.x, t.b.position.y, t.b.position.z);
	c.position = CFR::Vec3(t.c.position.x, t.c.position.y, t.c.position.z);
	
	if (geometry.getTypeNormal() != CFR::TYPE_DISABLE) {
		addNormal(t.a, t.b, t.c);
		addNormal(t.b, t.c, t.a);
		addNormal(t.c, t.a, t.b);
		a.normal = CFR::Vec3(t.a.normal.x, t.a.normal.y, t.a.normal.z);
		b.normal = CFR::Vec3(t.b.normal.x, t.b.normal.y, t.b.normal.z);
		c.normal = CFR::Vec3(t.c.normal.x, t.c.normal.y, t.c.normal.z);
	}
	
	bool hasUV = t.a.hasTexture && t.b.hasTexture && t.c.hasTexture;
	if (hasUV) {
		a.texcoord = CFR::Vec2(t.a.texture.x, t.a.texture.y);
		b.texcoord = CFR::Vec2(t.b.texture.x, t.b.texture.y);
		c.texcoord = CFR::Vec2(t.c.texture.x, t.c.texture.y);
	} else if (
		   geometry.getTypeTexcoord() != CFR::TYPE_DISABLE
		|| geometry.getTypeTangent()  != CFR::TYPE_DISABLE
		|| geometry.getTypeBinormal() != CFR::TYPE_DISABLE
	) {
		std::cout << "Disabling texture coordinates, tangents and binormals.\n";
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
	glm::vec3 posA(a.position.x, a.position.y, a.position.z);
	glm::vec3 posB(b.position.x, b.position.y, b.position.z);
	glm::vec3 posC(c.position.x, c.position.y, c.position.z);
	glm::vec2 texA(a.texcoord.x, a.texcoord.y);
	glm::vec2 texB(b.texcoord.x, b.texcoord.y);
	glm::vec2 texC(c.texcoord.x, c.texcoord.y);
	glm::vec2 deltaTexB = texB - texA;
	glm::vec2 deltaTexC = texC - texA;
	glm::vec3 deltaPosB = posB - posA;
	glm::vec3 deltaPosC = posC - posA;
	float r = 1.0f / (deltaTexB.x * deltaTexC.y - deltaTexB.y * deltaTexC.x);
	glm::vec3 tangent = (deltaPosB * deltaTexC.y - deltaPosC * deltaTexB.y) * r;
	glm::vec3 binormal = (deltaPosC * deltaTexB.x  - deltaPosB * deltaTexC.x)*r;
	a.tangent  = CFR::Vec3(tangent.x, tangent.y, tangent.z);
	a.binormal = CFR::Vec3(binormal.x, binormal.y, binormal.z);
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

void Converter::done() {
	CFR::size_type currentElements = geometry.getElementCount();
	if (currentElements - lastElements == 0) return;
	model << "range " << lastElements << " " << currentElements << "\n";
	OBJ::Material &m = material;
	if (m.hasAmbient)     model << "ambient       " << m.ambient.r << " " << m.ambient.g << " " << m.ambient.b << "\n";
	if (m.hasDiffuse)     model << "diffuse       " << m.diffuse.r << " " << m.diffuse.g << " " << m.diffuse.b << "\n";
	if (m.hasSpecular)    model << "specular      " << m.diffuse.r << " " << m.diffuse.g << " " << m.diffuse.b << "\n";
	if (m.hasMapAmbient)  model << "occlusion_map " << m.mapAmbient.file << "\n";
	if (m.hasMapDiffuse)  model << "diffuse_map   " << m.mapDiffuse.file << "\n";
	if (m.hasMapSpecular) model << "specular_map  " << m.mapSpecular.file << "\n";
	if (m.hasMapBump)     model << "bump_map      " << m.mapBump.file << "\n";
	std::cout << "Writing object " << lastElements << " " << currentElements << "\n";
	lastElements = currentElements;
	model << "end\n\n";
}

bool Converter::parse(OBJ::Render::UseMaterial &m) {
	done();
	material = materials.find(m.name);
	report(false);
	return true;
}

bool Converter::parse(OBJ::Render::MaterialLib &m) {
	for (std::size_t i = 0; i < m.files.size(); i++) materials.read(m.files[i], std::cout);
	std::cout << materials.size() << " Materials saved.\n";
	return true;
}

Converter::Converter(CFR::Geometry &geometry, std::ostream &model)
: geometry(geometry), model(model) {}
