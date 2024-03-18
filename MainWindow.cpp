#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QColorDialog>
#include <QFontDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QApplication>
#include <QMessageBox>
#include <QCloseEvent>
#include "MainWindow.h"

MainWindow::MainWindow() : QWidget(0), _session("UNICLAM", "AutoPad")
{
	_menuBar = new QMenuBar();

	_fileMenu = new QMenu("File");
	_newAction = new QAction(QIcon(":/icons/new.png"), "New", _fileMenu);
	_newAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	_fileMenu->addAction(_newAction);
	_openAction = new QAction(QIcon(":/icons/open.png"), "Open", _fileMenu);
	_openAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	_fileMenu->addAction(_openAction);
	_openRecentMenu = new QMenu("Open recent");
	_fileMenu->addMenu(_openRecentMenu);
	_saveAction = new QAction(QIcon(":/icons/save.png"), "Save", _fileMenu);
	_saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	_saveAction->setEnabled(false);
	_fileMenu->addAction(_saveAction);
	_saveAsAction = new QAction("Save as", _fileMenu);
	_fileMenu->addAction(_saveAsAction);
	_printAction = new QAction(QIcon(":/icons/printer.png"), "Print", _fileMenu);
	_printAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	_fileMenu->addSeparator();
	_fileMenu->addAction(_printAction);
	_fileMenu->addSeparator();
	_exitAction = new QAction("Exit", _fileMenu);
	_exitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
	_fileMenu->addAction(_exitAction);

	_editMenu = new QMenu("Edit");
	_undoAction = new QAction(QIcon(":/icons/undo.png"), "Undo", _editMenu);
	_undoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
	_undoAction->setEnabled(false);
	_editMenu->addAction(_undoAction);
	_redoAction = new QAction(QIcon(":/icons/redo.png"), "Redo", _editMenu);
	_redoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z));
	_redoAction->setEnabled(false);
	_editMenu->addAction(_redoAction);

	_formatMenu = new QMenu("Format");
	_wordWrapToggle = new QAction(QIcon(":/icons/wordwrap.png"), "Word Wrap", _formatMenu);
	_wordWrapToggle->setCheckable(true);
	_wordWrapToggle->setChecked(_session.wordWrap());
	_formatMenu->addAction(_wordWrapToggle);
	_fontAction = new QAction("Font...", _formatMenu);
	_formatMenu->addAction(_fontAction);
	_colorAction = new QAction("Color...", _formatMenu);
	_formatMenu->addAction(_colorAction);

	_viewMenu = new QMenu("View");
	_toolBarToggle = new QAction("Tool Bar", _viewMenu);
	_toolBarToggle->setCheckable(true);
	_toolBarToggle->setChecked(true);
	_viewMenu->addAction(_toolBarToggle);
	_statusBarToggle = new QAction("Status Bar", _viewMenu);
	_statusBarToggle->setCheckable(true);
	_statusBarToggle->setChecked(true);
	_viewMenu->addAction(_statusBarToggle);

	_aboutMenu = new QMenu("?");
	_aboutQtAction = new QAction("About Qt", _aboutMenu);
	_aboutMenu->addAction(_aboutQtAction);
	_aboutAction = new QAction(QIcon(":/icons/about.png"), "About AutoPad", _aboutMenu);
	_aboutMenu->addAction(_aboutAction);

	_menuBar->addMenu(_fileMenu);
	_menuBar->addMenu(_editMenu);
	_menuBar->addMenu(_formatMenu);
	_menuBar->addMenu(_viewMenu);
	_menuBar->addMenu(_aboutMenu);

	_toolBar = new QToolBar();
	_toolBar->setIconSize(QSize(32,32));
	_toolBar->addAction(_newAction);
	_toolBar->addAction(_openAction);
	_toolBar->addAction(_saveAction);
	_toolBar->addAction(_printAction);
	_toolBar->addSeparator();
	_toolBar->addAction(_undoAction);
	_toolBar->addAction(_redoAction);
	_toolBar->addSeparator();
	_toolBar->addAction(_wordWrapToggle);
	_toolBar->addSeparator();
	_toolBar->addAction(_aboutAction);

	_tabs = new QTabWidget();
	_tabs->setIconSize(QSize(13,13));
	_tabs->setTabIcon(0, QIcon(":/icons/saved.ico"));
	_tabs->setTabsClosable(true);

	_statusBar = new QStatusBar();

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(_menuBar);
	layout->addWidget(_toolBar);
	layout->addWidget(_tabs);
	layout->addWidget(_statusBar);
	setLayout(layout);

	setMinimumSize(800, 600);
	setWindowIcon(QIcon(":/icons/notepad.png"));
	setWindowState(Qt::WindowMaximized);

	connect(_newAction, SIGNAL(triggered()), this, SLOT(newActionTriggered()));
	connect(_openAction, SIGNAL(triggered()), this, SLOT(openActionTriggered()));
	connect(_saveAction, SIGNAL(triggered()), this, SLOT(saveActionTriggered()));
	connect(_saveAsAction, SIGNAL(triggered()), this, SLOT(saveActionTriggered()));
	connect(_printAction, SIGNAL(triggered()), this, SLOT(printActionTriggered()));
	connect(_exitAction, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
	connect(_undoAction, SIGNAL(triggered()), this, SLOT(undoActionTriggered()));
	connect(_redoAction, SIGNAL(triggered()), this, SLOT(redoActionTriggered()));
	connect(_fontAction, SIGNAL(triggered()), this, SLOT(fontActionTriggered()));
	connect(_colorAction, SIGNAL(triggered()), this, SLOT(colorActionTriggered()));
	connect(_aboutAction, SIGNAL(triggered()), this, SLOT(aboutActionTriggered()));
	connect(_aboutQtAction, SIGNAL(triggered()), this, SLOT(aboutQtActionTriggered()));
	connect(_wordWrapToggle, SIGNAL(toggled(bool)), this, SLOT(wordWrapToggled(bool)));
	connect(_toolBarToggle, SIGNAL(toggled(bool)), this, SLOT(toolBarToggled(bool)));
	connect(_statusBarToggle, SIGNAL(toggled(bool)), this, SLOT(statusBarToggled(bool)));
	connect(_tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseClicked(int)));
	connect(_tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

	updateRecentFiles();
}


