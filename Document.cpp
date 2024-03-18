#include "Document.h"
#include "Corrector.h"
#include <fstream>
#include <QFileInfo>
#include <QMenu>
#include <QContextMenuEvent>

std::set<int> Document::_available_ids = std::set<int>();

Document::Document()
{
	// se true, significa che e' il primo oggetto creato durante l'esecuzione
	//          oppure che non ci sono piu' id disponibili (BUG)
	if(_available_ids.empty())
	{
		for(int i=1; i<256; i++)
			_available_ids.insert(i);
	}

	_id = *(_available_ids.begin());
	_available_ids.erase(_available_ids.begin());
	_saved = true;

	setPlaceholderText("Type your text here...");

	init();
}

Document::Document(QString filepath)
{
	_id = 0;
	_filepath = filepath;
	_saved = true;

	std::ifstream f(filepath.toStdString());
	if(!f.is_open())
		throw("cannot read");
	std::string text;
	std::string line;
	while(std::getline(f, line))
		text += line + "\n";
	f.close();

	setPlainText(text.c_str());

	QFileInfo info(filepath);
	_filename = info.fileName();

	init();
}

void Document::init()
{
	_isUndoAvailable = false;
	_isRedoAvailable = false;

	connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(undoAvailable(bool)));
	connect(this, SIGNAL(redoAvailable(bool)), this, SLOT(redoAvailable(bool)));
}

Document::~Document()
{
	if(_id)
		_available_ids.insert(_id);
}

void Document::setFilePath(QString newFilePath)
{
	_filepath = newFilePath;
	QFileInfo info(_filepath);
	_filename = info.fileName();
}

void Document::save()
{
	std::ofstream f(_filepath.toStdString());
	if(!f.is_open())
		throw("cannot save");
	f << toPlainText().toStdString();
	f.close();

	_saved = true;
}

void Document::wheelEvent(QWheelEvent *e)
{
	if(e->modifiers() == Qt::CTRL)
	{
		if(e->delta() > 0)
			zoomIn();
		else
			zoomOut();
	}
	else
		QTextEdit::wheelEvent(e);
}

void Document::contextMenuEvent(QContextMenuEvent *event)
{
	// create custom (overridden) menu
	QMenu *menu = createStandardContextMenu(event->pos());
	menu->exec(event->globalPos());
	delete menu;
}

QMenu* Document::createStandardContextMenu(const QPoint &position)
{
	// get standard right-click menu from superclass
	QMenu* stdMenu= QTextEdit::createStandardContextMenu();

	// update click position so we can later retrieve text cursor
	_lastContextMenuPos = position;

	// get word at current position
	QTextCursor tc = cursorForPosition(position);
	tc.select(QTextCursor::WordUnderCursor);
	QString word = tc.selectedText();
	tc.clearSelection();
	
	// convert to lowercase C++ string
	std::string wordstd = word.toLower().toStdString();

	// if word is already correct, we do not add anything to the menu
	if(Corrector::instance()->isCorrect(wordstd))
		return stdMenu;

	// ...otherwise we get similar words and display them in the menu
	auto suggestions = Corrector::instance()->suggest(wordstd);
	stdMenu->insertSeparator(stdMenu->actions().first());
	for(auto & sug : suggestions)
	{
		QAction *sug_act = new QAction(sug.c_str(), stdMenu);
		sug_act->setFont(QFont(sug_act->font().family(), sug_act->font().pointSize(), QFont::Bold));
		stdMenu->insertAction(stdMenu->actions().first(), sug_act );

		connect(sug_act, SIGNAL(triggered()), this, SLOT(suggestedWordClicked()));
	}

	return stdMenu;
}

void Document::suggestedWordClicked()
{
	QAction *sug_act = dynamic_cast<QAction*>(QObject::sender());

	if(sug_act)
	{
		QTextCursor tc = cursorForPosition(_lastContextMenuPos);
		tc.select(QTextCursor::WordUnderCursor);
		tc.deleteChar();	// deletes currently selected text
		tc.insertText(sug_act->text());
		tc.clearSelection();
	}
}

void Document::undoAvailable(bool available)
{
	_isUndoAvailable = available;
}

void Document::redoAvailable(bool available)
{
	_isRedoAvailable = available;
}