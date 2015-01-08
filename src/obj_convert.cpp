#include "Common/Common.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <glm/glm.hpp>

struct Converter : public OBJ::ElementReader {
	
	CFR::Geometry &geometry;
	CFR::Model    &model;
	std::time_t lastReport = 0;
	std::size_t lines      = 0;
	OBJ::MaterialSaver materials;
	OBJ::Material      material;
	CFR::size_type     lastElements = 0;
	std::string        lastMaterial;
	
	Converter(CFR::Geometry &geometry, CFR::Model &model);
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
	if (argc <= 1) {
		std::cerr << "Error: No input files." << std::endl;
		std::cin.get();
		return -1;
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
	geometry.setTypeTexcoord(CFR::TYPE_HALF_FLOAT);
	geometry.setTypeNormal  (CFR::TYPE_HALF_FLOAT);
	geometry.setTypeTangent (CFR::TYPE_HALF_FLOAT);
	
	/* Model */
	CFR::Model model(removePath(fileGeometry));
	model.setHeader("CFR Model generated from " + to_string(removePath(args[1])));
	
	/* Read obj file */
	Converter c(geometry, model);
	c.lines = lines;
	c.read(args[1], std::cout);
	
	/* Save geometry */
	try {
		std::cout << "Saving geometry to " << removePath(fileGeometry) << std::endl;
		geometry.saveToFile(fileGeometry);
	} catch (CFR::Exception &fail) {
		std::cerr << "Error saving geometry: " << fail.what() << std::endl;
		std::cin.get();
		return -1;
	}
	
	/* Save model */
	std::cout << "Saving model to " << removePath(fileModel) << std::endl;
	model.saveToFile(fileModel);
	
	/* Wait for input */
	std::cout << "\nFinished." << std::endl;
	std::cin.get();
	
	return 0;
}

CFR::Vec3 createVec3(float x, float y, float z) { CFR::Vec3 vec; vec.x = x; vec.y = y; vec.z = z; return vec; }
CFR::Vec2 createVec2(float x, float y) { CFR::Vec2 vec; vec.x = x; vec.y = y; return vec; }

Converter::Converter(CFR::Geometry &geometry, CFR::Model &model) : geometry(geometry), model(model) {}
bool Converter::parse(OBJ::Vertex::Geometry&  v) { report(false); return ElementReader::parse(v); }
bool Converter::parse(OBJ::Grouping::Groups&   ) { report(false); return true; }
bool Converter::parse(OBJ::Grouping::Smoothing&) { report(false); return true; }
bool Converter::parse(OBJ::Triangle &t) {
	CFR::Vertex a, b, c;
	a.position = createVec3(t.a.position.x, t.a.position.y, t.a.position.z);
	b.position = createVec3(t.b.position.x, t.b.position.y, t.b.position.z);
	c.position = createVec3(t.c.position.x, t.c.position.y, t.c.position.z);
	if (geometry.getTypeNormal() != CFR::TYPE_DISABLE) {
		addNormal(t.a, t.b, t.c);
		addNormal(t.b, t.c, t.a);
		addNormal(t.c, t.a, t.b);
		a.normal = createVec3(t.a.normal.x, t.a.normal.y, t.a.normal.z);
		b.normal = createVec3(t.b.normal.x, t.b.normal.y, t.b.normal.z);
		c.normal = createVec3(t.c.normal.x, t.c.normal.y, t.c.normal.z);
	}
	bool hasUV = t.a.hasTexture && t.b.hasTexture && t.c.hasTexture;
	if (hasUV) {
		a.texcoord = createVec2(t.a.texture.x, t.a.texture.y);
		b.texcoord = createVec2(t.b.texture.x, t.b.texture.y);
		c.texcoord = createVec2(t.c.texture.x, t.c.texture.y);
	} else if (geometry.getTypeTexcoord() != CFR::TYPE_DISABLE || geometry.getTypeTangent() != CFR::TYPE_DISABLE) {
		std::cout << "Disabling texture coordinates and tangents.\n";
		geometry.setTypeTexcoord(CFR::TYPE_DISABLE);
		geometry.setTypeTangent (CFR::TYPE_DISABLE);
		std::cout << "Recalculating geometry.\n";
		CFR::size_type count = geometry.getVertexCount();
		geometry.recalculate();
		count -= geometry.getVertexCount();
		std::cout << count << " vertices removed.\n";
	}
	if (geometry.getTypeTangent() != CFR::TYPE_DISABLE) {
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
	glm::vec3 normalA(a.normal.x, a.normal.y, a.normal.z);
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
	float r = 1.0f / (deltaTexB.x * deltaTexC.y - deltaTexC.x * deltaTexB.y);
	glm::vec3 sdir = r * (deltaTexC.y * deltaPosB - deltaTexB.y * deltaPosC);
	glm::vec3 tdir = r * (deltaTexB.x * deltaPosC - deltaTexC.x * deltaPosB);
	glm::vec4 tangent = glm::vec4(glm::normalize(sdir - normalA * glm::dot(normalA, sdir)), 1.f);
	if (glm::dot(glm::cross(normalA, sdir), tdir) < 0.f) tangent.w = -1.f;
	a.tangent.x = tangent.x;
	a.tangent.y = tangent.y;
	a.tangent.z = tangent.z;
	a.tangent.w = tangent.w;
}
bool Converter::parse(OBJ::Render::UseMaterial &m) {
	if (m.name.compare(lastMaterial) == 0) return true;
	done();
	material = materials.find(m.name);
	lastMaterial = m.name;
	report(false);
	return true;
}
bool Converter::parse(OBJ::Render::MaterialLib &m) {
	for (std::size_t i = 0; i < m.files.size(); i++) materials.read(m.files[i], std::cout);
	std::cout << materials.size() << " Materials loaded.\n";
	return true;
}
void Converter::done() {
	CFR::size_type currentElements = geometry.getElementCount();
	if (currentElements - lastElements == 0) return;
	CFR::ModelObject object;
	object.start = lastElements;
	object.end   = currentElements;
	if (material.hasDiffuse)     object.diffuse = createVec3(material.diffuse.r, material.diffuse.g, material.diffuse.b);
	if (material.hasMapDiffuse)  object.diffuse_map = material.mapDiffuse.file;
	if (material.hasEmission)    object.emit = (material.emission.r + material.emission.g + material.emission.b) / 3.f;
	if (material.hasSpecularExponent) object.specular_exp = material.specularExponent.value;
	if (material.hasSpecular)    object.specular = (material.specular.r + material.specular.g + material.specular.b) / 3.f;
	if (material.hasMapSpecular) object.specular_map = material.mapSpecular.file;
	if (material.hasMapAlpha)    object.mask_map = material.mapAlpha.file;
	model.addObject(object);
	lastElements = currentElements;
}
void Converter::report(bool force) {
	std::time_t now = std::time(nullptr);
	if (force || now > lastReport) {
		lastReport = now;
		float done = (100.f * getLineNumber()) / lines;
		std::cout << "Progress " << done << "%";
		std::cout << " Line " << getLineNumber();
		std::cout << " Elements " << geometry.getElementCount();
		std::cout << " Vertices " << geometry.getVertexCount();
		std::cout << std::endl;
	}
}