void MainWindow::updateRecentFiles()
{
	_openRecentMenu->clear();

	for(auto & f : _session.recentFiles())
	{
		QAction *act = new QAction(f, _openRecentMenu);
		_openRecentMenu->addAction(act);
		connect(act, SIGNAL(triggered()), this, SLOT(openActionTriggered()));
	}
}

void MainWindow::newActionTriggered()
{
	Document* doc = new Document();
	_tabs->addTab(doc, QIcon(":/icons/saved.ico"), QString("new ") + QString::number(doc->id()));
	_tabs->setCurrentWidget(doc);

	initDocument();
}

void MainWindow::openActionTriggered()
{
	QString filepath;

	if(QObject::sender() == _openAction)
		filepath = QFileDialog::getOpenFileName(this, "Select file", QString(), "Text (*.txt);; All files (*.*)");
	else
		filepath = dynamic_cast<QAction*>(QObject::sender())->text();

	if(!filepath.isEmpty())
	{
		// evita di aprire lo stesso documento due volte
		// se trova il documento gia' aperto, lo seleziona
		int doc_index = -1;
		for(int i=0; i<_tabs->count(); i++)
			if(dynamic_cast<Document*>(_tabs->widget(i))->filepath() == filepath)
			{
				doc_index = i;
				break;
			}
		if(doc_index >= 0)
			_tabs->setCurrentIndex(doc_index);
		else
		{
			Document* doc = new Document(filepath);
			_tabs->addTab(doc, QIcon(":/icons/saved.ico"), doc->filename());
			_tabs->setCurrentWidget(doc);

			initDocument();

			_session.addRecentFile(filepath);
			updateRecentFiles();
		}
	}

}

// utility methods
void MainWindow::initDocument()
{
	Document* doc = currentDocument();
	if(doc)
	{
		connect(doc, SIGNAL(textChanged()), this, SLOT(documentChanged()));
		connect(doc, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));

		wordWrapToggled(_wordWrapToggle->isChecked());
		doc->setFont(_session.font());
	}
}

void MainWindow::saveActionTriggered()
{
	if(!currentDocument())
		return;

	if(currentDocument()->filepath().isEmpty() || 
		QObject::sender() == _saveAsAction)
	{
		QString filepath = QFileDialog::getSaveFileName(this, "Save file", QString(), "Text (*.txt)");
		if(!filepath.isEmpty())
		{
			currentDocument()->setFilePath(filepath);
			_tabs->setTabText(_tabs->indexOf(currentDocument()), currentDocument()->filename());
			currentDocument()->save();

			_session.addRecentFile(filepath);
			updateRecentFiles();
		}
	}
	else
		currentDocument()->save();

	if(currentDocument()->saved())
	{
		_tabs->setTabIcon(_tabs->indexOf(currentDocument()), QIcon(":/icons/saved.ico"));
		_saveAction->setEnabled(false);
	}

}

void MainWindow::printActionTriggered()
{
	QPrinter printer;
	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec() == QDialog::Accepted)
		currentDocument()->print(&printer);
}

void MainWindow::undoActionTriggered()
{
	if(currentDocument())
	{
		currentDocument()->undo();
		_undoAction->setEnabled(currentDocument()->isUndoAvailable());
		_redoAction->setEnabled(currentDocument()->isRedoAvailable());
	}
}

