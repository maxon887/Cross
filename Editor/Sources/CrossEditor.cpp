#include "CrossEditor.h"
#include "SceneView.h"
#include "System.h"
#include "Shaders/Shader.h"
#include "Platform/Windows/WINSystem.h"

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
	connect(ui.actionNewScene, &QAction::triggered, this, &CrossEditor::OnNewSceneClick);
	connect(ui.actionSaveSceneAs, &QAction::triggered, this, &CrossEditor::OnSaveAsClick);
	connect(ui.actionSetupProjectDirectory, &QAction::triggered, this, &CrossEditor::OnSetupProjectDirectoryClick);
	
	ui.sceneExplorerTree->EntitySelected.Connect(ui.propertiesView, &PropertiesView::OnEntitySelected);
	ui.sceneExplorerTree->EntityChanged.Connect(ui.propertiesView, &PropertiesView::OnEntityChanged);
	ui.fileExplorerTree->FileSelected.Connect(ui.propertiesView, &PropertiesView::OnFileSelected);

	SomethingChanged.Connect(this, &CrossEditor::OnSomethingChanged);
	ScreenChanged.Connect(this, &CrossEditor::OnScreenChanged);
}

void CrossEditor::Start() {
	OnNewSceneClick();
}

void CrossEditor::Update(float sec){
	ui.propertiesView->Update(sec);
}

SceneExplorer* CrossEditor::GetSceneExplorer() {
	return ui.sceneExplorerTree;
}

PropertiesView* CrossEditor::GetPropertyView() {
	return ui.propertiesView;
}

FileExplorer* CrossEditor::GetFileExplorer() {
	return ui.fileExplorerTree;
}

void CrossEditor::closeEvent(QCloseEvent* eve) {
	ui.glHandler->ShutDown();
	GetSceneExplorer()->reset();
	QSettings settings("Data/EditorConfig.ini", QSettings::IniFormat);
	settings.setValue("geometry", QVariant(geometry()));
	settings.setValue("windowState", saveState());

	WINSystem* winSys = dynamic_cast<WINSystem*>(system);
	settings.setValue("projectDirectory", winSys->AssetsPath().c_str());

	QMainWindow::closeEvent(eve);
}

void CrossEditor::LoadScene(QString& scenePath) {
	scene_file = scenePath;
	GetSceneExplorer()->clearSelection();
	SetScreen(new SceneView(scenePath.toStdString()));
}

void CrossEditor::RestoreSettings() {
	WINSystem* winSys = dynamic_cast<WINSystem*>(system);
	QSettings settings("Data/EditorConfig.ini", QSettings::IniFormat);
	QString file = settings.fileName();
	if(settings.contains("geometry")) {
		setGeometry(settings.value("geometry").value<QRect>());
	}
	if(settings.contains("projectDirectory")) {
		QString projectDirectory = settings.value("projectDirectory").toString();
		winSys->SetAssetPath(projectDirectory.toStdString().c_str());
		GetFileExplorer()->SetupProjectDirectory(projectDirectory);
	} else {
		GetFileExplorer()->SetupProjectDirectory(QDir::currentPath() + "/" + QString(system->AssetsPath().c_str()));
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

void CrossEditor::AdjustSize(QWidget* widget) {
	QSize maxSize = widget->maximumSize() * system->GetScreenScale();
	if(maxSize.width() < 0) maxSize.setWidth(MAXINT);
	if(maxSize.height() < 0) maxSize.setHeight(MAXINT);
	QSize minSize = widget->minimumSize() * system->GetScreenScale();
	widget->setMaximumSize(maxSize);
	widget->setMinimumSize(minSize);
}

void CrossEditor::OnNewSceneClick() {
	GetSceneExplorer()->reset();
	scene_file = "Untitled";
	setWindowTitle(QString("Cross Editor - Untitled*"));
	SetScreen(new SceneView());
}

void CrossEditor::OnSaveAsClick(){
	QString path = QDir::currentPath() + "/" + QString(system->AssetsPath().c_str()) + scene_file;
	QString filePath = QFileDialog::getSaveFileName(this, "Save Scene", path, "Scene File (*.scn)");
	if(filePath != ""){
		scene_file = filePath;
		QFileInfo file(filePath);
		setWindowTitle(QString("Cross Editor - ") + QString(file.baseName()));
		game->GetCurrentScene()->SetName(file.baseName().toStdString().c_str());
		game->GetCurrentScene()->Save(filePath.toStdString());
	}
}

void CrossEditor::OnSetupProjectDirectoryClick(){
	QString path = QFileDialog::getExistingDirectory(this, "Setup Project Directory", QDir::currentPath());
	if(path != ""){
		path += "/";
		WINSystem* winSys = dynamic_cast<WINSystem*>(system);
		CROSS_FAIL(winSys, "You are not under Windows opertating system");
		winSys->SetAssetPath(path.toStdString());
		GetFileExplorer()->SetupProjectDirectory(path);
	}
}

void CrossEditor::OnSomethingChanged(Action* action){
	setWindowTitle(QString("Cross Editor - ") + GetCurrentScene()->GetName().c_str() + QString("*"));
}

void CrossEditor::OnScreenChanged(Screen* screen){
	setWindowTitle(QString("Cross Editor - ") + screen->GetName().c_str());
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