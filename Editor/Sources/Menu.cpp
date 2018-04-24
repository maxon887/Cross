#include "Menu.h"
#include "CrossEditor.h"
#include "SceneExplorer.h"
#include "SceneView.h"
#include "ESystem.h"

#include "ui_CrossEditor.h"

#include <QFileDialog>

Menu::Menu(QWidget* parent) :
	QMenuBar(parent)
{
	editor->UIInitialized.Connect(this, &Menu::OnUIInitialized);
}

void Menu::OnUIInitialized() {
	Ui::CrossEditorUI *ui = editor->GetUI();
	connect(ui->new_scene_action, &QAction::triggered, this, &Menu::OnNewSceneClick);
	connect(ui->save_scene_as_action, &QAction::triggered, this, &Menu::OnSaveAsClick);
	connect(ui->setup_project_directory_action, &QAction::triggered, this, &Menu::OnSetupProjectDirectoryClick);

	connect(ui->file_explorer_action, &QAction::triggered, this, &Menu::OnFileExplorerTriggered);
	ui->file_explorer->VisibilityChanged.Connect(this, &Menu::OnFileExplorerVisibilityChanged);
	connect(ui->scene_explorer_action, &QAction::triggered, this, &Menu::OnSceneExplorerTriggered);
	ui->scene_explorer->VisibilityChanged.Connect(this, &Menu::OnSceneExplorerVisibilityChanged);
	connect(ui->properties_action, &QAction::triggered, this, &Menu::OnPropertiesViewTriggered);
	ui->properties_view->VisibilityChanged.Connect(this, &Menu::OnPropertiesViewVisibilityChanged);
}

void Menu::OnNewSceneClick() {
	editor->GetSceneExplorer()->reset();
	editor->setWindowTitle(QString("Cross Editor - Untitled*"));
	editor->SetScreen(new SceneView());
}

void Menu::OnSaveAsClick() {
	QString path = QDir::currentPath() + "/" + QString(system->AssetsPath()) + game->GetCurrentScene()->GetName();
	QString filePath = QFileDialog::getSaveFileName(this, "Save Scene", path, "Scene File (*.scn)");
	if(filePath != "") {
		QFileInfo file(filePath);
		setWindowTitle(QString("Cross Editor - ") + QString(file.baseName()));
		game->GetCurrentScene()->SetName(file.baseName().toStdString().c_str());
		game->GetCurrentScene()->Save(filePath.toLatin1().data());
	}
}

void Menu::OnSetupProjectDirectoryClick() {
	QString path = QFileDialog::getExistingDirectory(this, "Setup Project Directory", QDir::currentPath());
	if(path != "") {
		path += "/";
		ESystem* esystem = dynamic_cast<ESystem*>(system);
		esystem->SetAssetPath(path.toLatin1().data());
		editor->GetFileExplorer()->SetupProjectDirectory(path);
	}
}

void Menu::OnFileExplorerTriggered(bool visibility) {
	editor->GetUI()->file_explorer_dock->setVisible(visibility);
}

void Menu::OnFileExplorerVisibilityChanged(bool visibility) {
	editor->GetUI()->file_explorer_action->setChecked(visibility);
}

void Menu::OnSceneExplorerTriggered(bool visibility) {
	editor->GetUI()->scene_explorer_dock->setVisible(visibility);
}

void Menu::OnSceneExplorerVisibilityChanged(bool visibility) {
	editor->GetUI()->scene_explorer_action->setChecked(visibility);
}

void Menu::OnPropertiesViewTriggered(bool visibility) {
	editor->GetUI()->properties_view->setVisible(visibility);
}

void Menu::OnPropertiesViewVisibilityChanged(bool visibility) {
	editor->GetUI()->properties_action->setChecked(visibility);
}