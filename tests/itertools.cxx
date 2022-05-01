#include "itertools.hxx"
#include <map>
#include <vector>

int main(int argc, char *argv[]) {
    // Test case 1
    std::vector<int> v = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46};
    auto result        = itertools::Iterator::from(v)
                      ->filter([](int &i) { return i % 2 == 0; })
                      ->map<char>([](int &i) { return static_cast<char>(i); })
                      ->collectPush<std::vector<char>>();
    assert(result.size() == 3);
    for (int i = 0; i < result.size(); ++i) {
        assert(result[i] % 2 == 0);
        assert(result[i] == v[2 * i + 1]);
    }
    // Test case 2
    auto map = itertools::Iterator::from(v)
                   ->map<std::pair<int, int>>([](int &i) { return std::pair<int, int>(i, i * 2); })
                   ->collectInsert<std::map<int, int>>();
    assert(map.size() == v.size());
    size_t index = 0;
    for (auto iter = map.begin(); iter != map.end(); ++iter) {
        assert((*iter).second == (*iter).first * 2);
        assert(v[index] == (*iter).first);
        assert(v[index] * 2 == (*iter).second);
        ++index;
    }
    // Test case 3
    std::vector<int> v2  = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46};
    std::vector<char> v3 = {'a', 'b', 'c', 'd', 'e', 'f'};
    itertools::Iterator::from(v2)
        ->map<int>([](int &i) { return i * 2; })
        ->filter([](int &i) { return i % 2 == 0; })
        ->zip<char>(itertools::Iterator::from(v3))
        ->print();
    return 0;
}
