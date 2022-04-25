#include <vector>
#include "itertools.hxx"

int main(int argc, char *argv[])
{
    std::vector<int> v = { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46 };
    itertools::Iterator::from(v).filter([](int i) { return i % 2 == 0; }).map<char>([](int i) { return static_cast<char>(i); }).print();
    return 0;
}
