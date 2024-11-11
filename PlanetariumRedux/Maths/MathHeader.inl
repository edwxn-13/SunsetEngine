
//INLINE FILE WOOOOOO


namespace SunsetMath 
{
	inline float Lerp(float a, float target, float rate)
	{
		return (1 - rate) * a + rate * target;
	}
};
