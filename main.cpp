#define PDC_DLL_BUILD 1
#include "curses.h"
#include "TrieNode.h"
#include "Trie.h"
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#define ENTER_KEY 10

using namespace std;

void saveToBinaryFile(vector<vector<int>> data);
string decimalConvert(int n);

vector<string> insertionSortWord(vector<vector<int>> data);
vector<string> selectionSortWord(vector<vector<int>> data);
vector<string> bubbleSortWord(vector<vector<int>> data);
vector<string> quickSortWord(vector<vector<int>> data);

template <typename T>
void insertionSort(vector<T>& data);

template <typename T>
void selectionSort(vector<T>& data);

template <typename T>
void bubbleSort(vector<T>& data);

template <typename T>
void sortHelper(vector<T>& data, int start_index, int end_index);

template <typename T>
void quickSort(vector<T>& data);
//unsigned char border_char = 219;

int main(int argc, char* argv[])
{
	Trie dictionary;
	

	ifstream dictionaryFile;
	dictionaryFile.open("keywords.txt");

	while (dictionaryFile.good())
	{
		string line;
		getline(dictionaryFile, line);
		dictionary.addWord(line);
	}

	dictionaryFile.close();

	WINDOW* main_window = nullptr;
	refresh();

	int num_rows = 0;
	int num_cols = 0;

	//initialize screen, begin curses mode
	main_window = initscr();

	//take up most of the screen
	getmaxyx(main_window, num_rows, num_cols);
	resize_term(num_rows - 1, num_cols - 1);
	getmaxyx(main_window, num_rows, num_cols);

	//turn off key echo
	noecho();
	//nodelay(main_window, TRUE);
	//keypad(main_window, TRUE);
	
	curs_set(2);

	if (has_colors() == FALSE) 
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}

	//fun stuff happens here

	//Can print any message anywhere
	//mvwprintw(main_window, 3, 2, "");
	//wrefresh(main_window);

	start_color();

	//colors
	init_pair(1, COLOR_RED, COLOR_WHITE); //red
	init_pair(2, COLOR_YELLOW, COLOR_WHITE); //orange/yellow
	init_pair(3, COLOR_GREEN, COLOR_WHITE); //green
	init_pair(4, COLOR_BLUE, COLOR_WHITE); //blue
	init_pair(5, COLOR_CYAN, COLOR_WHITE); //light blue
	init_pair(6, COLOR_MAGENTA, COLOR_WHITE); //purple

	//highlights text
	wattron(main_window, A_REVERSE);
	wattron(main_window, A_BLINK);

	//top gui bar
	//mvaddstr(1, 1, "|  File  |  Edit  |  Options  |  Buffers  |  Tools  |  Help  |");

	attron(COLOR_PAIR(1));
	mvaddstr(1, 1, "|  File  ");
	attroff(COLOR_PAIR(1));

	attron(COLOR_PAIR(2));
	mvaddstr(1, 10, "|  Edit  ");
	attroff(COLOR_PAIR(2));

	attron(COLOR_PAIR(3));
	mvaddstr(1, 19, "|  Options  ");
	attroff(COLOR_PAIR(3));

	attron(COLOR_PAIR(4));
	mvaddstr(1, 30, "|  Buffers  ");
	attroff(COLOR_PAIR(4));

	attron(COLOR_PAIR(6));
	mvaddstr(1, 42, "|  Tools  ");
	attroff(COLOR_PAIR(6));

	attron(COLOR_PAIR(1));
	mvaddstr(1, 52, "|  Help  |");
	attroff(COLOR_PAIR(1));

	//bottom gui bars
	//mvaddstr(num_rows - 3, 1, "|  ^G Get Help  |  ^O Write Out  |  ^R Read File  |  ^Y Prev Page  |  ^C Cur Pos   |");

	attron(COLOR_PAIR(1));
	mvaddstr(num_rows - 3, 1, "|  ^G Get Help  ");
	attroff(COLOR_PAIR(1));

	attron(COLOR_PAIR(2));
	mvaddstr(num_rows - 3, 17, "|  ^O Write Out  ");
	attroff(COLOR_PAIR(2));

	attron(COLOR_PAIR(3));
	mvaddstr(num_rows - 3, 34, "|  ^R Read File  ");
	attroff(COLOR_PAIR(3));

	attron(COLOR_PAIR(4));
	mvaddstr(num_rows - 3, 51, "|  ^Y Prev Page  ");
	attroff(COLOR_PAIR(4));

	attron(COLOR_PAIR(6));
	mvaddstr(num_rows - 3, 68, "|  ^C Cur Pos   |");
	attroff(COLOR_PAIR(6));

	//mvaddstr(num_rows - 2, 1, "|  ^X Exit      |  ^J Justify    |  ^W Where Is   |  ^V Next Page  |  ^T To Spell  |");
	attron(COLOR_PAIR(1));
	mvaddstr(num_rows - 2, 1, "|  ^X Exit      ");
	attroff(COLOR_PAIR(1));

	attron(COLOR_PAIR(2));
	mvaddstr(num_rows - 2, 17, "|  ^J Justify     ");
	attroff(COLOR_PAIR(2));

	attron(COLOR_PAIR(3));
	mvaddstr(num_rows - 2, 34, "|  ^W Where Is   ");
	attroff(COLOR_PAIR(3));

	attron(COLOR_PAIR(4));
	mvaddstr(num_rows - 2, 51, "|  ^V Next Page  ");
	attroff(COLOR_PAIR(4));

	attron(COLOR_PAIR(6));
	mvaddstr(num_rows - 2, 68, "|  ^T To Spell  |");
	attroff(COLOR_PAIR(6));

	attron(COLOR_PAIR(5));

	for (int i = 0; i < num_cols; i++)
	{
		//top border
		mvaddch(0, i, ACS_CKBOARD);

		//bottom border
		mvaddch(num_rows - 1, i, ACS_CKBOARD);
	}
	
	for (int i = 62; i < num_cols; i++)
	{
		//finish checkerboard line next to top gui bar
		mvaddch(1, i, ACS_CKBOARD);
	}

	for (int i = 85; i < num_cols; i++)
	{
		//finish checkerboard line next to bottom gui bars
		mvaddch(num_rows - 3, i, ACS_CKBOARD);
	}

	for (int i = 85; i < num_cols; i++)
	{
		//finish line next to bottom gui bars
		mvaddch(num_rows - 2, i, ACS_CKBOARD);
	}

	for (int i = 0; i < num_rows; i++)
	{
		//left column
		mvaddch(i, 0, ACS_CKBOARD);
		
		//right column
		mvaddch(i, num_cols - 1, ACS_CKBOARD);
	}

	attroff(COLOR_PAIR(5));
	wattroff(main_window, A_REVERSE);
	wattroff(main_window, A_BLINK);

	for (int i = 1; i < num_cols - 1; i++)
	{
		//line directly under top gui bar
		mvaddch(2, i, ACS_HLINE);
	}

	for (int i = 1; i < num_cols - 1; i++)
	{
		//line directly above bottom gui bar
		mvaddch(num_rows - 4, i, ACS_HLINE);
	}

	touchwin(main_window);
	WINDOW* text_win = derwin(main_window, num_rows - 7, num_cols - 3, 3, 1);
	keypad(text_win, TRUE);
	touchwin(text_win);
	wrefresh(text_win);

	//detects when a user pressed F1 (KEY_F(1-9 or 0) for function keys), KEY_UP/DOWN/LEFT/RIGHT, etc and saves that as input
	keypad(main_window, TRUE);
	int input = getch(text_win);
	int x_loc = 0;
	int y_loc = 0;
	int start = 0;
	int textx = 0;
	int texty = 0;
	int enterTextX = 0;
	string line;
	vector<string> v1{};

	vector<vector<int>> data{};
	for (int i = 0; i < num_rows - 7; i++)
	{
		data.push_back(vector<int>{});
		data[i].resize(num_cols - 3);
		for (int j = 0; j < data[i].size(); j++)
		{
			data[i][j] = -1;
		}
	}

	//outputting file information to window
	do 
	{
		//allows for typing of a-z, A-Z, 0-9, and ./!/@/#/$/% etc
		if (input >= 32 && input <= 126)
		{
			mvwaddch(text_win, texty, textx, input);

			//add char to vector of vectors
			data[texty][textx] = input;
			textx++;

			if (textx > num_cols - 4)
			{
				texty++;
				textx = enterTextX;
				textx = 0;
			}
		}
		//pushes cursor down 1 line, saves that x value, and resets x to 0
		else if(input == ENTER_KEY)
		{
			texty++;
			enterTextX = textx;
			textx = 0;
		}
		//backspace
		else if (input == KEY_DC)
		{
			mvwaddch(text_win, texty, textx, ' ');
			wmove(text_win, texty, textx - 1);
			wrefresh(text_win);
			
			//if it's at the start of the line, it will push it back
			//up a line and set it to the x value of that previous line
			if (textx == 0)
			{
				if (texty != 0)
				{
					//delete row if on edge of row
					texty--;
					//reset x to row above
					textx = enterTextX;
				}
				else
				{
					mvwaddch(text_win, texty, textx, ' ');
					wmove(text_win, texty, textx);
					wrefresh(text_win);
				}
			}
			//moves x back a column
			else
			{
				textx--;
			}
		}
		//allows user to move backwards in their text to edit previously typed things
		else if (input == KEY_LEFT)
		{
			textx--;
			wmove(text_win, texty, textx);
			wrefresh(text_win);
		}
		//allows user to move forwards
		else if (input == KEY_RIGHT)
		{
			if (textx < num_cols - 4)
			{
				textx++;
				wmove(text_win, texty, textx);
				wrefresh(text_win);
			}
			
		}
		//converts input on screen to binary
		else if (input == ALT_P)
		{
			saveToBinaryFile(data);
		}

		else if (input == ALT_Q)
		{
			wclear(text_win);
			wrefresh(text_win);
		}

		else if (input == ALT_Z)
		{
			vector<string> result = insertionSortWord(data);
			wclear(text_win);
			wrefresh(text_win);

			for (int i = 0; i < result.size(); i++)
			{
				for (int j = 0; j < result[i].length(); j++)
				{
					mvwaddch(text_win, i, j, result[i][j]);
				}
			}
		}

		else if (input == ALT_X)
		{
			vector<string> result = selectionSortWord(data);
			wclear(text_win);
			wrefresh(text_win);

			for (int i = 0; i < result.size(); i++)
			{
				for (int j = 0; j < result[i].length(); j++)
				{
					mvwaddch(text_win, i, j, result[i][j]);
				}
			}
		}

		else if (input == ALT_C)
		{
			int x = 0;
			vector<string> result = bubbleSortWord(data);
			wclear(text_win);
			wrefresh(text_win);
			
			for (int i = 0; i < result.size(); i++)
			{
				for (int j = 0; j < result[i].length(); j++)
				{
					mvwaddch(text_win, i, j, result[i][j]);
				}
			}
		}

		else if (input == ALT_V)
		{
			vector<string> result = quickSortWord(data);
			wclear(text_win);
			wrefresh(text_win);

			for (int i = 0; i < result.size(); i++)
			{
				for (int j = 0; j < result[i].length(); j++)
				{
					mvwaddch(text_win, i, j, result[i][j]);
				}
			}
		}

		wrefresh(text_win);
		input = wgetch(text_win);

	} while ((input != KEY_F(9)));

	

	//revert back to normal console mode
	nodelay(main_window, TRUE);
	
	mvaddstr(0, 0, "Press any key to continue...");
	endwin();
}

