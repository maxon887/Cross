#include "CrossEditor.h"
#include "SceneView.h"
#include "System.h"
#include "Shaders/Shader.h"
#include "ESystem.h"

#include <QSettings.h>
#include <QFileDialog.h>
#include <QMessageBox>
#include <QKeyEvent>

CrossEditor* editor = NULL;

CrossEditor::CrossEditor(QWidget *parent) :
	QMainWindow(parent),
	Game()
{
	editor = this;
	game = editor;
	ui.setupUi(this);
	UIInitialized.Emit();
	connect(ui.new_scene_action, &QAction::triggered, this, &CrossEditor::OnNewSceneClick);
	connect(ui.save_scene_as_action, &QAction::triggered, this, &CrossEditor::OnSaveAsClick);
	connect(ui.setup_project_directory_action, &QAction::triggered, this, &CrossEditor::OnSetupProjectDirectoryClick);
	
	ui.scene_explorer->EntitySelected.Connect(ui.properties_view, &PropertiesView::OnEntitySelected);
	ui.scene_explorer->EntityChanged.Connect(ui.properties_view, &PropertiesView::OnEntityChanged);
	ui.file_explorer->FileSelected.Connect(ui.properties_view, &PropertiesView::OnFileSelected);

	SomethingChanged.Connect(this, &CrossEditor::OnSomethingChanged);
	ScreenChanged.Connect(this, &CrossEditor::OnScreenChanged);
}

void CrossEditor::Start() {
	OnNewSceneClick();
}

void CrossEditor::Update(float sec){
	ui.properties_view->Update(sec);
}

SceneExplorer* CrossEditor::GetSceneExplorer() {
	return ui.scene_explorer;
}

PropertiesView* CrossEditor::GetPropertyView() {
	return ui.properties_view;
}

FileExplorer* CrossEditor::GetFileExplorer() {
	return ui.file_explorer;
}

void CrossEditor::closeEvent(QCloseEvent* eve) {
	ui.gl_handler->ShutDown();
	QSettings settings("Data/EditorConfig.ini", QSettings::IniFormat);
	settings.setValue("geometry", QVariant(geometry()));
	settings.setValue("windowState", saveState());

	QString projectDir = GetFileExplorer()->GetProjectDirectory();
	settings.setValue("projectDirectory", projectDir);

	QMainWindow::closeEvent(eve);
}

void CrossEditor::LoadScene(QString& scenePath) {
	scene_file = scenePath;
	GetSceneExplorer()->clearSelection();
	SetScreen(new SceneView(scenePath.toLatin1().data()));
}

void CrossEditor::RestoreSettings() {
	ESystem* esystem = dynamic_cast<ESystem*>(system);
	QSettings settings("Data/EditorConfig.ini", QSettings::IniFormat);
	QString file = settings.fileName();
	if(settings.contains("geometry")) {
		setGeometry(settings.value("geometry").value<QRect>());
	}
	if(settings.contains("projectDirectory")) {
		QString projectDirectory = settings.value("projectDirectory").toString();
		esystem->SetAssetPath(projectDirectory.toStdString().c_str());
		GetFileExplorer()->SetupProjectDirectory(projectDirectory);
	} else {
		GetFileExplorer()->SetupProjectDirectory(QDir::currentPath() + "/" + QString(system->AssetsPath().ToCStr()));
	}
	restoreState(settings.value("windowState").toByteArray());
}

void CrossEditor::ExceptionMsgBox(const char* msg) {
	QMessageBox msgBox;
	msgBox.setText("Unhandled Exception");
	msgBox.setInformativeText(msg);
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
}

void CrossEditor::OnNewSceneClick() {
	GetSceneExplorer()->reset();
	scene_file = "Untitled";
	setWindowTitle(QString("Cross Editor - Untitled*"));
	SetScreen(new SceneView());
}

void CrossEditor::OnSaveAsClick(){
	QString path = QDir::currentPath() + "/" + QString(system->AssetsPath()) + scene_file;
	QString filePath = QFileDialog::getSaveFileName(this, "Save Scene", path, "Scene File (*.scn)");
	if(filePath != ""){
		scene_file = filePath;
		QFileInfo file(filePath);
		setWindowTitle(QString("Cross Editor - ") + QString(file.baseName()));
		game->GetCurrentScene()->SetName(file.baseName().toStdString().c_str());
		game->GetCurrentScene()->Save(filePath.toLatin1().data());
	}
}

void CrossEditor::OnSetupProjectDirectoryClick(){
	QString path = QFileDialog::getExistingDirectory(this, "Setup Project Directory", QDir::currentPath());
	if(path != ""){
		path += "/";
		ESystem* esystem = dynamic_cast<ESystem*>(system);
		esystem->SetAssetPath(path.toLatin1().data());
		GetFileExplorer()->SetupProjectDirectory(path);
	}
}

void CrossEditor::OnSomethingChanged(Action* action){
	setWindowTitle(QString("Cross Editor - ") + GetCurrentScene()->GetName() + QString("*"));
}

void CrossEditor::OnScreenChanged(Screen* screen){
	setWindowTitle(QString("Cross Editor - ") + screen->GetName());
}

void CrossEditor::keyPressEvent(QKeyEvent* key) {
	Key k = (Key)key->nativeVirtualKey();
	if(k < Key::MAX_KEY_NUM) {
		input->KeyPressed.Emit(k);
	}
}

void CrossEditor::keyReleaseEvent(QKeyEvent* key) {
	Key k = (Key)key->nativeVirtualKey();
	if(k < Key::MAX_KEY_NUM) {
		input->KeyReleased.Emit(k);
	}
}
