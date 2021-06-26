#include "Tool.h"

using namespace std;

int StringToInt(const std::string &src_str)
{
  istringstream ss(src_str);
  int value = -INT_MAX;
  ss >> value;
  return value;
}

std::list<std::string> SplitWord(const std::string &line)
{
  istringstream ss(line);
  string tmp_word;
  list<string> list;
  while (ss >> tmp_word)
  {
    list.push_back(tmp_word);
  }
  return list;
}
