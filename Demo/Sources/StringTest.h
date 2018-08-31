#pragma once
#include "Screen.h"
#include "UI/View.h"

using namespace cross;

class StringTestView : public View {
public:
	StringTestView();

	void Content(float sec) override;

private:
	static const S32 long_names_count = 7;
	static const S32 short_names_count = 6;

	float CreateLongCrossStringsResult = 0;
	float CreateLongStdStringsResult = 0;
	float CreateShortCrossStringsResult = 0;
	float CreateShortStdStringsResult = 0;
	float ConversionCrossStringResult = 0;
	float ConversionStdStringResult = 0;
	float ConcatenationCrossStringResult = 0;
	float ConcatenationStdStringResult = 0;
	float FindCrossStringResult = 0;
	float FindStdStringResult = 0;
	float ReplaceCrossStringResult = 0;
	float ReplaceStdStringResult = 0;
	String long_names_cross[long_names_count];
	std::string long_names_std[long_names_count];

	float CreateLongCrossStrings();
	float CreateLongStdStrings();
	float CreateShortCrossStrings();
	float CreateShortStdStrings();
	float ConcatenationCrossStringTest();
	float ConcatenationStdStringTest();
	float FindCrossStringTest();
	float FindStdStringTest();
	float ReplaceCrossStringTest();
	float ReplaceStdStringTest();
};

class StringTest : public Screen {
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	StringTestView* view = nullptr;
};