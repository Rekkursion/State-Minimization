#pragma once

#include <string>
#include <map>
#include <set>
#include <iostream>

#define NO_NAME "NULL"

class State {
public:
	State();
	State(std::string);
	State(std::string, std::string, std::string, std::string);
	~State();

	bool operator==(State&);
	friend std::ostream& operator<<(std::ostream&, State&);
	void output2DotFile(std::ostream&);
	void insertNewTransform(std::string, std::string, std::string);
	void replaceNextStatesNamesWith(std::string, std::string);
	int getOutDegree();

private:
	std::string name;
	std::map<std::string, std::string> outputs;
	std::map<std::string, std::string> nextStates;
};

