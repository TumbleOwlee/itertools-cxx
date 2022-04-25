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

#ifndef _ITERTOOLS_MAP_HXX_
#define _ITERTOOLS_MAP_HXX_

#include "optional.hxx"
#include "types.hxx"
#include <functional>
#include <iostream>

namespace itertools {
    /**
     * @brief Mapped iterator over the elements of an iterator.
     * @typeparam InputType The type of the elements of the input iterator.
     * @typeparam OutputType The type of the elements of the output iterator.
     * @typeparam ParentType The type of the parent iterator.
     */
    template <typename InputType, typename OutputType, typename ParentType>
    class MapIterator {
      public:
        /**
         * @brief Construct a new MapIterator object.
         * @param map The mapping function.
         * @param parent The parent iterator.
         */
        MapIterator(std::function<OutputType(InputType &)> map, ParentType *parent) : m_parent(parent), m_map(map) {}

        /**
         * @brief Get the next element in the mapped container
         * @return Next element in the mapped container or None if the end is reached
         */
        Option<OutputType> next() {
            if (m_parent != nullptr) {
                Option<InputType> opt = m_parent->next();
                if (opt.isNone()) {
                    return Option<OutputType>();
                } else {
                    return Option<OutputType>(m_map(opt.get()));
                }
            }
            return Option<OutputType>();
        }

        /**
         * @brief Apply mapping iterator to filtered elements
         * @typeparam OuterOutputType Type of the mapped elements
         * @param map Mapping function
         * @return Mapped iterator
         */
        template <typename OuterOutputType>
        MapIterator<OutputType, OuterOutputType, MapIterator<InputType, OutputType, ParentType>>
        map(std::function<OuterOutputType(OutputType &)> map) {
            return MapIterator<OutputType, OuterOutputType, MapIterator<InputType, OutputType, ParentType>>(
                map, new MapIterator<InputType, OutputType, ParentType>(*this));
        }

        /**
         * @brief Apply filtering iterator to filtered elements
         * @param filter Filter function
         * @return Filtered iterator
         */
        FilterIterator<OutputType, MapIterator<InputType, OutputType, ParentType>>
        filter(std::function<bool(OutputType &)> filter) {
            return FilterIterator<OutputType, MapIterator<InputType, OutputType, ParentType>>(filter,
                                                                                              new MapIterator(*this));
        }

        /**
         * @brief Print all elements of the mapped container
         */
        void print() {
            std::cout << "{ ";
            Option<OutputType> opt = next();
            while (opt.isSome()) {
                std::cout << opt.get() << ", ";
                opt = next();
            }
            std::cout << "}" << std::endl;
        }

        /**
         * @brief Collects all values of iterator into a container
         * @typeparam ContainerType Type of the container
         * @return Container with all values of iterator
         */
        template <typename Collection>
        Collection collectInsert() {
            Collection c;
            Option<OutputType> opt = next();
            while (opt.isSome()) {
                c.insert(opt.get());
                opt = next();
            }
            return c;
        }

        /**
         * @brief Collects all values of iterator into a container
         * @typeparam ContainerType Type of the container
         * @return Container with all values of iterator
         */
        template <typename Collection>
        Collection collectPush() {
            Collection c;
            Option<OutputType> opt = next();
            while (opt.isSome()) {
                c.push_back(opt.get());
                opt = next();
            }
            return c;
        }

      private:
        // Parent iterator
        ParentType *m_parent = nullptr;
        // Mapping function
        std::function<OutputType(InputType &)> m_map;
    };
} // namespace itertools

#endif
