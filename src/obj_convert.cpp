#include <SFML/Graphics.hpp>
#include "Common/Common.hpp"
#include <iostream>
#include <map>
#include <iomanip>
#include <glm/glm.hpp>
#include <fstream>

int main(int argc, char* args[]) {
	
	/* Check arguments */
	if (argc != 2) {
		Popup("Error!", "No input file.").show();
		return 0;
	}
	
	// TODO
	(void) args;
	
	return 0;
}
