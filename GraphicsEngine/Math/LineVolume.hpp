#pragma once
#include "Line.hpp"
#include <vector>

namespace CommonUtilities
{
	template<typename T>
	class LineVolume
	{
	public:
		// Default constructor: empty LineVolume.
		LineVolume();

		// Constructor taking a list of Line that makes up the LineVolume.
		LineVolume(const std::vector<Line<T>>& aLineList);

		// Add a Line to the LineVolume.
		void AddLine(const Line<T>& aLine);

		// Returns whether a point is inside the LineVolume: it is inside when the point is
		// on the line or on the side the normal is pointing away from for all the lines in
		// the LineVolume.
		bool Inside(const Vector2<T>& aPosition);

	private:
		std::vector<Line<T>> myLineList;
	};

	template<typename T>
	bool CommonUtilities::LineVolume<T>::Inside(const Vector2<T>& aPosition)
	{
		for (Line<T> line : myLineList)
		{
			if (!line.Inside(aPosition))
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	void CommonUtilities::LineVolume<T>::AddLine(const Line<T>& aLine)
	{
		myLineList.push_back(aLine);
	}

	template<typename T>
	CommonUtilities::LineVolume<T>::LineVolume(const std::vector<Line<T>>& aLineList)
	{
		myLineList = aLineList;
	}

	template<typename T>
	CommonUtilities::LineVolume<T>::LineVolume()
	{
		myLineList.clear();
	}

}