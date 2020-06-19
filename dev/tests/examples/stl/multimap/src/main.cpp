// TODO add Copyright

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

using namespace std;

int main()
{
  cout << "Hello!\n";  
  ifstream input("dev/tests/examples/stl_course/multimap/resources/illiad.txt");
  multimap<string, pair<int, int>> word_positions;
  int line_number = 0;
  int word_in_line = 0;
  string line = "";

  if (!input.is_open())
  {
    cout << "Failure opening the file\n";
    return -1;
  }

  while(!input.eof())
  {
    ++line_number;
    getline(input, line);
    istringstream iss(line);
    string word = "";

    while(!iss.eof())
    {
      ++word_in_line;
      iss >> word;
      // TODO remove trailing non-alphabetic characters
      word_positions.insert(
        make_pair(word, make_pair(line_number, word_in_line)));
    }

    word_in_line = 0;
  }

  for (auto it = word_positions.cbegin(), it2 = it; it != word_positions.cend(); it = it2)
  {
    unsigned int counter = word_positions.count(it->first);

    cout << "\"" << it->first <<  "\" occures " 
         << counter << " times, and is on:" << endl;
         
    for (; it2 != word_positions.cend() && it->first == it2->first; ++it2)
    {
      auto [line, word] = it2->second;
      cout << "\tline " << line << ", position " << word << endl;
    }
  }
  cout << "read " << line_number << " lines of text\n";
  input.close();

  return 0;
}
