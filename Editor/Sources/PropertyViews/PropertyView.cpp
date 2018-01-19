#include "PropertyView.h"

void PropertyView::OnEntitySelected(Entity* e){
	if(isVisible()){
		hide();
	}
}

void PropertyView::OnFileSelected(const string& filename){
	if(isVisible()){
		hide();
	}
}