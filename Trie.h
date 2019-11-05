#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "TrieNode.h"

using namespace std;

class Trie
{
private:
	TrieNode* _root = nullptr;

protected:

public:
	Trie()
	{
		_root = new TrieNode{};
	}

	virtual ~Trie()
	{
		//TODO: clean up memory
		delete _root;
	}

	//TODO: implement
	void addWord(const string& word)
	{
		TrieNode* temp = _root;
		for (auto letter : word)
		{
			if (temp->hasChild(letter) == false)
			{
				temp->setChild(letter);
			}
			temp = temp->getChild(letter);
		}

		if (temp->hasChild('$') == false)
		{
			temp->setChild('$');
		}
	}

	//TODO: implement
	vector<string> search(const string& word)
	{
		vector<string> matches;
		TrieNode* temp = _root;

		if (word == "")
		{
			return matches;
		}

		for (auto letter : word)
		{
			if (temp->hasChild(letter) == false)
			{
				return matches;
			}
			temp = temp->getChild(letter);
		}

		if (temp->isWord())
		{
			matches.push_back(word);
		}

		for (auto node : temp->getChildren())
		{
			search_helper(word, node.second, matches);
		}

		return matches;
	}

	void search_helper(string word, TrieNode* temp, vector<string>& matches)
	{
		word += temp->getValue();

		if (temp->isWord())
		{
			matches.push_back(word);
		}

		for (auto node : temp->getChildren())
		{
			search_helper(word, node.second, matches);
		}
	}
};
