/*
 * CameraModel.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: steve
 */

#include "CameraModel.hpp"
#include "SmallPT.hpp"
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

	double viewHalfWidth = focalDistance * tan((fov) / 2.0);
	double aspectRatio = height / width;
	double viewHalfHeight = aspectRatio * viewHalfWidth;

	viewBottomLeft = camLookAt - (v * viewHalfHeight) - (u * viewHalfWidth);
	cx = (u*2*viewHalfWidth)/width;
	cy = (v*2*viewHalfHeight)/height;

	apertureX = u * apertureRadius;
	apertureY = v * apertureRadius;
}

double CameraModel::generateRandomNumber() const {
	unsigned short Xi[3];
	return (2.0 * erand48(Xi) - 1.0);
}

Ray CameraModel::generateRay(int x, int y, int sx, int sy, double dx, double dy) const {
	double r1 = generateRandomNumber();
	double r2 = generateRandomNumber();
	Vec viewPoint = viewBottomLeft + cx * (((sx + .5 + dx) / 2 + x)) + cy * (((sy + .5 + dy) / 2 + y));
	Vec newCamPosition = camPosition + (apertureX * r1) + (apertureY * r2);
	//Vec d = cx * (((sx + .5 + dx) / 2 + x) / w - .5) + cy * (((sy + .5 + dy) / 2 + y) / h - .5) + cam.direction;
	return Ray(newCamPosition, (viewPoint - newCamPosition).norm());
}

CameraModel::~CameraModel() {
	// TODO Auto-generated destructor stub
}

