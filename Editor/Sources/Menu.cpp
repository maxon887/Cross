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
	Ui::CrossEditorUI *ui = editor->GetUI();
	connect(ui->new_scene_action, &QAction::triggered, this, &Menu::OnNewSceneClick);
	connect(ui->save_scene_as_action, &QAction::triggered, this, &Menu::OnSaveAsClick);
	connect(ui->setup_project_directory_action, &QAction::triggered, this, &Menu::OnSetupProjectDirectoryClick);
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