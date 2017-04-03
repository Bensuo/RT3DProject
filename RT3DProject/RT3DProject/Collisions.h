#pragma once
#include "AABB.h"
#include "Terrain.h"
#include "Player.h"
#include "Ray.h"
#include <algorithm>

namespace Collisions
{	
	struct CollisionInfo
	{
		bool collision = false;
		glm::vec3 mtv = glm::vec3(0);
		
	};

	inline void testAxis(const glm::vec3& axis, const float& minA, const float& maxA, const float& minB, const float& maxB, glm::vec3& mtvAxis, float& mtvDistance)
	{
		auto axisLengthSquared = glm::dot(axis, axis);
		//Calculates overlap ranges and find out if the overlap is on the left or right
		auto d0 = (maxB - minA);
		auto d1 = (maxA - minB);
		auto overlap = (d0 < d1) ? d0 : -d1;

		auto sep = axis * (overlap / axisLengthSquared);

		auto sepLengthSquared = dot(sep, sep);
		if (sepLengthSquared < mtvDistance)
		{
			mtvDistance = sepLengthSquared;
			mtvAxis = sep;
		}
	}

	inline glm::vec3 getMTV(const AABB& A, const AABB& B)
	{
		auto mtv = glm::vec3(0);
		auto mtvDistance = std::numeric_limits<float>::max();
		auto minA(A.c - A.r);
		auto maxA(A.c + A.r);
		auto minB(B.c - B.r);
		auto maxB(B.c + B.r);

		//Test the X, Y and Z axis to find the MTV
		testAxis(glm::vec3(1, 0, 0), minA.x, maxA.x, minB.x, maxB.x, mtv, mtvDistance);
		testAxis(glm::vec3(0, 1, 0), minA.y, maxA.y, minB.y, maxB.y, mtv, mtvDistance);
		testAxis(glm::vec3(0, 0, 1), minA.z, maxA.z, minB.z, maxB.z, mtv, mtvDistance);

		if (mtvDistance > 0)
		{
			mtv = normalize(mtv);
			mtvDistance = static_cast<float>(sqrt(mtvDistance)) * 1.001f;
			mtv *= mtvDistance;
		}

		return mtv;

	}

	inline CollisionInfo TestAABBAABB(const AABB& A, const AABB& B)
	{
		CollisionInfo info;
		if (abs(A.c[0] - B.c[0]) > (A.r[0] + B.r[0]))
		{
			info.collision = false;
			return info;
		}
		if (abs(A.c[1] - B.c[1]) > (A.r[1] + B.r[1]))
		{
			info.collision = false;
			return info;
		}
		if (abs(A.c[2] - B.c[2]) > (A.r[2] + B.r[2]))
		{
			info.collision = false;
			return info;
		}
		info.collision = true;
		info.mtv = getMTV(A, B);
		return info;
	}

	inline bool TestRayAABB(const Ray& ray, const AABB& aabb)
	{
		glm::vec3 AABBmin(aabb.c.x - aabb.r.x, aabb.c.y - aabb.r.y, aabb.c.z - aabb.r.z);
		glm::vec3 AABBmax(aabb.c.x + aabb.r.x, aabb.c.y + aabb.r.y, aabb.c.z + aabb.r.z);

		auto t1 = (AABBmin.x - ray.origin.x) / ray.dir.x;
		auto t2 = (AABBmax.x - ray.origin.x) / ray.dir.x;
		auto t3 = (AABBmin.y - ray.origin.y) / ray.dir.y;
		auto t4 = (AABBmax.y - ray.origin.y) / ray.dir.y;
		auto t5 = (AABBmin.z - ray.origin.z) / ray.dir.z;
		auto t6 = (AABBmax.z - ray.origin.z) / ray.dir.z;

		auto tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		auto tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

		// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
		if (tmax < 0)
		{		
			return false;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax)
		{		
			return false;
		}
		
		return true;
	}

	inline void terrainCollision(Player * p, const Terrain * terrain)
	{
		auto h = terrain->getHeightAtPosition(p->getPosition().x, p->getPosition().z);
		if (p->getPosition().y - p->getAABB().r.y < h)
		{
			auto newPos = p->getPosition();
			newPos.y = h + p->getAABB().r.y;
			p->setPosition(newPos);
			p->land();
		}
	}

	inline void terrainCollision(Pickup * p, const Terrain * terrain)
	{
		auto h = terrain->getHeightAtPosition(p->getPosition().x, p->getPosition().z);
		if (p->getPosition().y - p->getAABB().r.y < h)
		{
			auto newPos = p->getPosition();
			newPos.y = h + p->getAABB().r.y;
			p->setPosition(newPos);
		}
	}
}