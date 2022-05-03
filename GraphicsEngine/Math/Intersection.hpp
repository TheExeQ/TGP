#pragma once
#include "Plane.hpp"
#include "Sphere.hpp"
#include "AABB3D.hpp"
#include "Ray.hpp"
#include <math.h>

namespace CommonUtilities 
{
	// If the ray is parallel to the plane, aOutIntersectionPoint remains unchanged. If
	// the ray is in the plane, true is returned, if not, false is returned. If the ray
	// isn't parallel to the plane, the intersection point is stored in
	// aOutIntersectionPoint and true returned.
	template<typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>&
		aOutIntersectionPoint);

	// If the ray intersects the AABB, true is returned, if not, false is returned.
	// A ray in one of the AABB's sides is counted as intersecting it.
	template<typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay);

	// If the ray intersects the sphere, true is returned, if not, false is returned.
	// A rat intersecting the surface of the sphere is considered as intersecting it.
	template<typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay);
}

template<typename T>
bool CommonUtilities::IntersectionPlaneRay(const Plane<T>&aPlane, const Ray<T>&aRay, Vector3<T>&aOutIntersectionPoint)
{
	Plane<T> plane = aPlane;
	Ray<T> ray = aRay;

	T denom = plane.GetNormal().Dot(ray.GetDirection());
	Vector3<T> distance = plane.GetPosition() - ray.GetOrigin();
	T pointLength;

	if (denom == 0) 
	{ 
		return (distance.Dot(plane.GetNormal()) == 0); 
	}
	else if (std::abs(denom) > std::numeric_limits<T>::epsilon())
	{
		pointLength = distance.Dot(plane.GetNormal()) / denom;

		aOutIntersectionPoint = ray.GetOrigin() + ray.GetDirection().GetNormalized() * pointLength;

		return (pointLength >= 0);
	}

	return false;
}

template<typename T>
bool CommonUtilities::IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay)
{
	AABB3D<T> aabb = aAABB;
	Ray<T> ray = aRay;
	T pointLength = ray.GetDirection().Dot(aabb.GetPosition() - ray.GetOrigin());

	Vector3<T> point = ray.GetOrigin() + ray.GetDirection().GetNormalized() * pointLength;

	return aabb.IsInside(point);
}

template<typename T>
bool CommonUtilities::IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
{
	Sphere<T> sphere = aSphere;
	Ray<T> ray = aRay;
	T pointLength = ray.GetDirection().GetNormalized().Dot(sphere.GetPosition() - ray.GetOrigin());

	Vector3<T> point = ray.GetOrigin() + ray.GetDirection().GetNormalized() * pointLength;

	return sphere.IsInside(point);
}