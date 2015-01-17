#include <string>
#include <set>
#include <map> 
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Lsystem.hpp"

using namespace std;

string LSystem::apply(int generations){
	string begin (1,start);
	return apply(begin,generations);
}

string Deterministic_LSystem::apply(string begin,int generations){
	string new_string = "";

	for (int i = 0; i < generations; i++){
		for (auto c : begin){
			map<char, string>::iterator it = rules.find(c);
			if (it != rules.end())			// if rule found, apply string
				new_string += (it->second);
			else							// if not, keep the original character
				new_string += c;
		}
		begin = new_string;
	}

	return new_string;
}

Deterministic_LSystem Deterministic_LSystem::from_file(string name){
	ifstream file;
	file.open(name);
	if (!file.is_open()){
		cerr << "could not open file " << "\"" << name  << "\"" << endl;
		exit(1);
	}

	string tmp;
	getline(file, tmp);
	if (tmp != "DETERMINISTIC"){
		cerr << "File does not describe a D0L. \n";
		exit(1);
	}

	float angle = NAN;
	char start = '\0';

	//read attributes
	getline(file, tmp);	
	sscanf(tmp.c_str(), "s: %f",&angle);
	getline(file, tmp);
	sscanf(tmp.c_str(), "w: %c", &start);

	if ((angle == NAN) || (start == '\0')){
		cerr << "file not properly formatted: couldn't read attributes" << endl;
		exit(1);
	}

	cout << "LSystem: " << name << endl;
	cout << "Starting character: " << start << endl;
	cout << "Fixed angle: " << angle << endl;

	getline(file, tmp);

	map<char, string> rules;
	while (tmp != ""){
		int rule = 0;
		char symbol = '\0';
		char tmp2[100];

		sscanf(tmp.c_str(), "r%i: %c -> %s\n", &rule,&symbol,tmp2);
		
		string rewrite(tmp2);

		if ((rule == 0) || (symbol == '\0') ||  (rewrite == "")){
			cerr << "file not properly formatted: couldn't read rule" << endl;
			exit(1);
		}
		cout << "Rule " << rule << " : " << symbol << " -> " << "\"" << rewrite << "\"" << endl;
		rules.insert(pair<char, string>(symbol, rewrite));

		getline(file, tmp);
	}
	cout << endl;
	return Deterministic_LSystem(rules, start,angle);
}


string Stochastic_LSystem::apply(string begin, int generations){
	string new_string = "";

	for (int i = 0; i < generations; i++){
		for (auto c : begin){
			auto ret = rules.equal_range(c);	// range from first rule, to last

			multimap<char, string>::iterator it = ret.first;
			vector<string> strings;
			for (it = ret.first; it != ret.second; it++);	//put all rules into a vector
				strings.push_back(it->second);
			
			if (it != rules.end()) 			// if rule found, apply string
				if (strings.size() != 1){		// use random choice if multipe rules present
					uniform_int_distribution<int> distribution(0, strings.size() - 1);
	
					new_string += strings[distribution(generator)];
				}
				else							// otherwise identical to deterministic
					new_string += it->second;
			else							// if not, keep the original character
				new_string += c;
		}
		begin = new_string;
	}

	return new_string;
}


Stochastic_LSystem Stochastic_LSystem::from_file(string name){
	ifstream file;
	file.open(name);
	if (!file.is_open()){
		cerr << "could not open file " << "\"" << name << "\"" << endl;
		exit(1);
	}

	string tmp;
	getline(file, tmp);
	if (tmp != "STOCHASTIC"){
		cerr << "File does not describe a stochastic L-system. \n";
		exit(1);
	}

	float angle = NAN;
	char start = '\0';

	//read attributes
	getline(file, tmp);
	sscanf(tmp.c_str(), "s: %f", &angle);
	getline(file, tmp);
	sscanf(tmp.c_str(), "w: %c", &start);

	if ((angle == NAN) || (start == '\0')){
		cerr << "file not properly formatted: couldn't read attributes" << endl;
		exit(1);
	}

	cout << "LSystem: " << name << endl;
	cout << "Starting character: " << start << endl;
	cout << "Fixed angle: " << angle << endl;

	getline(file, tmp);

	multimap<char, string> rules;
	while (tmp != ""){
		int rule = 0;
		char symbol = '\0';
		char tmp2[100];

		sscanf(tmp.c_str(), "r%i: %c -> %s\n", &rule, &symbol, tmp2);

		string rewrite(tmp2);

		if ((rule == 0) || (symbol == '\0') || (rewrite == "")){
			cerr << "file not properly formatted: couldn't read rule" << endl;
			exit(1);
		}
		cout << "Rule " << rule << " : " << symbol << " -> " << "\"" << rewrite << "\"" << endl;
		rules.insert(pair<char, string>(symbol, rewrite));

		getline(file, tmp);
	}
	cout << endl;
	return Stochastic_LSystem(rules, start,angle);
}


//string ContextSensitive_LSystem::apply(){
//	string begin(1, start);
//	return apply(begin);
//}
//
//string ContextSensitive_LSystem::apply(string begin){
//	string new_string = "";
//
//	for (auto ch : begin){
//		context tmp;
//		tmp.c = '\0';
//		for (auto entry : rules)
//			if (entry.first.c == ch)
//				tmp = entry.first;
//		if (tmp.c == '\0')
//			continue;
//
//
//		map<char, string>::iterator it = rules.find(c);
//		if (it != rules.end)			// if rule found, apply string
//			new_string += (it->second);
//	}
//
//	return new_string;
//}