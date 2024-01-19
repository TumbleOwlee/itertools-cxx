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

#ifndef _ITERTOOLS_ENUMERATE_HXX_
#define _ITERTOOLS_ENUMERATE_HXX_

#include "types.hxx"
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

namespace itertools {

    /**
     * @brief A enumerate iterator.
     * @typename OutputType Output type of the iterator.
     */
    template <typename OutputType>
    class EnumerateIterator : public IIterator<std::pair<size_t, OutputType>>,
                        public std::enable_shared_from_this<EnumerateIterator<OutputType>> {
      public:
        /**
         * @brief Typedef of enumerated iterator.
         */
        typedef std::pair<size_t, OutputType> OutputPairType;

        /**
         * @brief Constructor of enumerate Iterator.
         * @param parent Iterator of container.
         */
        EnumerateIterator(std::shared_ptr<IIterator<OutputType>> parent)
            : m_index(0), m_parent(parent) {}

        /**
         * @brief Returns the next element of zipped iterator.
         * @return Next element of zipped iterator.
         */
        Option<OutputPairType> next() override {
            while (m_parent) {
                Option<OutputType> p = m_parent->next();
                if (p.isNone()) {
                    return Option<OutputPairType>();
                } else {
                    return Option<OutputPairType>(std::make_pair(m_index++, p.get()));
                }
            }
            return Option<OutputPairType>();
        }

        /**
         * @brief Enumerate an iterator
         * @return Enumerate iterator
         */
        std::shared_ptr<EnumerateIterator<OutputPairType>> enumerate() {
            return std::make_shared<EnumerateIterator<OutputPairType>>(this->shared_from_this());
        }

        /**
         * @brief Zip iterator with this iterator
         * @typeparam OtherOutputType Type of the other iterator
         * @param second Iterator to zip with.
         * @return Zip iterator
         */
        template <typename OtherOutputType>
        std::shared_ptr<ZipIterator<OutputPairType, OtherOutputType>> zip(std::shared_ptr<IIterator<OtherOutputType>> second) {
            return std::make_shared<ZipIterator<OutputPairType, OtherOutputType>>(this->shared_from_this(), second);
        }

        /**
         * @brief Apply mapping iterator to zipped elements
         * @typeparam OutputType Type of the mapped elements
         * @param map Mapping function
         * @return Mapped iterator
         */
        template <typename OtherOutputType>
        std::shared_ptr<MapIterator<OutputPairType, OtherOutputType>> map(std::function<OtherOutputType(OutputPairType &)> map) {
            return std::make_shared<MapIterator<OutputPairType, OtherOutputType>>(map, this->shared_from_this());
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
        // Enumerater
        size_t m_index;
        // Iterator
        std::shared_ptr<IIterator<OutputType>> m_parent;
    };

} // namespace itertools

#endif
