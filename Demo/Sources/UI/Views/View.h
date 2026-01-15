#pragma once
#include "Cross.h"
#include "Base/String.h"

using namespace cross;

class View {
public:
	enum DockPosition {
		LEFT,
		RIGHT,
		NONE
	};

	View(const String& name, bool defaultVisibility = false);
	virtual ~View();

	virtual void Shown() { }
	virtual void Hidden() { }

	virtual void PreUpdate() { }
	virtual void Update(float sec) { }
	virtual void PostUpdate() { }

	virtual bool VisibleInMenu() { return true; }
	virtual bool AvailableInMenu() { return true; }

	virtual DockPosition GetDefaultDockPosition() const { return DockPosition::NONE; }

	void Show();
	void Hide();
	bool IsVisible() const;
	const String& GetName() const;

	void Run(float sec);

protected:
	void SetFlags(U32 flags);

private:
	String landscape_name = "View##Landscape";
	String portrait_name = "View##Portrait";
	U32 flags = 0;
	bool visible = false;
};
