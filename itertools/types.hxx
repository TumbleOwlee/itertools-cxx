// MIT License
//
// Copyright (c) 2022 David Loewe
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _ITERTOOLS_TYPES_HXX_
#define _ITERTOOLS_TYPES_HXX_

#include "optional.hxx"

namespace itertools {
    /**
     * @brief Basic iterator interface
     * @typeparam ValueType The type of the elements
     */
    template <typename ValueType>
    class IIterator {
      public:
        virtual Option<ValueType> next() = 0;
        virtual ~IIterator() = default;
    };

    /**
     * @brief Provides static methods to create iterators
     */
    class Iterator;

    /**
     * @brief Wrapper class for iterator tools
     * @details This class is used to wrap iterators to make them usable without the need of shared_ptr handling.
     * @typeparam IteratorType Iterator type
     * @typeparam OutputType Output type of the iterator
     */
    template <typename IteratorType, typename OutputType>
    class IteratorWrapper;

    /**
     * @brief Basic iterator class for iterating over a container.
     * @typeparam C Container type.
     */
    template <typename Container>
    class BasicIterator;

    /**
     * @brief Iterator to iterate over a filtered container
     * @typeparam ValueType Type of the filter values
     */
    template <typename ValueType>
    class FilterIterator;

    /**
     * @brief Iterator to iterate over container and map each value
     * @typeparam InputType Type of the map input values
     * @typeparam OutputType Type of the map output values
     */
    template <typename InputType, typename OutputType>
    class MapIterator;

    /**
     * @brief Iterator to iterate over two iterators simultaneously
     * @typeparam FirstOutputType Type of the first iterator output type
     * @typeparam SecondOutputType Type of the second iterator output type
     */
    template <typename FirstOutputType, typename SecondOutputType>
    class ZipIterator;

    /**
     * @brief Iterator to iterate over an enumerated iterator
     * @typeparam OutputType Type of the input iterator type
     */
    template <typename OutputType>
    class EnumerateIterator;

    /**
     * @brief Wrapper for iterator handling
     * @typeparam IteratorType Type of the iterator
     * @typeparam OutputType Type of the iterator output
     */
    template <typename IteratorType, typename OutputType>
    class IteratorWrapper;

} // namespace itertools

#endif
