#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>


std::string str_tolower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return s;
}

std::set<std::string> find_replacements(const std::set<std::string>& dict, std::string str)
{
	std::set<std::string> res;
	for (size_t i = 0; i < str.length(); ++i) {
		std::string temp = str;
		for (char c = 'a'; c <= 'z'; ++c) {
			temp[i] = c;
			if (dict.contains(temp))
				res.insert(temp);
			temp[i] = str[i];

			temp.insert(i, 1, c);
			if (dict.contains(temp))
				res.insert(temp);
			temp.erase(i, 1);
		}

		temp.erase(i, 1);
		if (dict.contains(temp))
			res.insert(temp);
		temp.insert(i, 1, str[i]);
	}

	std::string temp = str;
	for (char c = 'a'; c <= 'z'; ++c) {
		temp.push_back(c);
		if (dict.contains(temp))
			res.insert(temp);
		temp.pop_back();
	}

	return res;
}


int main()
{
	std::ifstream input("input.txt");
	std::ofstream output("output.txt");
	std::ifstream dict_in("dictionary.txt");
	std::ofstream dict_out("dictionaryNew.txt");

	std::set<std::string> dict;
	std::string buffer;
	while (dict_in >> buffer) {
		dict.insert(buffer);
	}

	int key;
	while (input >> buffer) {
		char punct_mark = 0;
		std::string temp = str_tolower(buffer);
		switch (temp.back())
		{
		case ',':
		case '.':
		case ';':
		case '!':
		case '?':
		case ':':
			punct_mark = temp.back();
			temp.pop_back();
			break;
		default:
			break;
		}
		
		if (!dict.contains(temp)) {
			std::cout << "Unknown word: '" << temp << "'\n"
				<< "What should I do with it?\n"
				<< "1. Add the word to the dictionary\n"
				<< "2. Ignore the word\n"
				<< "3. Replace the word with a similar word from the dictionary\n"
				<< "> ";
			do {
				std::cin >> key;
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				if (key != 1 && key != 2 && key != 3)
					std::cerr << "Incorrect option (must be 1, 2 or 3)\n> ";
			} while (key != 1 && key != 2 && key != 3);

			bool replace = false;
			switch (key)
			{
			case 1:
				dict.insert(temp);
				break;
			case 2:
				break;
			case 3:
				replace = true;
				break;
			}

			if (replace) {
				std::set<std::string> replacements = find_replacements(dict, temp);
				if (replacements.empty()) {
					std::cout << "There are no suitable replacements for the word in the dictionary\n"
						<< "1. Add the word to the dictionary\n"
						<< "2. Ignore the word\n"
						<< "> ";
					do {
						std::cin >> key;
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						if (key != 1 && key != 2)
							std::cerr << "Incorrect option (must be 1 or 2)\n> ";
					} while (key != 1 && key != 2);

					switch (key)
					{
					case 1:
						dict.insert(temp);
						break;
					case 2:
						break;
					}
				}
				else {
					std::cout << "Choose a replacement:\n";
					int ind = 1;
					for (const auto& word : replacements) {
						std::cout << ind << ". " << word << '\n';
						++ind;
					}
					do {
						std::cin >> key;
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						
						if (key < 1 || key > replacements.size())
							std::cerr << "Incorrect option (must be from 1 to " << replacements.size() << ")\n> ";
					} while (key < 1 || key > replacements.size());

					auto it = replacements.begin();
					std::advance(it, key - 1);
					buffer = *it;
					if (punct_mark)
						buffer += punct_mark;
				}
			}
		}
		output << buffer << ' ';
	}
	
	for (const auto& word : dict) {
		dict_out << word << '\n';
	}

	return 0;
}