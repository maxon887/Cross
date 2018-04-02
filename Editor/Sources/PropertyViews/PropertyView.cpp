#include "PropertyView.h"

void PropertyView::OnEntitySelected(Entity* e){
	if(isVisible()){
		hide();
	}
}

void PropertyView::OnFileSelected(const String& filename){
	if(isVisible()){
		hide();
	}
}