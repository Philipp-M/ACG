#include <string>
#include <set>
#include <map> 
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Lsystem.hpp"

using namespace std;

string Deterministic_LSystem::apply(int generations){
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
	double angle = NAN;
	char start = '\0';

	//read attributes
	getline(file, tmp);	
	sscanf(tmp.c_str(), "s: %lf",&angle);
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
	return Deterministic_LSystem(set<char>(), rules, start);
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