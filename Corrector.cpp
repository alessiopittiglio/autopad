#include "Corrector.h"
#include <QFile>
#include <QElapsedTimer>
#include <QTextStream>

Corrector::Corrector()
{
	// read whole dictionary
	QFile f(":/dictionaries/ita.txt");
	if(!f.open(QIODevice::ReadOnly))
		throw ("cannot read dictionary");
	QTextStream in(&f);
	while (!in.atEnd())
	{
		QString line = in.readLine();
		dictionary.insert(line.toStdString());
	}
	f.close();
}

Corrector* Corrector::instance()
{
	static Corrector unique_instance;
	return &unique_instance;
}

// suggest most similar word in dictionary using string distance
std::list<std::string> Corrector::suggest(std::string word, int n)
{
	// @TODO: implement n > 1
	if(n != 1)
		throw "Not yet implemented";

	std::list <std::string> suggestions;
	QElapsedTimer timer;
	timer.start();

	// 'old' approach to iterate over STL container
	//for(std::set<std::string>::iterator it = dictionary.begin(); it != dictionary.end(); it++)
	//	;
	int min_dist = std::numeric_limits<int>::max();
	std::string closest_word;
	for(auto & dict_word : dictionary)
	{
		// skip condition:
		// we hypothesize that the user correctly digits the first two letters
		if(dict_word[0] != word[0] || dict_word[1] != word[1])
			continue;

		int dist = Levenshtein_distance(dict_word, word);

		if(dist < min_dist)
		{
			min_dist = dist;
			closest_word = dict_word;
		}
	}
	
	suggestions.push_front(closest_word);

	printf("Time elapsed for word suggestion \"%s\" = %.3f seconds\n", word.c_str(), timer.elapsed()/1000.0f);

	return suggestions;
}

int Corrector::Levenshtein_distance(std::string X, std::string Y)
{
	int m = X.size();
	int n = Y.size();

	// check word can fit preallocated distance matrix
	if(m >= WORD_SIZE_MAX || n >= WORD_SIZE_MAX)
		throw ("exceeded max word length");

	// initialization
	for(int i=0; i<m+1; i++)
		D[i][0] = i;
	for(int j=0; j<n+1; j++)
		D[0][j] = j;

	// distance algorithm (dynamic programming)
	for(int i=1; i<m+1; i++)
		for(int j=1; j<n+1; j++)
			if(X[i] != Y[j])
				D[i][j] = 1 + min(D[i][j-1], D[i-1][j], D[i-1][j-1]);
			else
				D[i][j] = D[i-1][j-1];

	// solution
	return D[m][n];
}

// return true if word is already correct
bool Corrector::isCorrect(std::string word)
{
	return dictionary.find(word) != dictionary.end();
}