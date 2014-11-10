/*
 * CameraModel.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: steve
 */

#include "CameraModel.hpp"
#include <math.h>

CameraModel::CameraModel(Vec _camPosition, Vec _camDirection, double _fov, double _focalDistance, double _apertureRadius, unsigned int _width, unsigned int _height):
	camPosition(_camPosition), camDirection(_camDirection), fov(_fov), focalDistance(_focalDistance), apertureRadius(_apertureRadius), width(_width), height(_height)
{
	initCamera();
}

void CameraModel::initCamera() {
	camLookAt = camPosition + camDirection * focalDistance;

	u = (camDirection % Vec(0.0, 1.0, 0.0)).norm();
	v = (u % camDirection).norm();

	double viewHalfWidth = focalDistance * tan(fov/2);
	double aspectRatio = height / width;
	double viewHalfHeight = aspectRatio * viewHalfWidth;

	viewBottomLeft = camLookAt - (v * viewHalfHeight) - (u * viewHalfWidth);
	cx = (u*2*viewHalfWidth)/width;
	cy = (v*2*viewHalfHeight)/height;

	apertureX = u * apertureRadius;
	apertureY = v * apertureRadius;
}

double CameraModel::generateRandomeNumber(double min, double max) {
	double r;
	srand(time(NULL));
	r = (double) rand() / RAND_MAX;
	return min + r * (max - min);
}

Ray CameraModel::generateRay(int x, int y) {
	double r1 = generateRandomeNumber(-1.0, 1.0);
	double r2 = generateRandomeNumber(-1.0, 1.0);
	Vec viewPoint = viewBottomLeft + cx*x + cy*y;
	Vec newCamPosition = camPosition + r1 * apertureX + r2 * apertureY;
	return Ray(newCamPosition, (viewPoint - newCamPosition).norm());
}

CameraModel::~CameraModel() {
	// TODO Auto-generated destructor stub
}

