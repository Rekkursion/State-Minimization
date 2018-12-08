#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "State.h"

#define DEBUG

#define DOT_START_KISS	".start_kiss"
#define DOT_I			".i"
#define DOT_O			".o"
#define DOT_P			".p"
#define DOT_S			".s"
#define DOT_R			".r"
#define DOT_END_KISS	".end_kiss"

std::string inputKissFilename;
std::string outputKissFilename;
std::string outputDotFilename;

int inputBitNum;
int outputBitNum;
int inputTermNum;
int outputTermNum;
int inputStateNum;
int outputStateNum;
std::string resetState;

int outDegree;
std::map<std::string, State> stateList;

// get and analyze input data
bool getInput(int argc, char** argv) {
	if (argc != 4) {
		puts("Wrong input format.");
		return false;
	}

	inputKissFilename = argv[1];
	outputKissFilename = argv[2];
	outputDotFilename = argv[3];

	std::ifstream fin;
	fin.open(inputKissFilename.c_str(), std::ios::in);
	if (fin.fail()) {
		puts("Input file not found.");
		return false;
	}

	std::string lineHead;
	std::string curInput, curSourceState, curDestState, curOutput;

	while (fin >> lineHead) {
		// .start_kiss
		if (lineHead == DOT_START_KISS) {
		}

		// .i
		else if (lineHead == DOT_I) {
			fin >> inputBitNum;
			outDegree = (1 << inputBitNum);
		}

		// .o
		else if (lineHead == DOT_O) {
			fin >> outputBitNum;
		}

		// .p
		else if (lineHead == DOT_P) {
			fin >> inputTermNum;
		}

		// .s
		else if (lineHead == DOT_S) {
			fin >> inputStateNum;
		}

		// .r
		else if (lineHead == DOT_R) {
			fin >> resetState;
		}

		// .end_kiss
		else if (lineHead == DOT_END_KISS) {
			break;
		}

		// input terms
		else {
			curInput = lineHead;
			fin >> curSourceState >> curDestState >> curOutput;

			bool existed = false;
			for (std::map<std::string, State>::iterator it = stateList.begin(); it != stateList.end(); it++) {

				if (it->first == curSourceState) {
					it->second.insertNewTransform(curInput, curOutput, curDestState);
					existed = true;

					break;
				}
			}

			if (!existed) {
				State newState = State(curSourceState, curInput, curOutput, curDestState);
				stateList.insert(std::pair<std::string, State>(curSourceState, newState));
			}
		}
	}
	fin.close();

	return true;
}

// state minimize
void minimize() {
	bool isContinue = true;

	while (isContinue) {
		isContinue = false;

		for (std::map<std::string, State>::iterator it_p = stateList.begin(); it_p != stateList.end(); it_p++) {
			for (std::map<std::string, State>::iterator it_q = it_p; it_q != stateList.end(); it_q++) {
				if (it_p->first == it_q->first)
					continue;

				if (it_p->second == it_q->second) {
					for (std::map<std::string, State>::iterator it = stateList.begin(); it != stateList.end(); it++) {
						it->second.replaceNextStatesNamesWith(it_q->first, it_p->first);
					}

					stateList.erase(it_q);
					isContinue = true;

					break;
				}
			}

			if (isContinue)
				break;
		}
	}

	outputStateNum = stateList.size();
	outputTermNum = 0;

	for (std::map<std::string, State>::iterator it = stateList.begin(); it != stateList.end(); it++)
		outputTermNum += it->second.getOutDegree();
		
	return;
}

// make output KISS file
void makeKissOutput() {
	std::ofstream fout;
	fout.open(outputKissFilename.c_str(), std::ios::out);

	fout << ".start_kiss\n";
	fout << ".i " << inputBitNum << std::endl;
	fout << ".o " << outputBitNum << std::endl;
	fout << ".p " << outputTermNum << std::endl;
	fout << ".s " << outputStateNum << std::endl;
	fout << ".r " << resetState << std::endl;
	for (std::map<std::string, State>::iterator it_list = stateList.begin(); it_list != stateList.end(); it_list++)
		fout << it_list->second;
	fout << ".end_kiss\n";

	fout.close();

	return;
}

// make output DOT file
void makeDotOutput() {
	// Rekkursion
	std::ofstream fout;
	fout.open(outputDotFilename.c_str(), std::ios::out);

	fout << "digraph STG {\n\trankdir=LR;\n\t\n\tINIT [shape=point];\n";
	for (std::map<std::string, State>::iterator it = stateList.begin(); it != stateList.end(); it++)
		fout << "\t" << it->first << " [label=\"" << it->first << "\"];\n";
	fout << "\t\n";
	fout << "\tINIT -> " << resetState << ";\n";
	for (std::map<std::string, State>::iterator it = stateList.begin(); it != stateList.end(); it++)
		it->second.output2DotFile(fout);
	fout << "}\n";

	fout.close();

	return;
}