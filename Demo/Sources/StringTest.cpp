#include "Cross.h"
#include "StringTest.h"
#include "Demo.h"
#include "UI/MenuBar.h"
#include "Utils/Debugger.h"

#include "ThirdParty/ImGui/imgui.h"

using namespace cross;

char* long_names[] = {
	"This is the first of long string",
	"Some useless information there",
	"Shmerch sperch the plehtsm",
	"Just enother usless text, wich no one see",
	"Some times i think about what i do, and how the World will change in the future",
	"Specially designe for the search test string, wich no one see",
	"Bring me the fucking pen" };

char* short_names[] = {
	"e",
	"Shmerch",
	"hi",
	"fuck",
	"the",
	"lol",
	"no one see" };

float StringTestView::CreateLongCrossStrings() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 10000; i++) {
		String test = long_names[Random(0, long_names_count)];
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::CreateLongStdStrings() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 10000; i++) {
		std::string test = long_names[Random(0, long_names_count)];
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::CreateShortCrossStrings() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 10000; i++) {
		String test = short_names[Random(0, short_names_count)];
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::CreateShortStdStrings() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 10000; i++) {
		std::string test = short_names[Random(0, short_names_count)];
	}

	return Debugger::Instance()->GetTimeCheck();
}

float ConversionCrossStringTest() {
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

float ConversionStdStringTest() {
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

float StringTestView::ConcatenationCrossStringTest() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 1000; i++) {
		String result;
		bool shouldGoNext = true;
		while(shouldGoNext) {
			char chose = Random(0, 10);
			switch(chose) {
			case 0:
				result += Random(0, 10000);
				break;
			case 1:
				result += Random(0.f, 100000.f);
				break;
			case 2:
				result += long_names_cross[Random(0, long_names_count)];
				break;
			case 3:
				result += long_names_cross[Random(0, short_names_count)];
				break;
			case 4:
				result += long_names[Random(0, long_names_count)];
				break;
			case 5:
				result += short_names[Random(0, short_names_count)];
				break;
			case 6:
				shouldGoNext = false;
			default:
				break;
			}
		}
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::ConcatenationStdStringTest() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 1000; i++) {
		std::string result;
		bool shouldGoNext = true;
		while(shouldGoNext) {
			char chose = Random(0, 10);
			switch(chose) {
			case 0:
				result += Random(0, 10000);
				break;
			case 1:
				result += Random(0.f, 100000.f);
				break;
			case 2:
				result += long_names_std[Random(0, long_names_count)];
				break;
			case 3:
				result += long_names_std[Random(0, short_names_count)];
				break;
			case 4:
				result += long_names[Random(0, long_names_count)];
				break;
			case 5:
				result += short_names[Random(0, short_names_count)];
				break;
			case 6:
				shouldGoNext = false;
			default:
				break;
			}
		}
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::FindCrossStringTest() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 500000; i++) {
		S32 result = long_names_cross[Random(0, long_names_count)].Find(short_names[Random(0, short_names_count + 1)]);
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::FindStdStringTest() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 500000; i++) {
		long_names_std[Random(0, long_names_count)].find(short_names[Random(0, short_names_count + 1)]);
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::ReplaceCrossStringTest() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 5000; i++) {
		String test = long_names[Random(0, long_names_count)];
		test.Replace(short_names[Random(0, short_names_count + 1)], short_names[Random(0, short_names_count + 1)]);
	}

	return Debugger::Instance()->GetTimeCheck();
}

float StringTestView::ReplaceStdStringTest() {
	Debugger::Instance()->SetTimeCheck();

	for(int i = 0; i < 5000; i++) {
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

StringTestView::StringTestView() :
	View("String Test")
{
	for(int i = 0; i < long_names_count; i++) {
		long_names_cross[i] = String(long_names[i]);
		long_names_std[i] = std::string(long_names[i]);
	}
}

void StringTestView::Content(float sec) {
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
	ImGui::Text("%f", CreateLongCrossStringsResult);
	ImGui::NextColumn();
	ImGui::Text("%f", CreateLongStdStringsResult);
	ImGui::NextColumn();
	if(ImGui::Button("Run##1")) {
		CreateLongCrossStringsResult = CreateLongCrossStrings();
		CreateLongStdStringsResult = CreateLongStdStrings();
	}
	ImGui::NextColumn();

	ImGui::Text("Short String Creation");
	ImGui::NextColumn();
	ImGui::Text("%f", CreateShortCrossStringsResult);
	ImGui::NextColumn();
	ImGui::Text("%f", CreateShortStdStringsResult);
	ImGui::NextColumn();
	if(ImGui::Button("Run##2")) {
		CreateShortCrossStringsResult = CreateShortCrossStrings();
		CreateShortStdStringsResult = CreateShortStdStrings();
	}
	ImGui::NextColumn();

	ImGui::Text("Primitive Types Conversion");
	ImGui::NextColumn();
	ImGui::Text("%f", ConversionCrossStringResult);
	ImGui::NextColumn();
	ImGui::Text("%f", ConversionStdStringResult);
	ImGui::NextColumn();
	if(ImGui::Button("Run##3")) {
		ConversionCrossStringResult = ConversionCrossStringTest();
		ConversionStdStringResult = ConversionStdStringTest();
	}
	ImGui::NextColumn();

	ImGui::Text("Concatination");
	ImGui::NextColumn();
	ImGui::Text("%f", ConcatenationCrossStringResult);
	ImGui::NextColumn();
	ImGui::Text("%f", ConcatenationStdStringResult);
	ImGui::NextColumn();
	if(ImGui::Button("Run##4")) {
		ConcatenationCrossStringResult = ConcatenationCrossStringTest();
		ConcatenationStdStringResult = ConcatenationStdStringTest();
	}
	ImGui::NextColumn();

	ImGui::Text("Find SubString");
	ImGui::NextColumn();
	ImGui::Text("%f", FindCrossStringResult);
	ImGui::NextColumn();
	ImGui::Text("%f", FindStdStringResult);
	ImGui::NextColumn();
	if(ImGui::Button("Run##5")) {
		FindCrossStringResult = FindCrossStringTest();
		FindStdStringResult = FindStdStringTest();
	}
	ImGui::NextColumn();

	ImGui::Text("Replace With SubString");
	ImGui::NextColumn();
	ImGui::Text("%f", ReplaceCrossStringResult);
	ImGui::NextColumn();
	ImGui::Text("%f", ReplaceStdStringResult);
	ImGui::NextColumn();
	if(ImGui::Button("Run##6")) {
		ReplaceCrossStringResult = ReplaceCrossStringTest();
		ReplaceStdStringResult = ReplaceStdStringTest();
	}
	ImGui::NextColumn();

	ImGui::Text("Summary");
	ImGui::NextColumn();
	float summary = 0;
	summary += CreateLongCrossStringsResult;
	summary += CreateShortCrossStringsResult;
	summary += ConversionCrossStringResult;
	summary += ConcatenationCrossStringResult;
	summary += FindCrossStringResult;
	summary += ReplaceCrossStringResult;
	ImGui::Text("%f", summary);
	ImGui::NextColumn();
	summary = 0;
	summary += CreateLongStdStringsResult;
	summary += CreateShortStdStringsResult;
	summary += ConversionStdStringResult;
	summary += ConcatenationStdStringResult;
	summary += FindStdStringResult;
	summary += ReplaceStdStringResult;
	ImGui::Text("%f", summary);
	ImGui::NextColumn();
	ImGui::NextColumn();
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
