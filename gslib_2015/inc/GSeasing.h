/*
* Robert Penner's easing functions.
*
* See the following to learn more about these famous functions:
* http://www.robertpenner.com/easing/
*
* License:
* http://www.robertpenner.com/easing_terms_of_use.html
*/
#ifndef GS_EASING_H_
#define GS_EASING_H_

#include <cmath>

inline float gsEasingInBack(float t, float s = 1.70158f)
{
	return t * t * ((s + 1) * t - s);
}

inline float gsEasingInBounce(float t)
{
	t = 1.0f - t;
	if (t < 1 / 2.75f) {
		return 1.0f - (7.5625f * t * t);
	}
	if (t < 2.0f / 2.75f) {
		t -= 1.5f / 2.75f;
		return 1.0f - (7.5625f * t * t + 0.75f);
	}
	if (t < 2.5f / 2.75f) {
		t -= 2.25f / 2.75f;
		return 1.0f - (7.5625f * t * t + 0.9375f);
	}
	t -= 2.625f / 2.75f;
	return 1.0f - (7.5625f * t * t + 0.984375f);
}

inline float gsEasingInCirc(float t)
{
	return -(std::sqrt(1.0f - t * t) - 1.0f);
}

inline float gsEasingInCubic(float t)
{
	return t * t * t;
}

inline float gsEasingInElastic(float t, float s = 1.70158f)
{
	float p = 0.3f;
	float a = 1.0f;
	if (t == 0.0f) {
		return 0.0f;
	}
	if (t == 1.0f) {
		return 1.0f;
	}
	if (a < 1.0f) {
		a = 1.0f;
		s = p / 4.0f;
	} else {
		s = p / (2.0f * 3.14159265359f) * std::asin(1.0f / a);
	}
	t -= 1.0f;
	return -(a * std::pow(2.0f, 10.0f * t) * std::sin((t - s) * (2.0f * 3.14159265359f) / p));
}

inline float gsEasingInExpo(float t)
{
	return (t == 0) ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f));
}

inline float gsEasingInQuad(float t)
{
	return t * t;
}

inline float gsEasingInQuart(float t)
{
	return t * t * t * t;
}

inline float gsEasingInQuint(float t)
{
	return t * t * t * t * t;
}

inline float gsEasingInSine(float t)
{
	return -std::cos(t * (3.14159265359f/ 2.0f)) + 1.0f;
}

inline float gsEasingOutBack(float t, float s = 1.70158f)
{
	t -= 1.0f;
	return t * t * ((s + 1.0f) * t + s) + 1.0f;
}

inline float gsEasingOutBounce(float t)
{
	if (t < 1.0f / 2.75f) {
		return 7.5625f * t * t;
	}
	if (t < 2.0f / 2.75f) {
		t -= 1.5f / 2.75f;
		return 7.5625f * t * t + 0.75f;
	}
	if (t < 2.5f / 2.75f) {
		t -= 2.25f / 2.75f;
		return 7.5625f * t * t + 0.9375f;
	}
	t -= 2.625f / 2.75f;
	return 7.5625 * t * t + 0.984375;
}

inline float gsEasingOutCirc(float t)
{
	t -= 1.0f;
	return std::sqrt(1.0f - t * t);
}

inline float gsEasingOutCubic(float t)
{
	t -= 1.0f;
	return t * t * t + 1.0f;
}

inline float gsEasingOutElastic(float t, float s = 1.70158f)
{
	float p = 0.3f;
	float a = 1.0f;
	if (t == 0.0f) {
		return 0.0f;
	}
	if (t == 1.0f) {
		return 1.0f;
	}
	if (a < 1.0f) {
		a = 1.0f;
		s = p / 4.0f;
	} else {
		s = p / (2.0f * 3.14159265359f) * std::asin(1.0f / a);
	}
	return a * std::pow(2.0f, -10.0f * t) * sin((t - s) * (2.0f * 3.14159265359f) / p) + 1.0f;
}

inline float gsEasingOutExpo(float t)
{
	return (t == 1.0f) ? 1.0f : (-std::pow(2.0f, -10.0f * t) + 1.0f);
}

inline float gsEasingOutQuad(float t)
{
	return -t * (t - 2.0f);
}

inline float gsEasingOutQuart(float t)
{
	t -= 1.0f;
	return 1.0 - t * t * t * t;
}

inline float gsEasingOutQuint(float t)
{
	t -= 1.0f;
	return t * t * t * t * t + 1.0f;
}

inline float gsEasingOutSine(float t)
{
	return std::sin(t * (3.14159265359f / 2.0f));
}

inline float gsEasingInOutBack(float t, float s = 1.70158f)
{
	float k = 1.525f;
	t *= 2.0f;
	s *= k;
	if (t < 1) {
		return 0.5 * (t * t * ((s + 1) * t - s));
	}
	t -= 2;
	return 0.5 * (t * t * ((s + 1) * t + s) + 2);
}

inline float gsEasingInOutBounce(float t)
{
	return (t < 0.5f)
		? (gsEasingInBounce(t * 2.0f) * 0.5f)
		: (gsEasingOutBounce(t * 2.0f - 1.0f) * 0.5f + 0.5f);
}

inline float gsEasingInOutCirc(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return -0.5f * (std::sqrt(1.0f - t * t) - 1.0f);
	}
	t -= 2.0f;
	return 0.5f * (sqrt(1 - t * t) + 1);
}

inline float gsEasingInOutCubic(float t)
{
	t *= 2.0f;
	if (t < 1) {
		return 0.5 * t * t * t;
	}
	t -= 2.0f;
	return 0.5f * (t * t * t + 2.0f);
}

inline float gsEasingInOutElastic(float t, float s = 1.70158f)
{
	float p = 0.3f * 1.5f;
	float a = 1.0f;
	if (t == 0.0f) {
		return 0.0f;
	}
	if (t == 1.0f) {
		return 1.0f;
	}
	if (a < 1.0f) {
		a = 1.0f;
		s = p / 4.0f;
	} else {
		s = p / (2.0f * 3.14159265359f) * std::asin(1.0f / a);
	}
	if (t < 1.0f) {
		t -= 1.0f;
		return -0.5f * (a * std::pow(2.0f, 10.0f * t) * std::sin((t - s) * (2.0f * 3.14159265359f) / p));
	}
	t -= 1.0f;
	return a * std::pow(2.0f, -10.0f * t) * std::sin((t - s) * (2.0f * 3.14159265359f) / p) * 0.5f + 1.0f;
}

inline float gsEasingInOutExpo(float t)
{
	if (t == 0.0f) {
		return 0.0f;
	}
	if (t == 1.0f) {
		return 1.0f;
	}
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * std::pow(2.0f, 10.0f * (t - 1.0f));
	}
	t -= 1.0f;
	return 0.5f * (-std::pow(2.0f, -10.0f * t) + 2.0f);
}

inline float gsEasingInOutQuad(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t * t;
	}
	t -= 1.0f;
	return -0.5f * (t * (t - 2.0f) - 1.0f);
}

inline float gsEasingInOutQuart(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t * t * t * t;
	}
	t -= 2.0f;
	return -0.5f * (t * t * t * t - 2.0f);
}

inline float gsEasingInOutQuint(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t * t * t * t * t;
	}
	t -= 2.0f;
	return 0.5f * (t * t * t * t * t + 2.0f);
}

inline float gsEasingInOutSine(float t)
{
	return -0.5f * (std::cos(3.14159265359f * t) - 1.0f);
}

#endif

// end of file