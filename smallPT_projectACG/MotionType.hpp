#pragma once
#include "DataTypes.hpp"


enum Dir { X, Y, Z };
enum Type { Translation, Rotation, Acc };
class MotionType {
private:
	Type type;
	Vec3 goal;
	Vec3 t;
	Dir d;
	double acc;
	float rad;
public:
	MotionType(Type _type, Vec3 _goal);
	MotionType(Type _type, float _rad, Dir _d);
	MotionType(Type _type, Vec3 _t, double _acc);
	MotionType();

	Type get_type();
	Vec3 get_goal();
	Vec3 get_t();
	Dir get_dir();
	double get_acc();
	float get_rad();
};
