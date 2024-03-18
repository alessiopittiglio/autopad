#pragma once

#include <QTextEdit>
#include <set>

class Document : public QTextEdit
{
	Q_OBJECT

	private:

		int _id;
		QString _filename;
		QString _filepath;
		bool _saved;
		QPoint _lastContextMenuPos;
		bool _isUndoAvailable;
		bool _isRedoAvailable;

		static std::set<int> _available_ids;

		void init();

	public:

		Document();
		Document(QString filepath);

		~Document();

		// getters
		int id(){return _id;}
		QString filename(){return _filename;}
		QString filepath(){return _filepath;}
		bool saved(){return _saved;}
		bool isUndoAvailable(){return _isUndoAvailable;}
		bool isRedoAvailable(){return _isRedoAvailable;}

		// setters
		void setFilePath(QString newFilePath);
		void setSaved(bool newSaved){_saved = newSaved;}

		void save();

		// @override event handlers
		void contextMenuEvent(QContextMenuEvent *event);
		void wheelEvent(QWheelEvent *e);

		QMenu* createStandardContextMenu(const QPoint &position);

	public slots:

		void suggestedWordClicked();
		void undoAvailable(bool);
		void redoAvailable(bool);
};