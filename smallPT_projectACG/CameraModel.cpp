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
}

Ray CameraModel::generateRay(int x, int y, int sx, int sy, double dx, double dy, unsigned short *Xi) const {
	double r = apertureRadius*erand48(Xi);
	double phi = 2*M_PI*erand48(Xi);

	Vec viewPoint = viewBottomLeft + cx * (((sx + .5 + dx) / 2 + x)) + cy * (((sy + .5 + dy) / 2 + y));
	Vec newCamPosition = camPosition + u*(r*cos(phi)) + v*(r*sin(phi));

	return Ray(newCamPosition, (viewPoint - newCamPosition).norm());
}

CameraModel::~CameraModel() {
}

