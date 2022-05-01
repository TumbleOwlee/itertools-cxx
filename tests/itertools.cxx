#include "itertools.hxx"
#include <map>
#include <vector>

int main(int argc, char *argv[]) {
    // START
    // Test case 1
    // Create a container of integer, filter even numbers, transform to char and collect them.
    std::vector<int> v = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46};
    auto result = itertools::Iterator::from(v)
                      .filter([](int &i) { return i % 2 == 0; })
                      .map<char>([](int &i) { return static_cast<char>(i); })
                      .collectPush<std::vector<char>>();
    assert(result.size() == 3);
    for (int i = 0; i < result.size(); ++i) {
        assert(result[i] % 2 == 0);
        assert(result[i] == v[2 * i + 1]);
    }
    // Test case 2
    // Use container of integer, map to pair values, collect in map.
    auto map = itertools::Iterator::from(v)
                   .map<std::pair<int, int>>([](int &i) { return std::pair<int, int>(i, i * 2); })
                   .collectInsert<std::map<int, int>>();
    assert(map.size() == v.size());
    size_t index = 0;
    for (auto iter = map.begin(); iter != map.end(); ++iter) {
        assert((*iter).second == (*iter).first * 2);
        assert(v[index] == (*iter).first);
        assert(v[index] * 2 == (*iter).second);
        ++index;
    }
    // Test case 3
    // Use two containers, multiply values of first iterator by 2, filter even numbers,
    // combine iterator with second char iterator, map pairs to string
    std::vector<int> v2 = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46};
    std::vector<char> v3 = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    auto result2 = itertools::Iterator::from(v2)
                       .map<int>([](int &i) { return i * 2; })
                       .filter([](int &i) { return i % 2 == 0; })
                       .zip<char>(itertools::Iterator::from(v3).into())
                       .map<std::string>([](std::pair<int, char> &p) {
                           return "{ " + std::to_string(p.first) + ", " + p.second + " }";
                       })
                       .collectPush<std::vector<std::string>>();
    assert(result2.size() == 6);
    std::vector<std::string> expected = {"{ 130, a }", "{ 132, b }", "{ 134, c }",
                                         "{ 136, d }", "{ 138, e }", "{ 140, f }"};
    for (int i = 0; i < result.size(); ++i) {
        assert(result2[i] == expected[i]);
    }
    // END
    return 0;
}
