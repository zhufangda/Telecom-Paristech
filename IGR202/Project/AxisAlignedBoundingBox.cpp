#include "AxisAlignedBoundingBox.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Ray.h"
#include <algorithm> 
#include "cmath"
#include <algorithm>

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
{
}

void AxisAlignedBoundingBox::init(const float& xMax, const float& xMin, const float& yMax, const float& yMin, const float& zMax, const float& zMin)
{
	
	x[0] = xMin;
	x[1] = xMax;
	y[0] = yMin;
	y[1] = yMax;
	z[0] = zMin;
	z[1] = zMax;

	m_positions.clear();
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				m_positions.push_back(Vec3f(x[i], y[j], z[k]));
			}
		}
	}

}

std::vector<Vec3f> AxisAlignedBoundingBox::positions()
{
	return this->m_positions;
}

unsigned int* AxisAlignedBoundingBox::indices()
{
	return this->m_indices;
}

float AxisAlignedBoundingBox::getZmin() const
{
	return this->z[0];
}

void AxisAlignedBoundingBox::drawBox(const float width) const 
{
	glLineWidth(width);

	const std::vector<float> colors(32, 1.0f);
	
	glVertexPointer(3, GL_FLOAT, sizeof(Vec3f), (GLvoid*)(m_positions.data()));
	glColorPointer(4, GL_FLOAT, 0, (GLvoid*)colors.data());
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (GLvoid*)(m_indices));
}

bool AxisAlignedBoundingBox::contain(const Vec3f& point) const
{
	if (point[0] >= x[0] && point[0] <= x[1] &&
		point[1] >= y[0] && point[1] <= y[1] &&
		point[2] >= z[0] && point[2] <= z[1]) {
		return true;
	}
	return false;
}


bool AxisAlignedBoundingBox::contain(const Ray & ray) const
{
	if (this->contain(ray.origin) || this->contain(ray.origin + ray.direction)) return true;
	return false;
}

/**
bool AxisAlignedBoundingBox::intersection(const Ray& r) const {
	if (contain(r)) return true;
	double tx1 = (x[0] - r.origin[0]) / r.direction[0];
	double tx2 = (x[1] - r.origin[0]) / r.direction[0];

	double tmin = std::min(tx1, tx2);
	double tmax = std::max(tx1, tx2);

	double ty1 = (y[0] - r.origin[1]) / r.direction[1];
	double ty2 = (y[1] - r.origin[1]) / r.direction[1];

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	double tz1 = (z[0] - r.origin[2]) / r.direction[2];
	double tz2 = (z[1] - r.origin[2]) / r.direction[2];
	
	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));
	
	bool resultat = tmax >= tmin;
	//if (resultat) this->drawBox(3.5);
	return resultat;


}/***/


bool Line_AABB_1d(float start, float dir, float min, float max, float& enter, float& exit)
{
	//If the line segment is more of a point, just check if it's within the segment
	if (fabs(dir) < 1.0E-8)
		return (start >= min && start <= max);

	//Find if the lines overlap
	float   ooDir = 1.0f / dir;
	float   t0 = (min - start) * ooDir;
	float   t1 = (max - start) * ooDir;

	//Make sure t0 is the "first" of the intersections
	if (t0 > t1)
		std::swap(t0, t1);

	//Check if intervals are disjoint
	if (t0 > exit || t1 < enter)
		return false;

	//Reduce interval based on intersection
	if (t0 > enter)
		enter = t0;
	if (t1 < exit)
		exit = t1;

	return true;
}

bool AxisAlignedBoundingBox::intersection(const Ray& r) const {

	float       enter = 0.0f;
	float       exit = 1.0f;

	//Check each dimension of Line/AABB for intersection
	if (!Line_AABB_1d(r.origin[0], r.direction[0], x[0], x[1], enter, exit))
		return false;
	if (!Line_AABB_1d(r.origin[1], r.direction[1], y[0], y[1], enter, exit))
		return false;
	if (!Line_AABB_1d(r.origin[2], r.direction[2], z[0], z[1], enter, exit))
		return false;

	//If there is intersection on all dimensions, report that poin
	return true;
}


AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
{
}

std::ostream & operator<<(std::ostream & output, const AxisAlignedBoundingBox& aabb) {
	output << "X:" << aabb.x[0] << " ~~ " << aabb.x[1] << "\tY:" << aabb.y[0] << " ~~ " << aabb.y[1] << "\tZ:" << aabb.z[0] << " ~~ " << aabb.z[1] << std::endl;
	return output;
}