//ALT_Z
vector<string> insertionSortWord(vector<vector<int>> data)
{
	vector<string> wordsToSort;
	string word;

	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j] == ' ')
			{
				wordsToSort.push_back(word);
				word = "";
			}
			else if (data[i][j] != -1)
			{
				word += data[i][j];
			}
		}
	}
	wordsToSort.push_back(word);
	word = "";

	insertionSort(wordsToSort);

	return wordsToSort;
}

//ALT_X
vector<string> selectionSortWord(vector<vector<int>> data)
{
	vector<string> wordsToSort;
	string word;

	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j] == ' ')
			{
				wordsToSort.push_back(word);
				word = "";
			}
			else if (data[i][j] != -1)
			{
				word += data[i][j];
			}
		}
	}
	wordsToSort.push_back(word);
	word = "";

	selectionSort(wordsToSort);

	return wordsToSort;
}

//ALT_C
vector<string> bubbleSortWord(vector<vector<int>> data)
{
	vector<string> wordsToSort;
	string word;

	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j] == ' ')
			{
				wordsToSort.push_back(word);
				word = "";
			}
			else if (data[i][j] != -1)
			{
				word += data[i][j];
			}
		}
	}
	wordsToSort.push_back(word);
	word = "";

	bubbleSort(wordsToSort);

	return wordsToSort;
}

