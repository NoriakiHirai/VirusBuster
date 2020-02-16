#ifndef GAME_LIB_GAME_LIB_MATH_H_
#define GAME_LIB_GAME_LIB_MATH_H_

namespace Hirai
{
	class Vector2
	{
    public:
		float x, y;

        Vector2& operator+=(const Vector2& v) {
            x += v.x; y += v.y;
            return *this;
        }

        Vector2& operator*=(const Vector2& v) {
            x *= v.x; y *= v.y;
            return *this;
        }

        Vector2 operator*(float t) {
            return Vector2{ x * t, y * t };
        }
	};

	struct Vector3
	{
		float x;
		float y;
		float z;
	};
}
#endif // !GAME_LIB_GAME_LIB_MATH_H_
