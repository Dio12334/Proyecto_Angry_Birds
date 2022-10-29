#ifndef COLLISION_H
#define COLLISION_H


#include <cmath>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <glm/glm.hpp>

struct LineSegment{
	LineSegment(const glm::vec3& start, const glm::vec3& end);
	glm::vec3 pointOnSegment(float t) const;
	float minDistSq(const glm::vec3& point) const;
	static float minDistSq(const LineSegment& s1, const LineSegment& s2);

	glm::vec3 start;
	glm::vec3 end;
};

struct Plane{
	Plane(const glm::vec3& normal, float d);
	Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
	float signedDist(const glm::vec3& point) const;

	glm::vec3 normal;
	float d;
};

struct Sphere{
	Sphere(const glm::vec3& center, float radius);
	bool contains(const glm::vec3& point) const;

	glm::vec3 center;
	float radius;
};

struct AABB{
	AABB(const glm::vec3& min, const glm::vec3& max);
	void updateMinMax(const glm::vec3& point);
	void rotate(const glm::quat& q);
	bool contains(const glm::vec3& point) const;
	float minDistSq(const glm::vec3& point) const;

	glm::vec3 min;
	glm::vec3 max;
};

struct OBB{
	glm::vec3 center;
	glm::quat rotation;
	glm::vec3 extents;
};

struct Capsule{
	Capsule(const glm::vec3& start, const glm::vec3& end, float radius);
	glm::vec3 pointOnSegment(float t) const;
	bool contains(const glm::vec3& point) const;

	LineSegment segment;
	float radius;
};

struct ConvexPolygon{
	bool contains(const glm::vec2& point) const;
	std::vector<glm::vec2> vertices;
};

bool intersect(const Sphere& a, const Sphere& b);
bool intersect(const AABB& a, const AABB& b);
bool intersect(const Capsule& a, const Capsule& b);
bool intersect(const Sphere& s, const AABB& box);

bool intersect(const LineSegment& l, const Sphere& s, float& outT);
bool intersect(const LineSegment& l, const Plane& p, float& outT);
bool intersect(const LineSegment& l, const AABB& b, float& outT, glm::vec3& outNorm);

bool sweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& t);

inline bool nearZero(float a, float epsilon = 0.001f){
    return std::fabs(a) < epsilon;
}

#endif

