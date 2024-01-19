#include "itertools.hxx"
#include <map>
#include <vector>

int main(int argc, char *argv[]) {
    std::vector<int> v1 = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46};
    std::vector<char> v2 = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    std::vector<char> v3 = {'A', 'X', 'C', 'X', 'E', 'X', 'G'};

    // Test Case 1
    // Create an filtered iterator and apply enumeration again.
    auto result1 = itertools::Iterator::from(v1)
        .filter([](int &val) { return val % 2 != 0; })
        .enumerate()
        .collectPush<std::vector<std::pair<size_t, int>>>();

    assert(result1.size() == 3U);
    for (size_t i = 0; i < result1.size(); ++i) {
        assert(result1[i].first == i);
        assert(result1[i].second == v1[i * 2U]);
    }

    // Test Case 2
    // Create an filtered iterator and zip it with another iterator
    auto result2 = itertools::Iterator::from(v1)
        .filter([](int &val) { return val % 2 != 0; })
        .zip<char>(itertools::Iterator::from(v2).into())
        .collectPush<std::vector<std::pair<int, char>>>();

    assert(result2.size() == 3U);
    for (size_t i = 0; i < result2.size(); ++i) {
        assert(result2[i].first == v1[i * 2]);
        assert(result2[i].second == v2[i]);
    }

    // Test Case 3
    // Create an filtered iterator and map it to another value
    auto result3 = itertools::Iterator::from(v1)
        .filter([](int &val) { return val % 2 != 0; })
        .map<char>([](int &val) { return static_cast<char>(val); })
        .collectPush<std::vector<char>>();

    assert(result3.size() == 3U);
    for (size_t i = 0; i < result3.size(); ++i) {
        assert(result3[i] == v2[i * 2]);
    }

    // Test Case 4
    // Create an filtered iterator and filter it
    auto result4 = itertools::Iterator::from(v1)
        .filter([](int &val) { return val % 2 != 0; })
        .filter([](int &val) { return val % 5 != 0; })
        .collectPush<std::vector<char>>();

    assert(result4.size() == 2U);
    assert(result4[0] == 0x43);
    assert(result4[1] == 0x45);

    // END
    return 0;
}
