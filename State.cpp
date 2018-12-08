#include "State.h"



State::State() {
	name = NO_NAME;
	outputs.clear();
	nextStates.clear();
}

State::State(std::string _name): State() {
	name = _name;
}

State::State(std::string curState, std::string input, std::string output, std::string nextState): State(curState) {
	insertNewTransform(input, output, nextState);
}

State::~State() {}

bool State::operator==(State& rhs) {
	std::map<std::string, std::string>::iterator it_right_n;
	std::map<std::string, std::string>::iterator it_right_o;

	if (this->outputs.size() != rhs.outputs.size() || this->nextStates.size() != rhs.nextStates.size())
		return false;

	for (std::map<std::string, std::string>::iterator it_left_o = this->outputs.begin(); it_left_o != this->outputs.end(); it_left_o++) {
		it_right_o = rhs.outputs.find(it_left_o->first);

		if (it_right_o == rhs.outputs.end() || it_right_o->second != it_left_o->second)
			return false;
	}

	for (std::map<std::string, std::string>::iterator it_left_n = this->nextStates.begin(); it_left_n != this->nextStates.end(); it_left_n++) {
		it_right_n = rhs.nextStates.find(it_left_n->first);

		if (it_right_n == rhs.nextStates.end() || it_right_n->second != it_left_n->second)
			return false;
	}

	return true;
}

std::ostream& operator<<(std::ostream& ostr, State& state) {
	std::map<std::string, std::string>::iterator it_n;
	
	for (std::map<std::string, std::string>::iterator it_o = state.outputs.begin(); it_o != state.outputs.end(); it_o++) {
		it_n = state.nextStates.find(it_o->first);

		if (it_n != state.nextStates.end())
			ostr << it_o->first << " " << state.name << " " << it_n->second << " " << it_o->second << std::endl;
	}

	return ostr;
}

void State::output2DotFile(std::ostream& ostr) {
	std::map<std::string, std::string>::iterator it_o;
	std::string nextStateName;
	std::string label;
	std::set<std::string> alreadyFoundStates;
	bool firstTransform;

	alreadyFoundStates.clear();
	for (std::map<std::string, std::string>::iterator it_n = nextStates.begin(); it_n != nextStates.end(); it_n++) {

		if (alreadyFoundStates.find(it_n->second) != alreadyFoundStates.end())
			continue;

		nextStateName = it_n->second;
		label = "";
		firstTransform = true;

		for (std::map<std::string, std::string>::iterator it_nn = nextStates.begin(); it_nn != nextStates.end(); it_nn++) {
			if (it_nn->second == nextStateName) {
				it_o = outputs.find(it_nn->first);

				if (it_o != outputs.end()) {
					if (firstTransform)
						label += it_nn->first + "/" + it_o->second;
					else
						label += "," + it_nn->first + "/" + it_o->second;

					firstTransform = false;
				}
			}
		}

		alreadyFoundStates.insert(nextStateName);
		ostr << "\t" << name << " -> " << nextStateName << " [label=\"" << label << "\"];\n";
	}

	return;
}

void State::insertNewTransform(std::string input, std::string output, std::string nextState) {
	outputs.insert(std::pair<std::string, std::string>(input, output));
	nextStates.insert(std::pair<std::string, std::string>(input, nextState));

	return;
}

void State::replaceNextStatesNamesWith(std::string oldName, std::string newName) {
	for (std::map<std::string, std::string>::iterator it = nextStates.begin(); it != nextStates.end(); it++) {
		if (it->second == oldName)
			it->second = newName;
	}

	return;
}

int State::getOutDegree() {
	return outputs.size();
}