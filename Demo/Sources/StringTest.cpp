#include "Cross.h"
#include "StringTest.h"
#include "Demo.h"
#include "UI/MenuBar.h"
#include "Utils/Debugger.h"

#include "ThirdParty/ImGui/imgui.h"

using namespace cross;

const char* long_names[] = {
	"This is the first of long string",
	"Some useless information there",
	"Shmerch sperch the plehtsm",
	"Just enother usless text, wich no one see",
	"Some times i think about what i do, and how the World will change in the future",
	"Specially designe for the search test string, wich no one see",
	"Bring me the fucking pen" };
const S32 long_names_count = 7;

const char* short_names[] = {
	"e",
	"Shmerch",
	"hi",
	"fuck",
	"the",
	"lol",
	"no one see" };
const S32 short_names_count = 6;

StringTestView::StringTestView() :
	View("String Test"),
	cross_results(tests_count, 0.f),
	std_results(tests_count, 0.f)
{ }

void StringTestView::Update(float sec) {
	ImGui::Columns(4);
	ImGui::Text("Test Name");
	ImGui::NextColumn();
	ImGui::Text("Cross");
	ImGui::NextColumn();
	ImGui::Text("Std");
	ImGui::NextColumn();
	ImGui::NextColumn();
	ImGui::Separator();

	ImGui::Text("Long String Creation");
	ImGui::NextColumn();
	ImGui::Text("%f", cross_results[0]);
	ImGui::NextColumn();
	ImGui::Text("%f", std_results[0]);
	ImGui::NextColumn();
	if(ImGui::Button("Run##0")) {
		cross_results[0] = CreateLongCrossStrings();
		std_results[0] = CreateLongStdStrings();
	}
	ImGui::NextColumn();

	ImGui::Text("Short String Creation");
	ImGui::NextColumn();
	ImGui::Text("%f", cross_results[1]);
	ImGui::NextColumn();
	ImGui::Text("%f", std_results[1]);
	ImGui::NextColumn();
	if(ImGui::Button("Run##1")) {
		cross_results[1] = CreateShortCrossStrings();
		std_results[1] = CreateShortStdStrings();
	}
	ImGui::NextColumn();

	ImGui::Text("Primitive Types Conversion");
	ImGui::NextColumn();
	ImGui::Text("%f", cross_results[2]);
	ImGui::NextColumn();
	ImGui::Text("%f", std_results[2]);
	ImGui::NextColumn();
	if(ImGui::Button("Run##2")) {
		cross_results[2] = ConversionCrossString();
		std_results[2] = ConversionStdString();
	}
	ImGui::NextColumn();

	ImGui::Text("Concatination");
	ImGui::NextColumn();
	ImGui::Text("%f", cross_results[3]);
	ImGui::NextColumn();
	ImGui::Text("%f", std_results[3]);
	ImGui::NextColumn();
	if(ImGui::Button("Run##3")) {
		cross_results[3] = ConcatenationCrossString();
		std_results[3] = ConcatenationStdString();
	}
	ImGui::NextColumn();

	ImGui::Text("Find SubString");
	ImGui::NextColumn();
	ImGui::Text("%f", cross_results[4]);
	ImGui::NextColumn();
	ImGui::Text("%f", std_results[4]);
	ImGui::NextColumn();
	if(ImGui::Button("Run##4")) {
		cross_results[4] = FindCrossString();
		std_results[4] = FindStdString();
	}
	ImGui::NextColumn();

	ImGui::Text("Replace With SubString");
	ImGui::NextColumn();
	ImGui::Text("%f", cross_results[5]);
	ImGui::NextColumn();
	ImGui::Text("%f", std_results[5]);
	ImGui::NextColumn();
	if(ImGui::Button("Run##5")) {
		cross_results[5] = ReplaceCrossString();
		std_results[5] = ReplaceStdString();
	}
	ImGui::NextColumn();

	ImGui::Text("Summary");
	ImGui::NextColumn();
	float summary = 0;
	for(float testResult : cross_results) {
		summary += testResult;
	}
	ImGui::Text("%f", summary);
	ImGui::NextColumn();
	summary = 0;
	for(float testResult : std_results) {
		summary += testResult;
	}
	ImGui::Text("%f", summary);
	ImGui::NextColumn();
	ImGui::NextColumn();
}

