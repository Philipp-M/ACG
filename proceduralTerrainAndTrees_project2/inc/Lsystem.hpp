#pragma once
#include <string>
#include <set>
#include <map> 

using namespace std;

typedef struct{
	string before;
	string after;
	char c;
} context;

typedef struct{
	float chance;
	char c;
} char_option;


class LSystem{
protected:
	char start;
	set<char> alphabet;
public:
	LSystem(set<char> _alphabet, char _star) : alphabet(_alphabet), start(_star){}

	string apply();
	string apply(string start_from);
};


class Deterministic_LSystem : public LSystem{
private: 
	map<char,string> rules; 
	
public:
	Deterministic_LSystem(set<char> alphabet, std::map<char, string> _rules, char start) : LSystem(alphabet, start), rules(_rules) {}
	string apply(int generations);
	string apply(string start_from,int generations);
	static Deterministic_LSystem from_file(string name);
};

//class ContextSensitive_LSystem : public LSystem{
//private:
//	std::map<context, std::string> rules;
//
//public:
//	ContextSensitive_LSystem(std::set<char> alphabet, std::map<context, std::string> _rules, char start) : LSystem(alphabet, start), rules(_rules) {}
//	std::string apply();
//	std::string apply(std::string start_from);
//};
//
//class Stochastic_LSystem : public LSystem {
//private:
//	std::map<char_option, std::string> rules;
//
//public:
//	Stochastic_LSystem(std::set<char> alphabet, std::map<char_option, std::string> _rules, char start) : LSystem(alphabet, start), rules(_rules) {}
//
//};