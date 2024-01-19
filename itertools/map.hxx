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
#include <memory>

namespace itertools {
    /**
     * @brief Mapped iterator over the elements of an iterator.
     * @typeparam InputType The type of the elements of the input iterator.
     * @typeparam OutputType The type of the elements of the output iterator.
     */
    template <typename InputType, typename OutputType>
    class MapIterator : public IIterator<OutputType>,
                        public std::enable_shared_from_this<MapIterator<InputType, OutputType>> {
      public:
        /**
         * @brief Construct a new MapIterator object.
         * @param map The mapping function.
         * @param parent The parent iterator.
         */
        MapIterator(std::function<OutputType(InputType &)> map, std::shared_ptr<IIterator<InputType>> parent)
            : m_parent(parent), m_map(map) {}

        /**
         * @brief Get the next element in the mapped container
         * @return Next element in the mapped container or None if the end is reached
         */
        Option<OutputType> next() override {
            if (m_parent) {
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
         * @brief Zip iterator with another iterator
         * @typeparam OtherOutputType Output type of the other iterator
         * @param IteratorType Iterator to zip with.
         * @return Zip iterator
         */
        template <typename OtherOutputType>
        std::shared_ptr<ZipIterator<OutputType, OtherOutputType>>
        zip(std::shared_ptr<IIterator<OtherOutputType>> second) {
            return std::make_shared<ZipIterator<OutputType, OtherOutputType>>(this->shared_from_this(), second);
        }

        /**
         * @brief Apply mapping iterator to filtered elements
         * @typeparam OuterOutputType Type of the mapped elements
         * @param map Mapping function
         * @return Mapped iterator
         */
        template <typename NextOutputType>
        std::shared_ptr<MapIterator<OutputType, NextOutputType>> map(std::function<NextOutputType(OutputType &)> map) {
            return std::make_shared<MapIterator<OutputType, NextOutputType>>(map, this->shared_from_this());
        }

        /**
         * @brief Apply filtering iterator to filtered elements
         * @param filter Filter function
         * @return Filtered iterator
         */
        std::shared_ptr<FilterIterator<OutputType>> filter(std::function<bool(OutputType &)> filter) {
            return std::make_shared<FilterIterator<OutputType>>(filter, this->shared_from_this());
        }

        /**
         * @brief Enumerate iterator
         * @return Enumerated iterator
         */
        std::shared_ptr<EnumerateIterator<OutputType>> enumerate() {
            return std::make_shared<EnumerateIterator<OutputType>>(this->shared_from_this());
        }

      private:
        // Parent iterator
        std::shared_ptr<IIterator<InputType>> m_parent;
        // Mapping function
        std::function<OutputType(InputType &)> m_map;
    };

} // namespace itertools

#endif
