#include <algorithm>
#include <iostream>
#include <iterator>

#include "RingBuffer.h"

using std::cout;
using std::endl;

int main()
{
    RingBuffer<int, 10> rb;

    for (size_t i = 0; i < 13; i++)
    {
        cout << "pushing: " << i << " size: " << rb.size() << endl;
        rb.push_back(i);
    }

    while (!rb.empty())
    {
        cout << "poping: " << rb.head() << endl;
        rb.pop();
    }

    {
        cout << rb << endl;
        cout << "Inserting with back_inserter_iterator -->>" << endl;
        auto it = std::back_insert_iterator<decltype(rb)>(rb);

        for (int i = 0; i < 13; ++i)
        {
            cout << "pushing: " << i << " size: " << rb.size() << endl;
            *it = i * 10;
        }
        cout << rb << endl;
        std::copy(begin(rb), end(rb), std::ostream_iterator<int>(cout, ","));
        cout << endl;
    }

    {
        cout << rb << endl;
        cout << "Inserting with decltype(rb)::iterator -->>" << endl;
        decltype(rb)::iterator it = begin(rb); // ditto as above

        for (int i = 0; i < 13; ++i)
        {
            cout << "pushing: " << i << " size: " << rb.size() << endl;
            *it = i * 10;
        }
        cout << rb << endl;
        std::copy(begin(rb), end(rb), std::ostream_iterator<int>(cout, ","));
        cout << endl;
    }

    auto sum = 0;
    for_each(begin(rb), end(rb), [&](auto v) {
        sum += v;
        cout << v << " ";
    });

    cout << endl;
    cout << "sum is " << sum << endl;

    while (!rb.empty())
    {
        cout << "popping: " << rb.head() << "\n";
        rb.pop();
    }

    return 0;
}