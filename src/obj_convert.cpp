#include <SFML/Graphics.hpp>
#include "Common/Common.hpp"
#include <iostream>

class Materials : public OBJ::MaterialReader {
protected:
	
	void parse(OBJ::Material &m) override;
	
private:
	
	std::vector<OBJ::Material> materials;
	
};

class Triangles : public OBJ::TriangleReader {
	
	bool parse(OBJ::Triangle &t) override;
	bool parse(OBJ::Render::MaterialLib& r) override;
	bool parse(OBJ::Render::UseMaterial& r) override;
	bool parse(OBJ::Grouping::Smoothing& g) override;
	bool parse(OBJ::Grouping::Groups& g) override;
	
private:
	
	std::vector<OBJ::Triangle> triangles;
	Materials materials;
	
};

int main(int argc, char* args[]) {
	
	/* Check arguments */
	if (argc != 2) {
		Popup("Error!", "No input files.").show();
		return 0;
	}
	
	Triangles triangles;
	triangles.read(args[1], std::cout);
	
	return 0;
}

void Materials::parse(OBJ::Material &m) {
	materials.push_back(m);
}

bool Triangles::parse(OBJ::Triangle &t) {
	triangles.push_back(t);
	return true;
}

bool Triangles::parse(OBJ::Render::MaterialLib& r) {
	for (std::size_t i = 0; i < r.files.size(); i++) {
		materials.read(r.files[i], std::cout);
	}
	return true;
}

bool Triangles::parse(OBJ::Render::UseMaterial& r) {
	(void) r;
	return true;
}

bool Triangles::parse(OBJ::Grouping::Smoothing& g) {
	(void) g;
	return true;
}

bool Triangles::parse(OBJ::Grouping::Groups& g) {
	(void) g;
	return true;
}

