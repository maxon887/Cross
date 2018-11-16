#include "Cross.h"
#include "ArrayTest.h"
#include "Demo.h"
#include "UI/MenuBar.h"
#include "Utils/Debugger.h"

#include "ThirdParty/ImGui/imgui.h"

ArrayTestView::ArrayTestView() :
	View("Array Test")
{ }

void ArrayTestView::Update(float sec) {
	ImGui::Columns(4);
	ImGui::Text("Test Name");
	ImGui::NextColumn();
	ImGui::Text("Cross");
	ImGui::NextColumn();
	ImGui::Text("Std");
	ImGui::NextColumn();
	ImGui::NextColumn();
	ImGui::Separator();

	ImGui::Text("Adding Integers");
	ImGui::NextColumn();
	ImGui::Text("%f", AddingIntegersCross);
	ImGui::NextColumn();
	ImGui::Text("%f", AddingIntegersStd);
	ImGui::NextColumn();
	if(ImGui::Button("Run##1")) {
		AddingIntegersCross = AddingIntegersCrossFunc();
		AddingIntegersStd = AddingIntegersStdFunc();
	}
	ImGui::NextColumn();

	ImGui::Text("Adding Hard Objects");
	ImGui::NextColumn();
	ImGui::Text("%f", AddingHardObjectsCross);
	ImGui::NextColumn();
	ImGui::Text("%f", AddingHardObjectsStd);
	ImGui::NextColumn();
	if(ImGui::Button("Run##2")) {
		AddingHardObjectsCross = AddingHardObjectsCrossFunc();
		AddingHardObjectsStd = AddingHardObjectsStdFunc();
	}
	ImGui::NextColumn();

	ImGui::Text("Copying");
	ImGui::NextColumn();
	ImGui::Text("%f", CopyingCross);
	ImGui::NextColumn();
	ImGui::Text("%f", CopyingStd);
	ImGui::NextColumn();
	if(ImGui::Button("Run##3")) {

		Array<S32> integers;
		for(S32 i = 0; i < 500; i++) {
			integers.Add(Random(-1000000, 1000000));
		}

		Array<Hard> hards;
		for(S32 i = 0; i < 100; i++) {
			hards.Add(Hard());
		}
		CopyingCross = CopyingCrossFunc(integers, hards);

		std::vector<S32> integersStd;
		for(S32 i = 0; i < 500; i++) {
			integersStd.push_back(Random(-1000000, 1000000));
		}

		std::vector<Hard> hardsStd;
		for(S32 i = 0; i < 100; i++) {
			hardsStd.push_back(Hard());
		}
		CopyingStd = CopyingStdFunc(integersStd, hardsStd);
	}
	ImGui::NextColumn();

	ImGui::Text("Finding Object");
	ImGui::NextColumn();
	ImGui::Text("%f", FindingCross);
	ImGui::NextColumn();
	ImGui::Text("%f", FindingStd);
	ImGui::NextColumn();
	if(ImGui::Button("Run##4")) {
		Array<S32> integers;
		for(S32 i = 0; i < 500; i++) {
			integers.Add(Random(-1000000, 1000000));
		}
		FindingCross = FindingCrossFunc(integers);
		std::vector<S32> integersStd;
		for(S32 i = 0; i < 500; i++) {
			integersStd.push_back(Random(-1000000, 1000000));
		}
		FindingStd = FindingStdFunc(integersStd);
	}
	ImGui::NextColumn();

	ImGui::Text("Reallocation");
	ImGui::NextColumn();
	ImGui::Text("%f", ReallocationCross);
	ImGui::NextColumn();
	ImGui::Text("%f", ReallocationStd);
	ImGui::NextColumn();
	if(ImGui::Button("Run##5")) {
		ReallocationCross = ReallocationCrossFunc();
		ReallocationStd = ReallocationStdFunc();
	}
	ImGui::NextColumn();

	ImGui::Text("Getting Random Element");
	ImGui::NextColumn();
	ImGui::Text("%f", GettingRandomElementCross);
	ImGui::NextColumn();
	ImGui::Text("%f", GettingRandomElementStd);
	ImGui::NextColumn();
	if(ImGui::Button("Run##6")) {
		Array<S32> integers;
		for(S32 i = 0; i < 1000; i++) {
			integers.Add(Random(-1000000, 1000000));
		}
		std::vector<S32> integersStd;
		for(S32 i = 0; i < 1000; i++) {
			integersStd.push_back(Random(-1000000, 1000000));
		}
		GettingRandomElementCross = GettingRandomElementCrossFunc(integers);
		GettingRandomElementStd = GettingRandomElementStdFunc(integersStd);
	}
	ImGui::NextColumn();

	ImGui::Text("Iteration");
	ImGui::NextColumn();
	ImGui::Text("%f", IterrationCross);
	ImGui::NextColumn();
	ImGui::Text("%f", IterrationStd);
	ImGui::NextColumn();
	if(ImGui::Button("Run##7")) {
		Array<S32> integers;
		for(S32 i = 0; i < 5000000; i++) {
			integers.Add(Random(-1000000, 1000000));
		}
		std::vector<S32> integersStd;
		for(S32 i = 0; i < 5000000; i++) {
			integersStd.push_back(Random(-1000000, 1000000));
		}
		IterrationCross = IterrationCrossFunc(integers);
		IterrationStd = IterrationStdFunc(integersStd);
	}
	ImGui::NextColumn();

	ImGui::Text("Summary");
	ImGui::NextColumn();
	float summary = 0;
	summary += AddingIntegersCross;
	summary += AddingHardObjectsCross;
	summary += CopyingCross;
	summary += FindingCross;
	summary += ReallocationCross;
	summary += GettingRandomElementCross;
	summary += IterrationCross;
	ImGui::Text("%f", summary);
	ImGui::NextColumn();
	summary = 0;
	summary += AddingIntegersStd;
	summary += AddingHardObjectsStd;
	summary += CopyingStd;
	summary += FindingStd;
	summary += ReallocationStd;
	summary += GettingRandomElementStd;
	summary += IterrationStd;
	ImGui::Text("%f", summary);
	ImGui::NextColumn();
	ImGui::NextColumn();
}

