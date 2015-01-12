#pragma once
#include <string>
#include <set>
#include <map> 

typedef struct{
	std::string before;
	std::string after;
	char c;
} context;

typedef struct{
	float chance;
	char c;
} char_option;


class LSystem{
protected:
	char start;
	std::set<char> alphabet;
public:
	LSystem(std::set<char> _alphabet, char _star) : alphabet(_alphabet), start(_star){}

	virtual std::string apply();
	virtual std::string apply(std::string start_from);
};


class Deterministic_LSystem : public LSystem{
private: 
	std::map<char,std::string> rules; 
	
public:
	Deterministic_LSystem(std::set<char> alphabet, std::map<char, std::string> _rules, char start) : LSystem(alphabet, start), rules(_rules) {}
	std::string apply();
	std::string apply(std::string start_from);
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