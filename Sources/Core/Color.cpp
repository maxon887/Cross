#include "Color.h"
#include "Math/All.h"
#include "Base/String.h"

#include <cstring>

using namespace cross;

const Color Color::Red(1.f, 0.f, 0.f);
const Color Color::Green(0.f, 1.f, 0.f);
const Color Color::Blue(0.f, 0.f, 1.f);
const Color Color::White(1.f, 1.f, 1.f);
const Color Color::Black(0.f, 0.f, 0.f);
const Color Color::Yellow(1.f, 1.f, 0.f);
const Color Color::Purple(1.f, 0.f, 1.f);

Color Color::Random(){
	return Color(cross::Random(0.f, 1.f), cross::Random(0.f, 1.f), cross::Random(0.f, 1.f));
}

Color::Color(float all) :
	Color(all, all, all)
{ }

Color::Color(float r, float g, float b):
	R(r),
	G(g),
	B(b),
	A(1.0f)
{ }

Color::Color(int r, int g, int b):
	R((float)r / 255.f),
	G((float)g / 255.f),
	B((float)b / 255.f),
	A(1.0f)
{ }

Color::Color(float r, float g, float b, float a) :
	R(r),
	G(g),
	B(b),
	A(a)
{ }

Color::Color(int r, int g, int b, int a) :
	R((float)r / 255.f),
	G((float)g / 255.f),
	B((float)b / 255.f),
	A((float)a / 255.f)
{ }

Color::Color(const String& hex) {
	String rStr = hex.SubString(0, 2);
	R = (float)rStr.ToInt(16) / 255.f;
	String gStr = hex.SubString(2, 4);
	G = (float)gStr.ToInt(16) / 255.f;
	String bStr = hex.SubString(4, 6);
	B = (float)bStr.ToInt(16) / 255.f;
	String aStr = hex.SubString(6, 8);
	A = (float)aStr.ToInt(16) / 255.f;
}

void Color::SetData(const char* data){
	memcpy(&R, data, sizeof(Color));
}

float* Color::GetData() {
	return &R;
}

String Color::ToString() const {
	String result = String(int(R * 255), "%02X", 3) + String(int(G * 255), "%02X", 3) + String(int(B * 255), "%02X", 3) + String(int(A * 255), "%02X", 3);
	return result;
}

void Color::operator = (float all) {
	R = all;
	G = all;
	B = all;
	A = 1.0f;
}

void Color::operator = (const String& hex) {
	String rStr = hex.SubString(0, 2);
	R = (float)rStr.ToInt(16) / 255.f;
	String gStr = hex.SubString(2, 4);
	G = (float)gStr.ToInt(16) / 255.f;
	String bStr = hex.SubString(4, 6);
	B = (float)bStr.ToInt(16) / 255.f;
	String aStr = hex.SubString(6, 8);
	A = (float)aStr.ToInt(16) / 255.f;
}

bool Color::operator == (const Color &c) const{
	return	this->R == c.R &&
			this->G == c.G &&
			this->B == c.B;
}

bool Color::operator != (const Color &c) const{
	return !((*this) == c);
}
