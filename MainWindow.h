#pragma once

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QTabWidget>

#include "Document.h"
#include "Session.h"

class MainWindow : public QWidget
{
	Q_OBJECT

	private:

		QMenuBar* _menuBar;

		QMenu* _fileMenu;
		QAction* _newAction;
		QAction* _openAction;
		QMenu*   _openRecentMenu;
		QAction* _saveAction;
		QAction* _saveAsAction;
		QAction* _printAction;
		QAction* _exitAction;

		QMenu* _editMenu;
		QAction* _undoAction;
		QAction* _redoAction;

		QMenu* _formatMenu;
		QAction* _wordWrapToggle;
		QAction* _fontAction;
		QAction* _colorAction;

		QMenu* _viewMenu;
		QAction* _toolBarToggle;
		QAction* _statusBarToggle;

		QMenu* _aboutMenu;
		QAction* _aboutQtAction;
		QAction* _aboutAction;

		QToolBar* _toolBar;

		QTabWidget* _tabs;

		QStatusBar* _statusBar;

		Session _session;

		// utility methods
		void initDocument();
		void updateRecentFiles();


	public:

		MainWindow();

		Document* currentDocument(){
			return dynamic_cast<Document*>(_tabs->currentWidget());
		}

		// overridden event handlers
		void closeEvent(QCloseEvent *evt);
		void leaveEvent(QEvent *e);
		void enterEvent(QEvent *e);

		bool eventFilter(QObject* watched, QEvent* event);




	public slots:

		// menu actions / toggles
		void newActionTriggered();
		void openActionTriggered();
		void saveActionTriggered();
		void printActionTriggered();
		void undoActionTriggered();
		void redoActionTriggered();
		void fontActionTriggered();
		void colorActionTriggered();
		void aboutQtActionTriggered();
		void aboutActionTriggered();
		void wordWrapToggled(bool checked);
		void toolBarToggled(bool checked);
		void statusBarToggled(bool checked);

		// document-related slots
		void tabCloseClicked(int index);
		void tabChanged(int index);
		void documentChanged();
		void cursorPositionChanged();

		
};