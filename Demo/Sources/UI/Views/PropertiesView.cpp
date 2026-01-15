#include "PropertiesView.h"
#include "FilesView.h"
#include "Game.h"

PropertiesView::PropertiesView(FilesView* fv) :
	View("Properties", true), shaderVB(fv), materialVB(fv)
{
	game->ScreenChanged.Connect(&materialVB, &MaterialVisualBox::OnScreenChanged);
}

void PropertiesView::Update(float sec) {
	shaderVB.Update();
	materialVB.Update();
}
