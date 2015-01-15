#include <string>
#include <set>
#include <map> 
#include "Lsystem.hpp"

using namespace std;

string Deterministic_LSystem::apply(int generations){
	string begin (1,start);
	return apply(begin,generations);
}

string Deterministic_LSystem::apply(string begin,int generations){
	string new_string = "";

	for (int i = 0; i < generations; i++)
		for (auto c : begin){
			 map<char, string>::iterator it = rules.find(c);
			if (it != rules.end())			// if rule found, apply string
				new_string += (it->second);
			else							// if not, keep the original character
				new_string += c;
		}

	return new_string;
}

//TODO: Finish a parser for .lsystem files
Deterministic_LSystem Deterministic_LSystem::from_file(string name){
	


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