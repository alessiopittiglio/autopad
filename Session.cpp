#include "Session.h"

Session::Session(std::string org, std::string appname)
	: settings(org.c_str(), appname.c_str())
{
	init_defaults();
	read();
}

Session::~Session()
{
	write();
}

void Session::init_defaults()
{
	_wordWrap = true;
	_font = QFont("Calibri", 14);
}

void Session::read()
{
	if(settings.contains("wordWrap"))
		_wordWrap = settings.value("wordWrap").toBool();

	if(settings.contains("fontFamily"))
		_font.setFamily(settings.value("fontFamily").toString());

	if(settings.contains("fontSize"))
		_font.setPointSize(settings.value("fontSize").toInt());

	if(settings.contains("fontWeight"))
		_font.setWeight(settings.value("fontWeight").toInt());

	if(settings.contains("fontItalic"))
		_font.setItalic(settings.value("fontItalic").toBool());

	size_t dim = settings.beginReadArray("recentFiles");
	for(int i=0; i<dim; i++)
	{
		settings.setArrayIndex(i);
		_recentFiles.push_back(settings.value("path").toString());
	}
	settings.endArray();
}

void Session::write()
{
	settings.setValue("wordWrap", _wordWrap);

	settings.setValue("fontFamily", _font.family());
	settings.setValue("fontSize", _font.pointSize());
	settings.setValue("fontWeight", _font.weight());
	settings.setValue("fontItalic", _font.italic());

	settings.beginWriteArray("recentFiles");
	int i=0;
	for(auto & f : _recentFiles)
	{
		settings.setArrayIndex(i++);
		settings.setValue("path", f);
	}
	settings.endArray();
}

void Session::addRecentFile(const QString & filepath)
{
	if(std::find(_recentFiles.begin(), _recentFiles.end(), filepath) != _recentFiles.end())
		_recentFiles.remove(filepath);

	_recentFiles.push_front(filepath);

	if(_recentFiles.size() > 10)
		_recentFiles.pop_back();
}