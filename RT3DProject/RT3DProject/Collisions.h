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

	void testAxis(glm::vec3 axis, float minA, float maxA, float minB, float maxB, glm::vec3& mtvAxis, float& mtvDistance)
	{
		float axisLengthSquared = glm::dot(axis, axis);
		//Calculates overlap ranges and find out if the overlap is on the left or right
		float d0 = (maxB - minA);
		float d1 = (maxA - minB);
		float overlap = (d0 < d1) ? d0 : -d1;

		glm::vec3 sep = axis * (overlap / axisLengthSquared);

		float sepLengthSquared = glm::dot(sep, sep);
		if (sepLengthSquared < mtvDistance)
		{
			mtvDistance = sepLengthSquared;
			mtvAxis = sep;
		}
	}

	glm::vec3 getMTV(const AABB& A, const AABB& B)
	{
		glm::vec3 mtv = glm::vec3(0);
		float mtvDistance = std::numeric_limits<float>::max();
		glm::vec3 minA(A.c - A.r);
		glm::vec3 maxA(A.c + A.r);
		glm::vec3 minB(B.c - B.r);
		glm::vec3 maxB(B.c + B.r);

		//Test the X, Y and Z axis to find the MTV
		testAxis(glm::vec3(1, 0, 0), minA.x, maxA.x, minB.x, maxB.x, mtv, mtvDistance);
		testAxis(glm::vec3(0, 1, 0), minA.y, maxA.y, minB.y, maxB.y, mtv, mtvDistance);
		testAxis(glm::vec3(0, 0, 1), minA.z, maxA.z, minB.z, maxB.z, mtv, mtvDistance);
		if (mtvDistance > 0)
		{
			mtv = glm::normalize(mtv);
			mtvDistance = (float)sqrt(mtvDistance) * 1.001f;
			mtv *= mtvDistance;
		}

		return mtv;

	}
	CollisionInfo TestAABBAABB(const AABB& A, const AABB& B)
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
	bool TestRayAABB(const Ray& ray, const AABB& aabb)
	{
		glm::vec3 AABBmin(aabb.c.x - aabb.r.x, aabb.c.y - aabb.r.y, aabb.c.z - aabb.r.z);
		glm::vec3 AABBmax(aabb.c.x + aabb.r.x, aabb.c.y + aabb.r.y, aabb.c.z + aabb.r.z);
		float t1 = (AABBmin.x - ray.origin.x) / ray.dir.x;
		float t2 = (AABBmax.x - ray.origin.x) / ray.dir.x;
		float t3 = (AABBmin.y - ray.origin.y) / ray.dir.y;
		float t4 = (AABBmax.y - ray.origin.y) / ray.dir.y;
		float t5 = (AABBmin.z - ray.origin.z) / ray.dir.z;
		float t6 = (AABBmax.z - ray.origin.z) / ray.dir.z;

		float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

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
	void terrainCollision(Player * p, Terrain * terrain)
	{
		float h = terrain->getHeightAtPosition(p->getPosition().x, p->getPosition().z);
		if (p->getPosition().y - p->getAABB().r.y < h)
		{
			glm::vec3 newPos = p->getPosition();
			newPos.y = h + p->getAABB().r.y;
			p->setPosition(newPos);
			p->Land();
		}
	}

	void terrainCollision(Pickup * p, Terrain * terrain)
	{
		float h = terrain->getHeightAtPosition(p->getPosition().x, p->getPosition().z);
		if (p->getPosition().y - p->getAABB().r.y < h)
		{
			glm::vec3 newPos = p->getPosition();
			newPos.y = h + p->getAABB().r.y;
			p->setPosition(newPos);
		}
	}
}