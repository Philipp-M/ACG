#pragma once
#include <string>
#include <map> 
#include <random>
#include <chrono>


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
	float angle;
	char start;
public:
	LSystem(char _star, float _angle) :  start(_star), angle(_angle) {}
	float get_angle() { return angle; }
	string apply(int generations);
	virtual string apply(string start_from, int generations) = 0;
};


class Deterministic_LSystem : public LSystem{
private: 
	map<char,string> rules; 
	
public:
	Deterministic_LSystem(map<char, string> _rules, char start, float angle) : LSystem(start,angle), rules(_rules) {}
	using LSystem::apply;
	string apply(string start_from,int generations);
	static Deterministic_LSystem* from_file(string name);
};

class Stochastic_LSystem : public LSystem {
private:
	mt19937 generator;	//random number generator
	multimap<char, string> rules; //allows for multiple values per key (e.g.: 'a' -> 'B' and 'a' -> 'C' )

public:
	Stochastic_LSystem(multimap<char, string> _rules, char start,float angle) : LSystem(start,angle), rules(_rules) {
		chrono::high_resolution_clock::duration d = chrono::system_clock::now().time_since_epoch();
		generator.seed((unsigned long)d.count());
	}
	using LSystem::apply;
	string apply(string start_from, int generations);
	static Stochastic_LSystem* from_file(string name);
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
