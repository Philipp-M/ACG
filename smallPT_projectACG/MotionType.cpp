#include "MotionType.h"


MotionType::MotionType(Type _type, Vec _goal) : type(_type), goal(_goal) {}
MotionType::MotionType(Type _type, float _rad, Dir _d) : type(_type), rad(_rad), d(_d) {}
MotionType::MotionType(Type _type, Vec _t, double _acc) : type(_type), t(_t), acc(_acc) {}
MotionType::MotionType() : type(Translation), goal(Vec(0, 0, 0)) {}

Type MotionType::get_type() {
	return type;
}
Vec	MotionType::get_goal() {
	return goal;
}
Vec MotionType::get_t() {
	return t;
}
Dir MotionType::get_dir() {
	return d;
}
double MotionType::get_acc() {
	return acc;
}
float MotionType::get_rad() {
	return rad;
}

