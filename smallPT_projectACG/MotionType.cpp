#include "MotionType.hpp"


MotionType::MotionType(Type _type, Vec3 _goal) : type(_type), goal(_goal) {}
MotionType::MotionType(Type _type, float _rad, Dir _d) : type(_type), d(_d), rad(_rad) {}
MotionType::MotionType(Type _type, Vec3 _t, double _acc) : type(_type), t(_t), acc(_acc) {}
MotionType::MotionType() : type(Translation), goal(Vec3(0, 0, 0)) {}

Type MotionType::get_type() {
	return type;
}
Vec3	MotionType::get_goal() {
	return goal;
}
Vec3 MotionType::get_t() {
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

