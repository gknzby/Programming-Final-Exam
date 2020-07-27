#include <iostream>
#include <set>
#include <string>
#include <iterator>
#include <vector>
#include <fstream>
#include <time.h>

using namespace std;

class Dictionary
{
private:
	set<string> wordList;
	vector<string> secrets;

	//Yinelenen harf kontrolü +
	bool TestLegalSecretWord(string word)
	{
		for (int i = 0; i < 5; i++)
			for (int j = i + 1; j < 5; j++)
				if (word[i] == word[j])
					return false;
		return true;
	}
	

public:
	Dictionary() {}
	Dictionary(char* dosyaYolu)
	{
		string bufferStr = "";

		ifstream wordDosya(dosyaYolu);

		while (getline(wordDosya, bufferStr))
		{
			wordList.insert(bufferStr);
			if (TestLegalSecretWord(bufferStr))
				secrets.push_back(bufferStr);
		}

	}
	~Dictionary() {}

	bool IsWordValid(string word)
	{
		for (set<string>::iterator it = wordList.begin(); it != wordList.end(); ++it)
			if (*it == word)
				return true;
		return false;
	}
	string BringValidWord()
	{
		int rast;

		while (true)
		{
			rast = rand() % secrets.size();
			if (TestLegalSecretWord(secrets[rast]))
				return secrets[rast];
		}
	}
};

class Words
{
private:
	int numguesses;
	bool cheated;
	string secret;
	Dictionary *dict;

	//Doðru yerdeki harf kontrolü +
	int countofTruePlaceLetters(string predict)
	{
		int sayac = 0;
		for (int i = 0; i < 5; i++)
		{
			if (predict[i] == secret[i])
				sayac++;
		}

		return sayac;
	}

	//Eþleþen harf sayýsý +
	int countofMatchLetters(string predict)
	{
		int sayac = 0;
		int konum[] = { 0, 0, 0, 0, 0 };

		for (int i = 0; i < 5; i++)
			for (int j = i + 1; j < 5; j++)
				if (predict[i] == predict[j])
					konum[j]++;

		for (int i = 0; i < 5; i++)
		{
			if (konum[i] != 0)
				continue;
			for (int j = 0; j < 5; j++)
			{
				if (predict[i] == secret[j])
				{
					sayac++;
				}
			}
		}

		return sayac;
	}

public:
	Words()
	{
		srand(time(NULL));
		char dosyaYolu[30] = "words.txt";
		dict = new Dictionary(dosyaYolu);

		secret = dict->BringValidWord();
		
		numguesses = 0, cheated = false;
	}
	~Words() {}

	void playgame(Words& words)
	{
		cout << "Bilbakalim Oyununa Hosgeldiniz :" << endl
			<< "5 harften olusan bir kelime tuttum." << endl;

		string predict = "";

		while (true)
		{
			cout << "Tahmininiz nedir : ";
			cin >> predict;

			numguesses++;
			if (predict == secret)
			{
				break;
			}
			else if (predict == "xxxxx")
			{
				cout << "Ipucu : Gizli kelime : " << secret << endl;
				cheated = true;
			}
			else
			{
				if (dict->IsWordValid(predict))
				{
					cout << "Eslesen harf sayisi : " << countofMatchLetters(predict) << endl
						<< "Eslesen harflerin dogru konumu : " << countofTruePlaceLetters(predict) << endl;
				}
				else
				{
					cout << "Bu kelime sozlukte yok!" << endl;
				}
			}
			cout << endl;
		}

		cout << "Bildiniz " << numguesses << " tahminde " << (cheated ? "ipucu kullanarak " : "") << "dogru sonuca ulastiniz!" << endl;
	}
};

int main()
{
	Words words;

	words.playgame(words);

	return 0;
}