float StringTestView::CreateLongCrossStrings() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 20000; i++) {
		String test = long_names[Random(0, long_names_count)];
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::CreateLongStdStrings() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 20000; i++) {
		std::string test = long_names[Random(0, long_names_count)];
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::CreateShortCrossStrings() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 70000; i++) {
		String test = short_names[Random(0, short_names_count)];
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::CreateShortStdStrings() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 70000; i++) {
		std::string test = short_names[Random(0, short_names_count)];
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::ConversionCrossString() {
	Debugger::Instance()->SetTimeCheck();

	String int_static = "10";
	String float_static = "10.0";

	for(int i = 0; i < 10000; i++) {
		char choice = Random(0, 5);
		switch(choice) {
		case 0: {
			String intStr = Random(0, 1000000);
			break;
		}
		case 1: {
			String floatStr = Random(0.f, 1000000.f);
			break;
		}
		case 2: {
			int realInt = int_static.ToInt();
			break;
		}
		case 3: {
			float realFloat = float_static.ToFloat();
			break;
		}
		default:
			break;

		}
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::ConversionStdString() {
	Debugger::Instance()->SetTimeCheck();

	string int_static = "10";
	string float_static = "10.0";

	for(int i = 0; i < 10000; i++) {
		char choice = Random(0, 5);
		switch(choice) {
		case 0: {
			string intStr = to_string( Random(0, 1000000) );
			break;
		}
		case 1: {
			string floatStr = to_string(Random(0.f, 1000000.f) );
			break;
		}
		case 2: {
			int realInt = std::stoi(int_static);
			break;
		}
		case 3: {
			float realFloat = std::stof(float_static);
			break;
		}
		default:
			break;

		}
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::ConcatenationCrossString() {
	Array<String> longNames;
	for(S32 i = 0; i < long_names_count; i++) {
		longNames.Add(long_names[i]);
	}
	Array<String> shortNames;
	for(S32 i = 0; i < short_names_count; i++) {
		shortNames.Add(short_names[i]);
	}

	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 4000; i++) {
		String result;
		bool shouldGoNext = true;
		while(shouldGoNext) {
			char chose = Random(0, 10);
			switch(chose) {
			case 0:
				result += longNames[Random(0, long_names_count)];
				break;
			case 1:
				result += shortNames[Random(0, short_names_count)];
				break;
			case 2:
				result += long_names[Random(0, long_names_count)];
				break;
			case 3:
				result += short_names[Random(0, short_names_count)];
				break;
			case 4:
				shouldGoNext = false;
			default:
				break;
			}
		}
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::ConcatenationStdString() {

	Array<std::string> longNames;
	for(S32 i = 0; i < long_names_count; i++) {
		longNames.Add(long_names[i]);
	}
	Array<std::string> shortNames;
	for(S32 i = 0; i < short_names_count; i++) {
		shortNames.Add(short_names[i]);
	}

	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 4000; i++) {
		std::string result;
		bool shouldGoNext = true;
		while(shouldGoNext) {
			char chose = Random(0, 10);
			switch(chose) {
			case 0:
				result += longNames[Random(0, long_names_count)];
				break;
			case 1:
				result += shortNames[Random(0, short_names_count)];
				break;
			case 2:
				result += long_names[Random(0, long_names_count)];
				break;
			case 3:
				result += short_names[Random(0, short_names_count)];
				break;
			case 4:
				shouldGoNext = false;
			default:
				break;
			}
		}
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::FindCrossString() {
	Array<String> longNames;
	for(S32 i = 0; i < long_names_count; i++) {
		longNames.Add(long_names[i]);
	}
	Array<String> shortNames;
	for(S32 i = 0; i < short_names_count +1 ; i++) {
		shortNames.Add(short_names[i]);
	}

	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 35000; i++) {
		S32 result = longNames[Random(0, long_names_count)].Find(shortNames[Random(0, short_names_count + 1)]);
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::FindStdString() {

	Array<std::string> longNames;
	for(S32 i = 0; i < long_names_count; i++) {
		longNames.Add(long_names[i]);
	}
	Array<std::string> shortNames;
	for(S32 i = 0; i < short_names_count + 1; i++) {
		shortNames.Add(short_names[i]);
	}

	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 35000; i++) {
		size_t result = longNames[Random(0, long_names_count)].find(shortNames[Random(0, short_names_count + 1)]);
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::ReplaceCrossString() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 10000; i++) {
		String test = long_names[Random(0, long_names_count)];
		test.Replace(short_names[Random(0, short_names_count + 1)], short_names[Random(0, short_names_count + 1)]);
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::ReplaceStdString() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 10000; i++) {
		std::string test = long_names[Random(0, long_names_count)];
		const char* from = short_names[Random(0, short_names_count + 1)];
		const char* to = short_names[Random(0, short_names_count + 1)];

		size_t index = 0;
		while(true) {
			/* Locate the substring to replace. */
			index = test.find(from, index);
			if(index == std::string::npos) break;

			/* Make the replacement. */
			test.erase(index, strlen(from));
			test.insert(index, to);

			/* Advance index forward so the next iteration doesn't pick it up as well. */
			index += strlen(to);
		}
	}

	return Debugger::Instance()->GetTimeCheck();
}

void StringTest::Start() {
	demo->GetMenuBar()->CloseAllViews();

	view = new StringTestView();
	view->Show();
}

void StringTest::Stop() {
	delete view;
}

void StringTest::Update(float sec) {
	view->Update(sec);
}
