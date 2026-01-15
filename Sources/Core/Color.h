#pragma once
#include "Cross.h"

namespace cross {

/*	Color will be stored in 4 channels with float point	precision */
class Color {
public:
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color White;
	static const Color Black;
	static const Color Yellow;
	static const Color Purple;

	/* Returns random color */
	static Color Random();

	float R, G, B, A;

	Color(float all);
	Color(float r, float g, float b);
	Color(int r, int g, int b);
	Color(float r, float g, float b, float a);
	Color(int r, int g, int b, int a);
	Color(const String& hex);

	/* Fill Color with flat data */
	void SetData(const char* data);
	/* Returns flat Color's data with sizeof 4 * float */
	float* GetData();
	/* Converts current color to String representation. Format FFFFFFFF */
	String ToString() const;

	void operator = (float all);
	void operator = (const String& hex);

	bool operator == (const Color &c) const;
	bool operator != (const Color &c) const;
};
	
}