float ArrayTestView::AddingIntegersCrossFunc() {
	Debugger::Instance()->SetTimeCheck();

	Array<S32> integers;
	for(S32 i = 0; i < 100000; i++) {
		integers.Add(Random(-1000000, 1000000));
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::AddingIntegersStdFunc() {
	Debugger::Instance()->SetTimeCheck();

	std::vector<S32> integers;
	for(S32 i = 0; i < 100000; i++) {
		integers.push_back(Random(-1000000, 1000000));
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::AddingHardObjectsCrossFunc() {
	Debugger::Instance()->SetTimeCheck();

	Array<Hard> integers;
	for(S32 i = 0; i < 30000; i++) {
		integers.Add(Hard());
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::AddingHardObjectsStdFunc() {
	Debugger::Instance()->SetTimeCheck();

	std::vector<Hard> integers;
	for(S32 i = 0; i < 30000; i++) {
		integers.push_back(Hard());
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::CopyingCrossFunc(const Array<S32>& integers, const Array<Hard>& hards) {
	Debugger::Instance()->SetTimeCheck();

	Array<S32> outsideInts;
	for(S32 i = 0; i < 100; i++) {
		outsideInts = integers;
	}

	Array<Hard> outsideHards;
	for(S32 i = 0; i < 100; i++) {
		outsideHards = hards;
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::CopyingStdFunc(const std::vector<S32>& integers, const std::vector<Hard>& hards) {
	Debugger::Instance()->SetTimeCheck();

	std::vector<S32> outsideInts;
	for(S32 i = 0; i < 70; i++) {
		outsideInts = integers;
	}

	std::vector<Hard> outsideHards;
	for(S32 i = 0; i < 70; i++) {
		outsideHards = hards;
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::FindingCrossFunc(const Array<S32>& integers) {
	Debugger::Instance()->SetTimeCheck();

	for(S32 i = 0; i < 10000; i++) {
		if(i % 2 == 0) {
			integers.Find(Random(-100000, 100000));
		} else {
			integers.Find(integers[Random(0, integers.Size())]);
		}
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::FindingStdFunc(const std::vector<S32>& integers) {
	Debugger::Instance()->SetTimeCheck();

	for(S32 i = 0; i < 10000; i++) {
		if(i % 2 == 0) {
			std::find(integers.begin(), integers.end(), Random(-100000, 100000));
		} else {
			std::find(integers.begin(), integers.end(), integers[Random(0, integers.size())]);
		}	
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::ReallocationCrossFunc() {
	Debugger::Instance()->SetTimeCheck();

	Array<S32> integers(1000, 0);
	while(integers.Capacity() < 100000) {
		integers.Reserve(integers.Capacity() + 10);
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::ReallocationStdFunc() {
	Debugger::Instance()->SetTimeCheck();

	std::vector<S32> integers(1000, 0);
	while(integers.capacity() < 100000) {
		integers.reserve(integers.capacity() + 10);
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::GettingRandomElementCrossFunc(const Array<S32>& integers) {
	Debugger::Instance()->SetTimeCheck();

	for(S32 i = 0; i < 150000; i++) {
		S32 index = Random(0, integers.Size());
		S32 value = integers[index];
		value++;
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::GettingRandomElementStdFunc(const std::vector<S32>& integers) {
	Debugger::Instance()->SetTimeCheck();

	for(S32 i = 0; i < 150000; i++) {
		S32 index = Random(0, integers.size());
		S32 value = integers[index];
		value++;
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::IterrationCrossFunc(const Array<S32>& integers) {
	Debugger::Instance()->SetTimeCheck();

	int res;
	for(int v : integers) {
		res += v;
	}
	printf("%f", res);

	return Debugger::Instance()->GetTimeCheck();
}

float ArrayTestView::IterrationStdFunc(const std::vector<S32>& integers) {
	Debugger::Instance()->SetTimeCheck();

	int res;
	for(int v : integers) {
		res += v;
	}
	printf("%f", res);
	return Debugger::Instance()->GetTimeCheck();
}

void ArrayTest::Start() {
	demo->GetMenuBar()->CloseAllViews();

	view = new ArrayTestView();
	view->Show();
}

void ArrayTest::Stop() {
	delete view;
}

void ArrayTest::Update(float sec) {
	view->Update(sec);
}
