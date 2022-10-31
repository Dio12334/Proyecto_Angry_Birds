#ifndef COLLISION_H
#define COLLISION_H

#include "Math.h"
#include <vector>

struct LineSegment{
	LineSegment(const Vector3& start, const Vector3& end);
	Vector3 pointOnSegment(float t) const;
	float minDistSq(const Vector3& point) const;
	static float minDistSq(const LineSegment& s1, const LineSegment& s2);

	Vector3 start;
	Vector3 end;
};

struct Plane{
	Plane(const Vector3& normal, float d);
	Plane(const Vector3& a, const Vector3& b, const Vector3& c);
	float signedDist(const Vector3& point) const;

	Vector3 normal;
	float d;
};

struct Sphere{
	Sphere(const Vector3& center, float radius);
	bool contains(const Vector3& point) const;

	Vector3 center;
	float radius;
};

struct AABB{
	AABB(const Vector3& min, const Vector3& max);
	void updateMinMax(const Vector3& point);
	void rotate(const Quaternion& q);
	bool contains(const Vector3& point) const;
	float minDistSq(const Vector3& point) const;

	Vector3 min;
	Vector3 max;
};

struct OBB{
	Vector3 center;
	Quaternion rotation;
	Vector3 extents;
};

struct Capsule{
	Capsule(const Vector3& start, const Vector3& end, float radius);
	Vector3 pointOnSegment(float t) const;
	bool contains(const Vector3& point) const;

	LineSegment segment;
	float radius;
};

struct ConvexPolygon{
	bool contains(const Vector2& point) const;
	std::vector<Vector2> vertices;
};

bool intersect(const Sphere& a, const Sphere& b);
bool intersect(const AABB& a, const AABB& b);
bool intersect(const Capsule& a, const Capsule& b);
bool intersect(const Sphere& s, const AABB& box);

bool intersect(const LineSegment& l, const Sphere& s, float& outT);
bool intersect(const LineSegment& l, const Plane& p, float& outT);
bool intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm);

bool sweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& t);


#endif

