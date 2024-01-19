#include "itertools.hxx"
#include <map>
#include <vector>

int main(int argc, char *argv[]) {
    std::vector<int> v1 = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46};
    std::vector<char> v2 = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    std::vector<char> v3 = {'A', 'X', 'C', 'X', 'E', 'X', 'G'};

    // Test Case 1
    // Create an zipped iterator and apply enumeration.
    auto result1 = itertools::Iterator::from(v1)
        .zip<char>(itertools::Iterator::from(v2).into())
        .enumerate()
        .collectPush<std::vector<std::pair<size_t, std::pair<int, char>>>>();

    assert(result1.size() == v1.size());
    for (size_t i = 0; i < result1.size(); ++i) {
        assert(result1[i].first == i);
        assert(result1[i].second.first == v1[i]);
        assert(result1[i].second.second == v2[i]);
    }

    // Test Case 2
    // Create an zipped iterator and zip it with another iterator
    auto result2 = itertools::Iterator::from(v1)
        .zip<char>(itertools::Iterator::from(v2).into())
        .zip<char>(itertools::Iterator::from(v3).into())
        .collectPush<std::vector<std::pair<std::pair<char, char>, char>>>();

    assert(result2.size() == v1.size());
    for (size_t i = 0; i < result2.size(); ++i) {
        assert(result2[i].first.first == v1[i]);
        assert(result2[i].first.second == v2[i]);
        assert(result2[i].second == v3[i]);
    }

    // Test Case 3
    // Create an enumerated iterator and map it to another value
    auto result3 = itertools::Iterator::from(v1)
        .zip<char>(itertools::Iterator::from(v2).into())
        .map<char>([](std::pair<int, char> &val) { return static_cast<char>(val.second); })
        .collectPush<std::vector<char>>();

    assert(result3.size() == v1.size());
    for (size_t i = 0; i < result3.size(); ++i) {
        assert(result3[i] == v2[i]);
    }

    // Test Case 4
    // Create an enumerated iterator and filter it
    auto result4 = itertools::Iterator::from(v2)
        .zip<char>(itertools::Iterator::from(v3).into())
        .filter([](std::pair<char, char> &val) { return val.first == val.second; })
        .collectPush<std::vector<std::pair<char, char>>>();

    assert(result4.size() == 4U);
    for (size_t i = 0; i < result4.size(); ++i) {
        assert(result4[i].first == result4[i].second);
    }

    // END
    return 0;
}
