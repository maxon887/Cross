#pragma once
#include "Screen.h"
#include "UI/View.h"

#include <vector>

using namespace cross;

class Hard {
public:
	Hard() {
		data = new char[data_size];
	}
	Hard(const Hard& other) {
		data = new char[data_size];
		memcpy(data, other.data, data_size);
	}
	Hard(Hard&& other) {
		data = other.data;
		other.data = nullptr;
	}
	~Hard() {
		delete data;
	}

	void Set(const char* info) {
		strcpy(data, info);
	}

	char* Get() {
		return data;
	}

	void operator = (const Hard& other) {
		printf("Hard Operator =\n");
		memcpy(other.data, data, data_size);
	}

private:
	const int data_size = 25;
	char* data = nullptr;
};

class ArrayTestView : public View {
public:
	ArrayTestView();

	void Update(float sec) override;

private:
	float AddingIntegersCross = 0.f;
	float AddingIntegersStd = 0.f;
	float AddingIntegersCrossFunc();
	float AddingIntegersStdFunc();

	float AddingHardObjectsCross = 0.f;
	float AddingHardObjectsStd = 0.f;
	float AddingHardObjectsCrossFunc();
	float AddingHardObjectsStdFunc();

	float CopyingCross = 0.f;
	float CopyingStd = 0.f;
	float CopyingCrossFunc(const Array<S32>& integers, const Array<Hard>& hards);
	float CopyingStdFunc(const std::vector<S32>& integers, const std::vector<Hard>& hards);
	
	float FindingCross = 0.f;
	float FindingStd = 0.f;
	float FindingCrossFunc(const Array<S32>& integers);
	float FindingStdFunc(const std::vector<S32>& integers);

	float ReallocationCross = 0.f;
	float ReallocationStd = 0.f;
	float ReallocationCrossFunc();
	float ReallocationStdFunc();

	float GettingRandomElementCross = 0.f;
	float GettingRandomElementStd = 0.f;
	float GettingRandomElementCrossFunc(const Array<S32>& integers);
	float GettingRandomElementStdFunc(const std::vector<S32>& integers);

	float IterrationCross = 0.f;
	float IterrationStd = 0.f;
	float IterrationCrossFunc(const Array<S32>& integers);
	float IterrationStdFunc(const std::vector<S32>& integers);
};

class ArrayTest : public Screen {
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;
private:
	ArrayTestView* view = nullptr;
};