//ALT_V
vector<string> quickSortWord(vector<vector<int>> data)
{
	vector<string> wordsToSort;
	string word;

	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j] == ' ')
			{
				wordsToSort.push_back(word);
				word = "";
			}
			else if (data[i][j] != -1)
			{
				word += data[i][j];
			}
		}
	}
	wordsToSort.push_back(word);
	word = "";

	quickSort(wordsToSort);

	return wordsToSort;
}

//Insertion
template <typename T>
void insertionSort(vector<T>& data)
{
	for (int i = 1; i < data.size(); i++)
		{
			for (int j = i; j > 0; j--)
			{
				if (data[j] < data[j - 1])
				{
					T temp = data[j];
					data[j] = data[j - 1];
					data[j - 1] = temp;
				}
				else
				{
					//in correct position relative to
					//sorted set of list

					//This extra check ensures O(N) on sorted
					//data
					break;
				}
			}
		}
}


//Selection
template <typename T>
void selectionSort(vector<T>& data)
{
	for (int i = 0; i < data.size(); i++)
	{
		int smallest_index = i;
		for (int j = i + 1; j < data.size(); j++)
		{
			if (data[j] < data[smallest_index])
			{
				smallest_index = j;
			}
		}

		T temp = data[i];
		data[i] = data[smallest_index];
		data[smallest_index] = temp;
	}
}

