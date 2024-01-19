#include "itertools.hxx"
#include <map>
#include <vector>

int main(int argc, char *argv[]) {
    std::vector<int> v1 = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46};
    std::vector<char> v2 = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    std::vector<char> v3 = {'A', 'X', 'C', 'X', 'E', 'X', 'G'};

    // Test Case 1
    // Create an mapped iterator and apply enumeration again.
    auto result1 = itertools::Iterator::from(v1)
        .map<char>([](int &val) { return static_cast<char>(val); })
        .enumerate()
        .collectPush<std::vector<std::pair<size_t, char>>>();

    assert(result1.size() == v1.size());
    for (size_t i = 0; i < result1.size(); ++i) {
        assert(result1[i].first == i);
        assert(result1[i].second == v2[i]);
    }

    // Test Case 2
    // Create an mapped iterator and zip it with another iterator
    auto result2 = itertools::Iterator::from(v1)
        .map<char>([](int &val) { return static_cast<char>(val); })
        .zip<char>(itertools::Iterator::from(v3).into())
        .collectPush<std::vector<std::pair<char, char>>>();

    assert(result2.size() == v1.size());
    for (size_t i = 0; i < result2.size(); ++i) {
        assert(result2[i].first == v2[i]);
        assert(result2[i].second == v3[i]);
    }

    // Test Case 3
    // Create an mapped iterator and map it to another value
    auto result3 = itertools::Iterator::from(v1)
        .map<char>([](int &val) { return static_cast<char>(val); })
        .map<char>([](char &val) { return val == 'B' || val == 'D' || val == 'F' ? 'X' : val; })
        .collectPush<std::vector<char>>();

    assert(result3.size() == v1.size());
    for (size_t i = 0; i < result3.size(); ++i) {
        assert(result3[i] == v3[i]);
    }

    // Test Case 4
    // Create an mapped iterator and filter it
    auto result4 = itertools::Iterator::from(v1)
        .map<char>([](int &val) { return static_cast<char>(val); })
        .filter([](char &val) { return val == 'A' || val == 'B'; })
        .collectPush<std::vector<char>>();

    assert(result4.size() == 2U);
    assert(result4[0] == 'A');
    assert(result4[1] == 'B');

    // END
    return 0;
}
