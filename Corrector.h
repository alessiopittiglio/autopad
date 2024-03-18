#pragma once

#include <set>
#include <string>
#include <list>

const int WORD_SIZE_MAX = 50;

class Corrector
{
	private:

		std::set <std::string> dictionary;
		int D[WORD_SIZE_MAX][WORD_SIZE_MAX];  // preallocated distance matrix

		// singleton design pattern
		// -> constructor not accessible
		Corrector();

		// utility methods
		int Levenshtein_distance(std::string X, std::string Y);

		static inline int min(int a, int b, int c){
			return std::min(std::min(a,b),c);
		}

	public:

		// singleton design pattern
		// -> access point for unique instance
		static Corrector* instance();

		// suggest most similar words in dictionary using string distance
		std::list<std::string> suggest(std::string word, int n=1);

		// return true if word is already correct
		bool isCorrect(std::string word);

		
};