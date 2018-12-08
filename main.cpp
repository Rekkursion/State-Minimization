#include "main.h"

int main(int argc, char** argv) {
	
	// get and analyze input data
	if (!getInput(argc, argv))
		return 0;
	
	// state minimize
	minimize();

	// make output KISS file
	makeKissOutput();

	// make output DOT file
	makeDotOutput();

	return 0;
}