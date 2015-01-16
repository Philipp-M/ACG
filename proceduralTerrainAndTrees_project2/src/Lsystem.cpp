#include <string>
#include <set>
#include <map> 
#include "Lsystem.h"

using namespace std;

string Deterministic_LSystem::apply(){
	string begin (1,start);
	return apply(begin);
}

string Deterministic_LSystem::apply(string begin){
	string new_string = "";

	for (auto c : begin){
		 map<char, string>::iterator it = rules.find(c);
		if (it != rules.end)			// if rule found, apply string
			new_string += (it->second);
		else							// if not, keep the original character
			new_string += c;
	}

	return new_string;
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