//Bubble
template <typename T>
void bubbleSort(vector<T>& data)
{
	for (int i = 0; i < data.size(); i++)
	{
		bool has_swapped = false;
		for (int j = 1; j < data.size() - i; j++)
		{
			if (data[j - 1] > data[j])
			{
				T temp = data[j - 1];
				data[j - 1] = data[j];
				data[j] = temp;
				has_swapped = true;
			}
		}
		if (has_swapped == false)
		{
			//data is sorted, break out of loop
			break;
		}
	}
}

//Quick sort
template <typename T>
void sortHelper(vector<T>& data, int start_index, int end_index)
{
	//array of size 1 or smaller
	if (end_index <= start_index)
	{
		return;
	}

	//array of size 2
	if (end_index - start_index == 1)
	{
		if (data[end_index] < data[start_index])
		{
			T temp = data[end_index];
			data[end_index] = data[start_index];
			data[start_index] = temp;
		}
		return;
	}

	//must be size 3 or larger

	//find pivot
	T first_item = data[start_index];
	T last_item = data[end_index];
	int mid_index = (start_index + end_index) / 2;
	T middle_item = data[mid_index];
	int pivot_index = start_index;


	if (
		middle_item > first_item && middle_item < last_item //ex: 1 5 10
		||
		middle_item < first_item && middle_item > last_item //ex: 10 5 1
		)
	{
		pivot_index = mid_index;
	}
	else if (
		last_item > first_item && last_item < middle_item //ex: 1 10 5
		||
		last_item < first_item && last_item > middle_item //ex: 10 1 5
		)
	{
		pivot_index = end_index;
	}

	//swap pivot with end index
	T pivot_value = data[pivot_index];
	data[pivot_index] = data[end_index];
	data[end_index] = pivot_value;

	/*
	1. Define i = front_index; j = end_index - 1;
	2. While data[i] < pivot AND i < j
	a. i++
	3. While data[j] > pivot and i < j
	a. j--
	4. if i != j
	a. Swap(data[i], data[j])
	b. GOTO #2
	*/
	int i = start_index;
	int j = end_index - 1;
	while (i < j)
	{
		while (data[i] < pivot_value && i < j)
		{
			i++;
		}
		while (data[j] >= pivot_value && i < j)
		{
			j--;
		}
		if (i < j)
		{
			T temp = data[i];
			data[i] = data[j];
			data[j] = temp;
		}
	}

	//swap pivot back
	T temp = data[i];
	data[i] = pivot_value;
	data[end_index] = temp;

	//recursively repeat
	sortHelper(data, start_index, i - 1);
	sortHelper(data, i + 1, end_index);

}

template <typename T>
void quickSort(vector<T>& data)
{
	sortHelper(data, 0, data.size() - 1);
}

void saveToBinaryFile(vector<vector<int>> data)
{
	unordered_map<string, int> wordFrequency;
	unordered_map<string, string> finalWord;
	vector<string> englishWords;
	priority_queue<pair<string, int>> sort;
	int counter = 1;

	string word = "";

	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j] == ' ')
			{
				wordFrequency[word]++;
				englishWords.push_back(word);
				word = "";
			}
			else if (data[i][j] != -1)
			{
				word += data[i][j];
			}
		}
	}
	wordFrequency[word]++;
	englishWords.push_back(word);
	word = "";

	for (auto pair : wordFrequency)
	{
		sort.push(pair);
	}

	while (!sort.empty())
	{
		string word = sort.top().first;

		finalWord[word] = decimalConvert(counter++);

		sort.pop();
	}

	ofstream binaryFile;
	binaryFile.open("binaryFile.txt");
	if (binaryFile.is_open())
	{
		for (auto pair : wordFrequency)
		{
			binaryFile << pair.first << " : " << pair.second << endl;
		}
	}

	binaryFile.close();

	ofstream binaryFileNew;
	binaryFileNew.open("binaryFileNew.txt");
	if (binaryFileNew.is_open())
	{
		for (auto word : englishWords)
		{
			binaryFileNew << finalWord[word] << " ";
		}
	}

	binaryFileNew.close();
}

string decimalConvert(int n)
{
	vector<int> a;
	string word = "";
	for (int i = 0; n > 0; i++)
	{
		a.push_back(n % 2);
		n /= 2;
	}

	for (auto characters : a)
	{
		word += to_string(characters);
	}

	return word;
}