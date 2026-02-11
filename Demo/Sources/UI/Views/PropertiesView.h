#pragma once
#include "Cross.h"
#include "View.h"
#include "UI/ShaderVisualBox.h"
#include "UI/MaterialVisualBox.h"

class FilesView;

class PropertiesView : public View {
public:
	PropertiesView(FilesView* fv);
	~PropertiesView() override;

	void Update(float sec) override;

	DockPosition GetDefaultDockPosition() const override { return DockPosition::RIGHT; }

private:
	ShaderVisualBox shaderVB;
	MaterialVisualBox materialVB;
};
