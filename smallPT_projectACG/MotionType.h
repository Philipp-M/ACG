#pragma once
#include "DataTypes.hpp"


enum Dir { X, Y, Z };
enum Type { Translation, Rotation, Acc };
class MotionType {
private:
	Type type;
	Vec goal;
	Vec t;
	Dir d;
	double acc;
	float rad;
public:
	MotionType(Type _type, Vec _goal);
	MotionType(Type _type, float _rad, Dir _d);
	MotionType(Type _type, Vec _t, double _acc);
	MotionType();

	Type get_type();
	Vec	get_goal();
	Vec get_t();
	Dir get_dir();
	double get_acc();
	float get_rad();
};
