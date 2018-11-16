#pragma once
#include "Screen.h"
#include "UI/View.h"

using namespace cross;

class StringTestView : public View {
public:
	StringTestView();

	void Update(float sec) override;

private:
	const S32 tests_count = 6;
	Array<float> cross_results;
	Array<float> std_results;

	static float CreateLongCrossStrings();
	static float CreateLongStdStrings();
	static float CreateShortCrossStrings();
	static float CreateShortStdStrings();
	static float ConversionCrossString();
	static float ConversionStdString();
	static float ConcatenationCrossString();
	static float ConcatenationStdString();
	static float FindCrossString();
	static float FindStdString();
	static float ReplaceCrossString();
	static float ReplaceStdString();
};

class StringTest : public Screen {
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	StringTestView* view = nullptr;
};