#include "../include/Collision.h"

LineSegment::LineSegment(const glm::vec3 &start, const glm::vec3 &end) {
    this->start = start;
    this->end = end;
}

glm::vec3 LineSegment::pointOnSegment(float t) const {

    return glm::vec3(start) + (end - start) * t;
}

float LineSegment::minDistSq(const LineSegment &s1, const LineSegment &s2) {
    glm::vec3 u = s1.end - s1.start;
    glm::vec3 v = s2.end - s2.start;
    glm::vec3 w = s1.start - s2.start;

    float a = glm::dot(u,u);
    float b = glm::dot(u,v);
    float c = glm::dot(v,v);
    float d = glm::dot(u,w);
    float e = glm::dot(v,w);

    float D = a*c-b*b;
    float sc,sN,sD = D;
    float tc,tN,tD = D;

    if (nearZero(D)) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else {                 // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    }
    else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d + b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (nearZero(sN) ? 0.0f : sN / sD);
    tc = (nearZero(tN) ? 0.0f : tN / tD);

    // get the difference of the two closest points
    glm::vec3   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)
    return dP.length();   // return the closest distance squared
}

float LineSegment::minDistSq(const glm::vec3 &point) const {
    glm::vec3 ab = end - start;
    glm::vec3 ba = -1.0f * ab;
    glm::vec3 ac = point - start;
    glm::vec3 bc = point - end;

    // Case 1: C projects prior to A
    if (glm::dot(ab, ac) < 0.0f)
    {
        return ac.length();
    }
        // Case 2: C projects after B
    else if (glm::dot(ba, bc) < 0.0f)
    {
        return bc.length();
    }
        // Case 3: C projects onto line
    else
    {
        // Compute p
        float scalar = glm::dot(ac, ab)
                       / glm::dot(ab, ab);
        glm::vec3 p = scalar * ab;
        // Compute length squared of ac - p
        return (ac - p).length();
    }
}

Plane::Plane(const glm::vec3 &normal, float d) {

}

Plane::Plane(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) {

}

float Plane::signedDist(const glm::vec3 &point) const {
    return 0;
}

Sphere::Sphere(const glm::vec3 &center, float radius) {

}

bool Sphere::contains(const glm::vec3 &point) const {
    return false;
}

AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) {

}

void AABB::updateMinMax(const glm::vec3 &point) {

}

void AABB::rotate(const glm::quat &q) {

}

bool AABB::contains(const glm::vec3 &point) const {
    return false;
}

float AABB::minDistSq(const glm::vec3 &point) const {
    return 0;
}

Capsule::Capsule(const glm::vec3 &start, const glm::vec3 &end, float radius) {

}

glm::vec3 Capsule::pointOnSegment(float t) const {
    return glm::vec3();
}

bool Capsule::contains(const glm::vec3 &point) const {
    return false;
}

bool ConvexPolygon::contains(const glm::vec2 &point) const {
    return false;
}
