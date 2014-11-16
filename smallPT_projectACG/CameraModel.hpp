/*
 * CameraModel.hpp
 *
 *  Created on: Nov 10, 2014
 *      Author: steve
 */

#ifndef CAMERAMODEL_HPP_
#define CAMERAMODEL_HPP_

#include "DataTypes.hpp"
#include "GScene.hpp"

class CameraModel {
public:
	/*
	 * Constructor for the CameraModel:
	 * Vec3 _camPosition			point where the camera is
	 * Vec3 _camDirection		vector where the camera is looking to
	 * double _fov				horizontal angle (radiant) defining the field of view
	 * double _focalDistance	distance to focal point from camPostion
	 * double _apertureRadius	radius for the size of the aperture
	 * unsigned int _width		width of the image
	 * unsigned int _height		height of the image
	 */
	CameraModel(Vec3 _camPosition, Vec3 _camDirection, double _fov, double _focalDistance, double _apertureRadius, unsigned int _width, unsigned int _height);

	virtual ~CameraModel();

	/**
	 * Method generates a Ray from the camPosition to a point in object space for a specified pixel.
	 * int x				pixel x value
	 * int y				pixel y value
	 * int sx				subpixel x value
	 * int sy				subpixel y value
	 * double dx			x value for tent filter
	 * double dy			y value for tent filter
	 * unsigned short *Xi	array for random double generation
	 */
	Ray generateRay(int x, int y, int sx, int sy, double dx, double dy, unsigned short *Xi) const;

private:
	/**
	 * Method called in constructor, setting up all important data for generateRay
	 */
	void initCamera();

	Vec3 camPosition, camDirection;
	Vec3 camLookAt, u, v;
	Vec3 viewBottomLeft;
	Vec3 cx, cy;
	double fov, focalDistance, apertureRadius;
	unsigned int width, height;
};

#endif /* CAMERAMODEL_HPP_ */
