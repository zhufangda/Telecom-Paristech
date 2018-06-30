#ifndef  _RAY_H
#define _RAY_H

#include "Vec3.h"
#include "Mesh.h"
class Ray {
public:
	Vec3f origin;
	Vec3f direction;

	Ray() {
		this->origin.init(0.0f, 0.0f, 0.0f);
		this->direction.init(2.0f, 0.0f, 0.0f);
	}
	Ray(Vec3f origin, Vec3f direction) {
		this->origin = origin;
		this->direction = direction;
	}

	void setOrigin(Vec3f vector) {
		origin = vector;
	}

	void setDirection(Vec3f vector) {
		direction = vector;
		direction.normalize();
	}


	bool intersectionRayonTriangle (const Mesh& mesh, const Triangle& indice) const {
		const Vec3f& p0 = mesh.positions()[indice[0]];
		const Vec3f& p1 = mesh.positions()[indice[1]];
		const Vec3f& p2 = mesh.positions()[indice[2]];

		Vec3f e0 = p1 - p0;
		Vec3f e1 = p2 - p0;
		Vec3f n = cross(e0, e1) / length(cross(e0, e1));
		Vec3f q = cross(direction, e1);
		float a = dot(e0, q);

	//	if (dot(n, direction) > 0 || fabs(a) < 0.0001) return false;
		if ( fabs(a) < 0.0001) return false;
		Vec3f s = (origin - p0) / a;
		Vec3f r = cross(s, e0);
		float b0 = dot(s, q);
		float b1 = dot(r, direction);
		float b2 = 1 - b1 - b0;

		if (b0 < 0 || b1 < 0 || b2 < 0) {
			return false;
		}

		float t = dot(e1, r);
		if (t >= 0) {
			return true;
		}
		return false;
	}
};

#endif

