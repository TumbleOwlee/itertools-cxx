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

#ifndef _ITERTOOLS_BASIC_HXX_
#define _ITERTOOLS_BASIC_HXX_

#include "optional.hxx"
#include "types.hxx"
#include <functional>
#include <iostream>

namespace itertools {
    /**
     * @brief Basic iterator class iterating over a container
     * @typeparam Container Container type
     */
    template <typename Container>
    class BasicIterator {
      public:
        /**
         * @brief Value type of the elements in the container
         */
        using ValueType = typename Container::value_type;

        /**
         * @brief Container iterator type
         */
        using ContainerIter = typename Container::iterator;

        /**
         * @brief Create a new iterator from a cntainer
         * @param container Container to iterate over
         */
        BasicIterator(Container &container) : m_begin(nullptr), m_end(nullptr) {
            m_begin = new ContainerIter(container.begin());
            m_end   = new ContainerIter(container.end());
        }

        /**
         * @brief Get the next element in the container
         * @return Next element in the container or None if the end is reached
         */
        Option<ValueType> next() {
            if ((*m_begin) != (*m_end)) {
                Option<ValueType> opt = Option<ValueType>(*(*m_begin));
                ++(*m_begin);
                return opt;
            } else {
                return Option<ValueType>();
            }
        }

        /**
         * @brief Apply filter on the iterated values
         * @param filter Filter to apply
         * @return New iterator with filter applied
         */
        FilterIterator<ValueType, BasicIterator<Container>> filter(std::function<bool(ValueType &)> filter) {
            return FilterIterator<ValueType, BasicIterator<Container>>(filter, new BasicIterator(*this));
        }

        /**
         * @brief Apply map on the iterated values
         * @typeparam OutputType Type of the mapped values
         * @param map Mapping to apply to each element
         * @return New iterator with map applied
         */
        template <typename OutputType>
        MapIterator<ValueType, OutputType, BasicIterator<Container>> map(std::function<OutputType(ValueType &)> map) {
            return MapIterator<ValueType, OutputType, BasicIterator<Container>>(map, new BasicIterator(*this));
        }

        /**
         * @brief Print all elements of the mapped container
         */
        void print() {
            std::cout << "{ ";
            while (Option<ValueType> opt = next() && opt != Option<ValueType>::None) {
                std::cout << opt.get() << ", " << std::endl;
            }
            std::cout << "}" << std::endl;
        }

      private:
        // Start of the container
        ContainerIter *m_begin;
        // End of the container
        ContainerIter const *m_end;
    };

    /**
     * @brief Static methods to create iterators
     */
    class Iterator {
      public:
        /**
         * @brief Create new iterator from a container
         * @typename Container Container type
         * @param container Container to iterate over
         * @return New basic iterator
         */
        template <typename Container>
        static BasicIterator<Container> from(Container &container) {
            return BasicIterator<Container>(container);
        }
    };
} // namespace itertools

#endif
