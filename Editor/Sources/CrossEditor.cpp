#include "CrossEditor.h"
#include "SceneView.h"
#include "System.h"
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
	connect(ui.actionNewScene, &QAction::triggered, this, &CrossEditor::OnNewSceneClick);
	connect(ui.actionSaveSceneAs, &QAction::triggered, this, &CrossEditor::OnSaveAsClick);
	connect(ui.actionSetupProjectDirectory, &QAction::triggered, this, &CrossEditor::OnSetupProjectDirectoryClick);
	connect(ui.actionFileExplorer, &QAction::triggered, this, &CrossEditor::OnFileExplorerClick);
	connect(ui.actionSceneExplorer, &QAction::triggered, this, &CrossEditor::OnSceneExplorerClick);
	
	this->ScreenChanged.Connect(this, &CrossEditor::OnScreenChanged);
	ui.sceneExplorerTree->EntitySelected.Connect(ui.propertiesView, &PropertiesView::OnEntitySelected);
	ui.sceneExplorerTree->EntityChanged.Connect(ui.propertiesView, &PropertiesView::OnEntityChanged);

	setWindowTitle(QString("Cross Editor - Untitled*"));
}

CrossEditor::~CrossEditor()
{ }

Screen* CrossEditor::GetStartScreen(){
	return new SceneView();
}

void CrossEditor::Update(float sec){
	ui.propertiesView->Update(sec);
}

void CrossEditor::closeEvent(QCloseEvent* eve){
	ui.glHandler->ShutDown();
	ui.sceneExplorerTree->reset();
	QSettings settings("CrossEditor");
	settings.setValue("geometry", QVariant(geometry()));
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(eve);
}

void CrossEditor::LoadScene(QString& scenePath){
	try{
		scene_file = scenePath;
		ui.sceneExplorerTree->clearSelection();
		SetScene(new SceneView(), scenePath.toStdString());
		QFile file(scenePath);
		setWindowTitle(QString("Cross Editor - ") + file.fileName());
	}catch(Exception exc){
		string msg = string(exc.message) +
			+"\nFile: " + string(exc.filename) +
			+"\nLine: " + to_string(exc.line);
		ExceptionMsgBox(msg.c_str());
	}
}

void CrossEditor::RestoreSettings(){
	QSettings settings("CrossEditor");
	if(settings.contains("geometry")){
		setGeometry(settings.value("geometry").value<QRect>());
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

void CrossEditor::OnScreenChanged(Screen* screen){
	SceneView* sv = dynamic_cast<SceneView*>(screen);
	if(sv){
		static S32 grabFocusDel = -1;
		static S32 entitySelectedDel = -1;
		ui.sceneExplorerTree->EntityGrabFocus.Disconnect(grabFocusDel);
		ui.sceneExplorerTree->EntitySelected.Disconnect(entitySelectedDel);
		grabFocusDel = ui.sceneExplorerTree->EntityGrabFocus.Connect(sv, &SceneView::OnEntityGrabFocus);
		entitySelectedDel = ui.sceneExplorerTree->EntitySelected.Connect(sv, &SceneView::OnEntitySelected);
	}
}

void CrossEditor::OnNewSceneClick(){
	ui.sceneExplorerTree->reset();
	scene_file = "Untitled";
	setWindowTitle(QString("Cross Editor - Untitled*"));
	SetScene(new SceneView());
}

void CrossEditor::OnSaveAsClick(){
	QString path = QDir::currentPath() + "/" + QString(sys->AssetsPath().c_str()) + scene_file;
	QString filePath = QFileDialog::getSaveFileName(this, "Save Scene", path, "Scene File (*.scn)");
	QFileInfo file(filePath);
	if(filePath != ""){
		setWindowTitle(QString("Cross Editor - ") + QString(file.baseName()));
		game->GetCurrentScene()->SetName(file.baseName().toStdString().c_str());
		game->GetCurrentScene()->Save(filePath.toStdString());
	}
}

void CrossEditor::OnSetupProjectDirectoryClick(){
	QString path = QFileDialog::getExistingDirectory(this, "Setup Project Directory", QDir::currentPath());
	path += "/";
	WINSystem* winSys = dynamic_cast<WINSystem*>(sys);
	if(path != ""){
		if(winSys){
			winSys->SetAssetPath(path.toStdString());
			ui.fileExplorerTree->SetupProjectDirectory(path);
		}else{
			throw CrossException("You are not under Windows opertating system");
		}
	}
}

void CrossEditor::OnFileExplorerClick(){
	ui.fileExplorer->show();
}

void CrossEditor::OnSceneExplorerClick(){
	ui.sceneExplorer->show();
}

void CrossEditor::keyPressEvent(QKeyEvent* key) {
	Key k = (Key)key->nativeVirtualKey();
	if(k < Key::MAX_KEY_NUM) {
		input->KeyPressed(k);
	}
}

void CrossEditor::keyReleaseEvent(QKeyEvent* key) {
	Key k = (Key)key->nativeVirtualKey();
	if(k < Key::MAX_KEY_NUM) {
		input->KeyReleased(k);
	}
}