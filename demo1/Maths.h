#pragma once
class Maths
{
public:
	struct Position
	{
		float X;
		float Y;
		Position():X(0),Y(0)
		{

		}
		Position(float x, float y):X(x),Y(y)
		{

		}
		Position &operator+=(const Position &rhs)
		{
			X += rhs.X;
			Y += rhs.Y;

			return *this;
		}
	};
	static void Clamp(double &arg, double min, double max)
	{
		if (arg < min)
		{
			arg = min;
		}

		if (arg > max)
		{
			arg = max;
		}
	}
	Maths(void);
	~Maths(void);
};

