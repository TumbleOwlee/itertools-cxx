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

#ifndef _ITERTOOLS_FILTER_HXX_
#define _ITERTOOLS_FILTER_HXX_

#include "optional.hxx"
#include "types.hxx"
#include <functional>
#include <iostream>
#include <memory>

namespace itertools {
    /**
     * @brief Filtered iterator over the elements of an iterator
     * @typeparam ValueType Type of the elements
     */
    template <typename ValueType>
    class FilterIterator : public IIterator<ValueType>, public std::enable_shared_from_this<FilterIterator<ValueType>> {
      public:
        /**
         * @brief Construct a new FilterIterator object
         * @param filter Filter function
         * @param parent Parent iterator
         */
        FilterIterator(std::function<bool(ValueType &)> filter, std::shared_ptr<IIterator<ValueType>> parent)
            : m_parent(parent), m_filter(filter) {}

        /**
         * @brief Get the next element in the filtered container
         * @return Next element in the filtered container or None if the end is
         * reached
         */
        Option<ValueType> next() override {
            while (m_parent) {
                Option<ValueType> opt = m_parent->next();
                if (opt.isNone()) {
                    return opt;
                } else {
                    if (m_filter(opt.get())) {
                        return opt;
                    }
                }
            }
            return Option<ValueType>();
        }

        /**
         * @brief Zip iterator with another iterator
         * @typeparam OutputType Output type of the other iterator
         * @param IteratorType Iterator to zip with.
         * @return Zip iterator
         */
        template <typename OtherOutputType>
        std::shared_ptr<ZipIterator<ValueType, OtherOutputType>>
        zip(std::shared_ptr<IIterator<OtherOutputType>> second) {
            return std::make_shared<ZipIterator<ValueType, OtherOutputType>>(this->shared_from_this(), second);
        }

        /**
         * @brief Apply mapping iterator to filtered elements
         * @typeparam OutputType Type of the mapped elements
         * @param map Mapping function
         * @return Mapped iterator
         */
        template <typename OutputType>
        std::shared_ptr<MapIterator<ValueType, OutputType>> map(std::function<OutputType(ValueType &)> map) {
            return std::make_shared<MapIterator<ValueType, OutputType>>(map, this->shared_from_this());
        }

        /**
         * @brief Apply filtering iterator to filtered elements
         * @param filter Filter function
         * @return Filtered iterator
         */
        std::shared_ptr<FilterIterator<ValueType>> filter(std::function<bool(ValueType &)> filter) {
            return std::make_shared<FilterIterator<ValueType>>(filter, this->shared_from_this());
        }

      private:
        // Parent iterator
        std::shared_ptr<IIterator<ValueType>> m_parent;
        // Filter function
        std::function<bool(ValueType &)> m_filter;
    };
} // namespace itertools

#endif
