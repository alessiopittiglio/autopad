#pragma once

#include <QSettings>
#include <string>
#include <list>
#include <QFont>

class Session
{
	private:

		QSettings settings;

		bool _wordWrap;
		std::list <QString> _recentFiles;
		QFont _font;
		// ...other options to be maintained 

		// utility methods
		void init_defaults();
		void read();
		void write();

	public:

		Session(std::string org, std::string appname);
		~Session();

		// getter
		bool wordWrap(){return _wordWrap;}
		const std::list <QString> & recentFiles(){return _recentFiles;}
		const QFont & font(){return _font;}

		// setter
		void setWordWrap(bool enabled){_wordWrap = enabled;}
		void setFont(const QFont & newFont){_font = newFont;}
		void addRecentFile(const QString & filepath);
};