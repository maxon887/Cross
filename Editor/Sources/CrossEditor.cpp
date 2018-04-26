#include "CrossEditor.h"
#include "SceneView.h"
#include "System.h"
#include "Shaders/Shader.h"
#include "ESystem.h"
#include "Menu.h"

#include "ui_CrossEditor.h"

#include <QSettings>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDir>

CrossEditor* editor = nullptr;

CrossEditor::CrossEditor(QWidget *parent) :
	QMainWindow(parent),
	Game()
{
	editor = this;
	game = editor;
	ui = new Ui::CrossEditorUI();
	ui->setupUi(this);
	UIInitialized.Emit();
	
	ui->scene_explorer->EntitySelected.Connect(ui->properties_view, &PropertiesView::OnEntitySelected);
	ui->scene_explorer->EntityChanged.Connect(ui->properties_view, &PropertiesView::OnEntityChanged);
	ui->file_explorer->FileSelected.Connect(ui->properties_view, &PropertiesView::OnFileSelected);

	SomethingChanged.Connect(this, &CrossEditor::OnSomethingChanged);
	ScreenChanged.Connect(this, &CrossEditor::OnScreenChanged);
}

CrossEditor::~CrossEditor() {
	delete ui;
}

void CrossEditor::Start() {
	Game::Start();
	CROSS_FAIL(system->IsAssetDirectoryExists("Engine"), 
		"Folder 'Engine' does not exists in project directory :\n  '#'\nEditor will not work properly",
		GetFileExplorer()->GetProjectDirectory().toLatin1().data());
	SetScreen(new SceneView());
}

void CrossEditor::Update(float sec) {
	ui->properties_view->Update(sec);
}

SceneExplorer* CrossEditor::GetSceneExplorer() {
	return ui->scene_explorer;
}

PropertiesView* CrossEditor::GetPropertyView() {
	return ui->properties_view;
}

FileExplorer* CrossEditor::GetFileExplorer() {
	return ui->file_explorer;
}

Ui::CrossEditorUI* CrossEditor::GetUI() {
	return ui;
}

void CrossEditor::closeEvent(QCloseEvent* eve) {
	ui->gl_handler->ShutDown();
	QSettings settings("Data/EditorConfig.ini", QSettings::IniFormat);
	settings.setValue("Geometry", QVariant(geometry()));
	settings.setValue("WindowState", saveState());

	QString projectDir = GetFileExplorer()->GetProjectDirectory();
	settings.setValue("ProjectDirectory", projectDir);

	QMainWindow::closeEvent(eve);
	qApp->quit();
}

void CrossEditor::LoadScene(QString& scenePath) {
	GetSceneExplorer()->clearSelection();
	SetScreen(new SceneView(scenePath.toLatin1().data()));
}

void CrossEditor::RestoreSettings() {
	ESystem* esystem = dynamic_cast<ESystem*>(system);
	QSettings settings("Data/EditorConfig.ini", QSettings::IniFormat);
	QString file = settings.fileName();
	if(settings.contains("Geometry")) {
		setGeometry(settings.value("Geometry").value<QRect>());
	}
	if(settings.contains("ProjectDirectory")) {
		QString projectDirectory = settings.value("ProjectDirectory").toString();
		esystem->SetAssetPath(projectDirectory.toStdString().c_str());
		GetFileExplorer()->SetupProjectDirectory(projectDirectory);
	} else {
		GetFileExplorer()->SetupProjectDirectory(QDir::currentPath() + "/" + QString(system->AssetsPath().ToCStr()));
	}
	restoreState(settings.value("WindowState").toByteArray());
}

void CrossEditor::OnSomethingChanged(Action* action) {
	setWindowTitle(QString("Cross Editor - ") + GetCurrentScene()->GetName() + QString("*"));
}

void CrossEditor::OnScreenChanged(Screen* screen) {
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
