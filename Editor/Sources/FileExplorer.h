#ifndef FILE_EXPLORER
#define FILE_EXPLORER

#include "Cross.h"
#include "Event.h"

#include <QTreeView>
#include <QLabel>

using namespace cross;

class QFileSystemModel;

class FileExplorer : public QTreeView
{
	Q_OBJECT
public:
	Event<QString> FileSelected;
	Event<QString> ProjectDirectoryChanged;
	Event<bool> VisibilityChanged;

	FileExplorer(QWidget* parent = 0);
	~FileExplorer();

	void SetupProjectDirectory(QString dir);
	QString GetProjectDirectory() const;
	QString GetRelativePath(const QString& absolutePath) const;

protected:
	void contextMenuEvent(QContextMenuEvent *eve) override;
	void hideEvent(QHideEvent *eve) override;
	void showEvent(QShowEvent *eve) override;

private:
	QFileSystemModel* file_system	= nullptr;
	QMenu* context_menu				= nullptr;
	QModelIndexList clipboard;

	void OnItemSelected(QModelIndex index);
	void OnItemDoubleClick(QModelIndex index);

	void OnNewFolderClick();
	void OnNewShaderClick();
	void OnNewMaterialClick();
	void OnCopyClick();
	void OnPasteClick();
	void OnDeleteClick();

	QString GetSelectedDirectory();
	QString GetAllowedName(const QString& dir, const QString& baseName, const QString& extension);
};

class ProjectDirectoryLabel : public QLabel
{
public:
	ProjectDirectoryLabel(QWidget* parent);

private:
	void OnProjectDirectoryChanged(QString path);
};

#endif