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

#ifndef _ITERTOOLS_ZIP_HXX_
#define _ITERTOOLS_ZIP_HXX_

#include "types.hxx"
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

namespace itertools {

    /**
     * @brief A zip iterator.
     * @typename FirstOutputType Output type of the first iterator.
     * @typename SecongOutputType Output type of the second iterator.
     */
    template <typename FirstOutputType, typename SecondOutputType>
    class ZipIterator : public IIterator<std::pair<FirstOutputType, SecondOutputType>>,
                        public std::enable_shared_from_this<ZipIterator<FirstOutputType, SecondOutputType>> {
      public:
        /**
         * @brief Typedef of zipped iterator.
         */
        typedef std::pair<FirstOutputType, SecondOutputType> OutputPairType;

        /**
         * @brief Constructor of zipped Iterator.
         * @param first Iterator of first container.
         * @param second Iterator of second container.
         */
        ZipIterator(std::shared_ptr<IIterator<FirstOutputType>> first,
                    std::shared_ptr<IIterator<SecondOutputType>> second)
            : m_first(first), m_second(second) {}

        /**
         * @brief Returns the next element of zipped iterator.
         * @return Next element of zipped iterator.
         */
        Option<OutputPairType> next() override {
            while (m_first && m_second) {
                Option<FirstOutputType> f = m_first->next();
                Option<SecondOutputType> s = m_second->next();
                if (f.isNone() || s.isNone()) {
                    return Option<OutputPairType>();
                } else {
                    return Option<OutputPairType>(std::make_pair(f.get(), s.get()));
                }
            }
            return Option<OutputPairType>();
        }

        /**
         * @brief Zip iterator with another iterator
         * @typeparam IteratorType Type of the other iterator
         * @param IteratorType Iterator to zip with.
         * @return Zip iterator
         */
        template <typename OutputType>
        std::shared_ptr<ZipIterator<OutputPairType, OutputType>> zip(std::shared_ptr<IIterator<OutputType>> second) {
            return std::make_shared<ZipIterator<OutputPairType, OutputType>>(this->shared_from_this(), second);
        }

        /**
         * @brief Apply mapping iterator to zipped elements
         * @typeparam OutputType Type of the mapped elements
         * @param map Mapping function
         * @return Mapped iterator
         */
        template <typename OutputType>
        std::shared_ptr<MapIterator<OutputPairType, OutputType>> map(std::function<OutputType(OutputPairType &)> map) {
            return std::make_shared<MapIterator<OutputPairType, OutputType>>(map, this->shared_from_this());
        }

        /**
         * @brief Apply filtering iterator to zipped elements
         * @param filter Filter function
         * @return Filtered iterator
         */
        std::shared_ptr<FilterIterator<OutputPairType>> filter(std::function<bool(OutputPairType &)> filter) {
            return std::make_shared<FilterIterator<OutputPairType>>(filter, this->shared_from_this());
        }

      private:
        // First iterator
        std::shared_ptr<IIterator<FirstOutputType>> m_first;
        // Second iterator
        std::shared_ptr<IIterator<SecondOutputType>> m_second;
    };

} // namespace itertools

#endif