void MainWindow::redoActionTriggered()
{
	if(currentDocument())
	{
		currentDocument()->redo();
		_undoAction->setEnabled(currentDocument()->isUndoAvailable());
		_redoAction->setEnabled(currentDocument()->isRedoAvailable());
	}
}

void MainWindow::fontActionTriggered()
{
	if(!currentDocument())
		return;

	bool dialog_accepted;
	QFont font = QFontDialog::getFont(&dialog_accepted, currentDocument()->font(), this);

	if(dialog_accepted)
	{
		for (int i=0; i<_tabs->count(); i++)
			dynamic_cast<Document*>(_tabs->widget(i))->setFont(font);

		_session.setFont(font);
	}
}

void MainWindow::colorActionTriggered()
{
	if(!currentDocument())
		return;

	QColor color = QColorDialog::getColor(currentDocument()->textColor(), this);

	if(color.isValid())
		currentDocument()->setTextColor(color);
}

void MainWindow::aboutQtActionTriggered()
{
	QMessageBox::aboutQt(this);
}

void MainWindow::aboutActionTriggered()
{
	QString text("<html><h1>AutoPad v");
	text = text + "1.0.0" + "</h1>"
		"<big>A tool for multi-document editing with automated text correction</big><br><br>"
		"Developed by the class of <i>Tecniche di Programmazione 2020</i> "
		"under the supervision of Prof. Alessandro Bria (email: <a href=\"mailto:a.bria@unicas.it\">a.bria@unicas.it</a>)</li><br><br>"
		"For further info, please visit our <a href=\"https://drive.google.com/drive/folders/0Bw_NEuFt5pE6fnBRZkhiT0Q1YVB6a2VyZkJCUjBLU09qWHdGNmw4NHAtVnlNWDR6bmdCbU0?usp=sharing/\">website</a>.<br>"
		"<br><small>(c) 2020 University of Cassino and L.M.</small></html>";
	
	QMessageBox::about(this, "About AutoPad", text);
}

void MainWindow::wordWrapToggled(bool checked)
{
	_session.setWordWrap(checked);
	for (int i=0; i<_tabs->count(); i++)
		dynamic_cast<Document*>(_tabs->widget(i))->setWordWrapMode(checked ? QTextOption::WordWrap : QTextOption::NoWrap);
}

void MainWindow::toolBarToggled(bool checked)
{
	_toolBar->setVisible(checked);
}

void MainWindow::statusBarToggled(bool checked)
{
	_statusBar->setVisible(checked);
}

void MainWindow::tabCloseClicked(int index)
{
	Document *doc = dynamic_cast<Document*>(_tabs->widget(index));
	if(!doc->saved())
	{
		QMessageBox::StandardButton choice = 
			QMessageBox::question(this, "Save", 
			QString("Save file \"") + _tabs->tabText(index) + "\"", 
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if(choice == QMessageBox::Yes)
			saveActionTriggered();
		else if(choice == QMessageBox::No)
			;
		else
			return;
	}
	delete _tabs->widget(index);
}

void MainWindow::tabChanged(int index)
{
	Document *doc = dynamic_cast<Document*>(_tabs->widget(index));
	if(doc)
	{
		_saveAction->setEnabled(!doc->saved());
		_undoAction->setEnabled(doc->isUndoAvailable());
		_redoAction->setEnabled(doc->isRedoAvailable());
	}
}

void MainWindow::cursorPositionChanged()
{
	if(currentDocument())
	{
		int line   = currentDocument()->textCursor().blockNumber();
		int column =  currentDocument()->textCursor().positionInBlock();
		_statusBar->showMessage(QString("Line: " + QString::number(line) + "   Column: " + QString::number(column)));
	}
}

void MainWindow::documentChanged()
{
	Document* doc_sender = dynamic_cast<Document*>(QObject::sender());
	if(doc_sender)
	{
		doc_sender->setSaved(false);
		_tabs->setTabIcon(_tabs->indexOf(doc_sender), QIcon(":/icons/unsaved.ico"));
		_saveAction->setEnabled(true);
		_undoAction->setEnabled(doc_sender->isUndoAvailable());
		_redoAction->setEnabled(doc_sender->isRedoAvailable());
	}
}

// overridden event handlers
void MainWindow::closeEvent(QCloseEvent *evt)
{
	while(_tabs->currentIndex() >= 0)
	{
		Document* doc_before = currentDocument();
		tabCloseClicked(_tabs->currentIndex());
		Document* doc_after = currentDocument();

		if(doc_after == doc_before)
		{
			evt->ignore();
			break;
		}
	}
}

void MainWindow::leaveEvent(QEvent *e)
{
	//setWindowOpacity(0.5);
}

void MainWindow::enterEvent(QEvent *e)
{
	//setWindowOpacity(1);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
	return false;
}