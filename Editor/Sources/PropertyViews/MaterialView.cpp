#include "MaterialView.h"
#include "File.h"
#include "System.h"

#include "Libs/TinyXML2/tinyxml2.h"
using namespace cross;
using namespace tinyxml2;

void MaterialView::Initialize(){

}

void MaterialView::OnFileSelected(const string& filepath){
	if(File::ExtensionFromFile(filepath) != "mat") {
		PropertyView::OnFileSelected(filepath);
		return;
	}



	show();
}