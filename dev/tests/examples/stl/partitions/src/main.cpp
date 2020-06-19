// TODO add Copyright

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

using namespace std;

int main()
{
  std::string str = "This Is Just An Example Of String With Capital Letters";

  auto predicate = [](const unsigned char v) {
    return isupper(v);
  };

  if (!is_partitioned(begin(str), end(str), predicate))
  {
    cout << "The string is NOT partionated" << endl;
    partition(begin(str), end(str), predicate);
  }
  cout << endl;

  copy(begin(str), end(str), ostream_iterator<unsigned char>(cout, ""));
  cout << endl;

  auto it = partition_point(begin(str), end(str), predicate);

  cout << "True bucket: ";
  copy(begin(str), it, ostream_iterator<unsigned char>(cout, ""));
  cout << endl;
  cout << "False bucket: ";
  copy(it, end(str), ostream_iterator<unsigned char>(cout, ""));
  cout << endl;

  return 0;
}
