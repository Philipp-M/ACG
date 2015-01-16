#pragma once
#include "Point.hpp"
#include <vector>
#include <cmath>

template<typename T>
class DrainagePolygon
{
private:
	std::vector<Point2<T> > vertices;
public:
	void setVertices(const std::vector<Point2<T> >& verts) { vertices = verts; }
	/**
	 * splits the Polygon based on a line defined by two Points, it does just split it in two parts.
	 * If there are other intersections with the line only the nearest intersections to the points are taken
	 * It's kind of a brute force approach, since it's not the subject to do most efficient code
	 *
	 * @return true on success(there is a right and left polygon)
	 */
	bool split(Point2<T> first, Point2<T> secnd, DrainagePolygon<T>& left, DrainagePolygon<T>& right);
	/**
	 * splits the Polygon based on a line defined by a start Point and an angle, for the rest see above
	 */
	bool split(Point2<T> start, T angle, DrainagePolygon<T>& left, DrainagePolygon<T>& right);
	bool isEmpty() { return vertices.empty(); }
	void clear() { vertices.clear(); };

	friend std::ostream& operator<<(std::ostream& os, const DrainagePolygon<T>& dt)
	{

		os << "DrainagePolygon {" << std::endl;
		for (const Point2<T>& vert : dt.vertices)
		{
			os << '\t' << vert << std::endl;
		}
		os << '}' << std::endl;
		return os;

	}
};

template<typename T>
bool DrainagePolygon<T>::split(Point2<T> first, Point2<T> secnd, DrainagePolygon<T>& left, DrainagePolygon<T>& right)
{
	if(!isEmpty())
	{
		std::vector<Point2<T> > intersections;
		const Point2<T>& lastP = vertices[0];
		for(size_t i = 1; i < vertices.size(); i++ )
		{
			const Point2<T>& newP = vertices[i];
			T denom = (first.x-secnd.x)*(lastP.y-newP.y) -
					(first.y-secnd.y)*(lastP.x-newP.x);
			if(denom == 0) // parallel
				continue;
			// calculate the intersection of the lines
			T x = ((first.x*secnd.y-first.y*secnd.x)*(lastP.x-newP.x) - (first.x-secnd.x)*(lastP.x*newP.y-lastP.y*newP.x))/denom;
			T y = ((first.x*secnd.y-first.y*secnd.x)*(lastP.y-newP.y) - (first.y-secnd.y)*(lastP.x*newP.y-lastP.y*newP.x))/denom;
			Point2<T> intPoint(x,y);
			// check if the intersection is in the line segment
			if((((x < lastP.x && x > newP.x) || (x > lastP.x && x < newP.x) ) &&
					((y < lastP.y && y > newP.y) || (y > lastP.y && y < newP.y))) || intPoint == lastP || intPoint == newP)
				intersections.push_back(intPoint);
		}
		// check intersection for the nearest
		const Point2<T> * nearest = nullptr;
		const Point2<T> * secndNearest = nullptr;
		T shrtL = 1e10;
		for(const Point2<T>& intP : intersections)
		{
			T tmp = (first-intP).length();
			if(tmp < shrtL)
			{
				shrtL = tmp;
				secndNearest = nearest;
				nearest = &intP;
			}
		}
		// for a reason only one or no intersection(s)
		if(secndNearest == nullptr)
			return false;
		//splitting the Polygon
		std::vector<Point2<T>> leftVertices, rightVertices;
		for(const Point2<T>& vert : vertices)
		{
			T orient = (secnd.x-first.x)*(vert.y-first.y) - (secnd.y-first.y)*(vert.x-first.x);
			if(orient < 0)
				rightVertices.push_back(vert);
			else if(orient > 0)
				leftVertices.push_back(vert);
			else // if orient == 0
			{
				leftVertices.push_back(vert);
				rightVertices.push_back(vert);
			}
		}
		// should be splitted in two parts...
		if (rightVertices.empty() || leftVertices.empty())
			return false;
		if ((rightVertices[0] - *nearest).length() < (rightVertices[0] - *secndNearest).length())
		{
			rightVertices.insert(rightVertices.begin(), *nearest);
			rightVertices.push_back(*secndNearest);
		}
		else
		{
			rightVertices.insert(rightVertices.begin(), *secndNearest);
			rightVertices.push_back(*nearest);
		}
		if ((leftVertices[0] - *nearest).length() < (leftVertices[0] - *secndNearest).length())
		{
			leftVertices.insert(leftVertices.begin(), *nearest);
			leftVertices.push_back(*secndNearest);
		}
		else
		{
			leftVertices.insert(leftVertices.begin(), *secndNearest);
			leftVertices.push_back(*nearest);
		}
		right.setVertices(rightVertices);
		left.setVertices(leftVertices);
		return true;
	}
}

template<typename T>
bool DrainagePolygon<T>::split(Point2<T> start, T angle, DrainagePolygon<T>& left, DrainagePolygon<T>& right)
{
	split(start, Point2<T>(cos(angle), sin(angle)), left, right);
}
