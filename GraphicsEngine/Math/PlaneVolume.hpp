#pragma once
#include "Plane.hpp"
#include <vector>

namespace CommonUtilities
{
	template<typename T>
	class PlaneVolume
	{
	public:
		// Default constructor: empty PlaneVolume.
		PlaneVolume();

		// Constructor taking a list of Plane that makes up the PlaneVolume.
		PlaneVolume(const std::vector<Plane<T>>& aPlaneList);

		// Add a Plane to the PlaneVolume.
		void AddPlane(const Plane<T>& aPlane);

		// Returns whether a point is inside the PlaneVolume: it is inside when the point is on the
		// plane or on the side the normal is pointing away from for all the planes in the PlaneVolume.
		bool Inside(const Vector3<T>& aPosition);

	private:
		std::vector<Plane<T>> myPlaneList;
	};

	template<typename T>
	bool CommonUtilities::PlaneVolume<T>::Inside(const Vector3<T>& aPosition)
	{
		for (Plane<T> plane : myPlaneList)
		{
			if (!plane.Inside(aPosition))
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	void CommonUtilities::PlaneVolume<T>::AddPlane(const Plane<T>& aPlane)
	{
		myPlaneList.push_back(aPlane);
	}

	template<typename T>
	CommonUtilities::PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& aPlaneList)
	{
		myPlaneList = aPlaneList;
	}

	template<typename T>
	CommonUtilities::PlaneVolume<T>::PlaneVolume()
	{
		myPlaneList.clear();
	}

}