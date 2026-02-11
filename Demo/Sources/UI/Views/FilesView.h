#pragma once
#include "Base/Array.h"
#include "Base/Event.h"
#include "UI/Views/View.h"

class FilesView : public View {
public:
	Event<const String&> FileSelected;
	Event<> FileTreeChanged;

	FilesView();

	void Shown() override;
	void Hidden() override;
	void Update(float sec) override;

	bool AvailableInMenu() override;

	DockPosition GetDefaultDockPosition() const override { return DockPosition::LEFT; }
	
	void AskToShowFile(const String& filename);

private:
	struct Node {
		String name = "";
		String path = "";
		bool initialized = false;
		bool force_open = false;
		//first string is filename second full path + filename
		Array<std::pair<String, String> > files;
		Array<Node> folders;
	};

	Node file_tree;
	String current_path;

	bool editing = false;
	bool clicked = false;
	bool should_refresh = false;

	void InitNode(Node& node);
	void Refresh();
	void BuildNote(Node& node);
	void FileDoubleClicked(const String& filename);
	void ForceOpenPath(String& leftoverPath, Node& currentNode);
	void Editing();

	void ContextMenu();
	
	void OnScreenChanged(Screen* screen);
	void OnResume();
};
