# Iterator Tools
[![Build Status](https://github-drone.code-ape.dev/api/badges/TumbleOwlee/itertools-cxx/status.svg?ref=refs/heads/main)](https://github-drone.code-ape.dev/TumbleOwlee/itertools-cxx)

This is a collection of iterator templates to perform interator chaining in C++. This is by far no proven solution. It's just a personal playground and thus doesn't provide anything special and may be broken. Please be aware and look somewhere else for actively maintained libraries that provide a more fully featured implementation.

## Iterator Chaining
This header library provides the functionality to perform chained operations on a any given container. The only requirement is that the container provides access to the beginning and end of the container via the `.begin()` and `.end()` functions. All chained operation are only applied if `.next()`, `.collectInsert()` (for containers with support of `.insert()`) or `.collectPush()` (for containers with support of `.push_back()`) is called.

## Supported Functionality
* [X] map : Map the given values of the iterator into another type/value
* [X] filter : Filter the iterator based on a lamda function
* [X] zip : Combine two iterators into one to iterate over pairs of both
* [X] enumerate : Iterate over values and their indices
* [X] product : Create product of all values based on operator*
* [X] sum : Create sum of all values based on operator+

## Usage
```C++
// Create any container providing .begin() and .end(), e.g. std::vector
std::vector<int> values = { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46 };

// Now we can create the iterator wrapper from the container
auto iterator = itertools::Iterator::from(values);

// .. and apply any of the supported chaining operations

// map all values to char
auto iterator = itertools::Iterator::from(values).map<char>([](int &i) { return static_cast<char>(i); })

// filter all even values
auto iterator = itertools::Iterator::from(values).filter([](int &i) { return i % 2 == 0; })

// zip two iterators (note: the .into() call is necessary for the type change)
auto iterator = itertools::Iterator::from(v2).zip<char>(itertools::Iterator::from(v3).into())

// or chain everything
auto iterator = itertools::Iterator::from(values)
                    .filter([](int &i) { return i % 2 == 0; })
                    .map<char>([](int &i) { return static_cast<char>(i); })
                    .zip<char>(itertools::Iterator::from(v3).into())
```
