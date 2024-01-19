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

#ifndef _ITERTOOLS_ITERATOR_HXX_
#define _ITERTOOLS_ITERATOR_HXX_

#include "optional.hxx"
#include "types.hxx"
#include <functional>
#include <iostream>
#include <memory>

namespace itertools {

    /**
     * @brief Provides static methods to create iterators
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
        static IteratorWrapper<BasicIterator<Container>, typename Container::value_type> from(Container &container) {
            return IteratorWrapper<BasicIterator<Container>, typename Container::value_type>(
                std::make_shared<BasicIterator<Container>>(container));
        }
    };

    /**
     * @brief Wrapper class for iterator tools
     * @details This class is used to wrap iterators to make them usable without the need of shared_ptr handling.
     * @typeparam IteratorType Iterator type
     * @typeparam OutputType Output type of the iterator
     */
    template <typename IteratorType, typename OutputType>
    class IteratorWrapper {
      public:
        /**
         * @brief Create new iterator wrapper from shared_ptr
         * @param iterator Iterator to wrap
         * @return New iterator wrapper
         */
        IteratorWrapper(std::shared_ptr<IteratorType> iterator) : m_iterator(iterator) {}

        /**
         * @brief Create zipped iterator from this and another iterator
         * @typeparam OtherOutputType Output type of the other iterator
         * @param other Iterator to zip with
         * @return Zipped iterator
         */
        template <typename OtherOutputType>
        IteratorWrapper<ZipIterator<OutputType, OtherOutputType>, std::pair<OutputType, OtherOutputType>>
        zip(IteratorWrapper<IIterator<OtherOutputType>, OtherOutputType> other) {
            return IteratorWrapper<ZipIterator<OutputType, OtherOutputType>, std::pair<OutputType, OtherOutputType>>(
                m_iterator->zip(other.inner()));
        }

        /**
         * @brief Create enumerated iterator from this
         * @return Enumerated iterator
         */
        IteratorWrapper<EnumerateIterator<OutputType>, std::pair<size_t, OutputType>> enumerate() {
            return IteratorWrapper<EnumerateIterator<OutputType>, std::pair<size_t, OutputType>>(m_iterator->enumerate());
        }

        /**
         * @brief Create filtered iterator from this and a filter
         * @param filter Filter to apply
         * @return Filtered iterator
         */
        IteratorWrapper<FilterIterator<OutputType>, OutputType> filter(std::function<bool(OutputType &)> filter) {
            return IteratorWrapper<FilterIterator<OutputType>, OutputType>(m_iterator->filter(filter));
        }

        /**
         * @brief Create mapped iterator from this and a map
         * @typeparam OtherOutputType Output type of the mapped values
         * @param map Mapping to apply to each element
         * @return Mapped iterator
         */
        template <typename OtherOutputType>
        IteratorWrapper<MapIterator<OutputType, OtherOutputType>, OtherOutputType>
        map(std::function<OtherOutputType(OutputType &)> map) {
            return IteratorWrapper<MapIterator<OutputType, OtherOutputType>, OtherOutputType>(m_iterator->map(map));
        }

        /**
         * @brief Transform this wrapper into wrapper of iterator interface
         * @return Wrapper of iterator interface
         */
        IteratorWrapper<IIterator<OutputType>, OutputType> into() {
            return IteratorWrapper<IIterator<OutputType>, OutputType>(m_iterator);
        }

        /**
         * @brief Print all elements of the filtered container
         */
        void print() {
            std::cout << "{ ";
            Option<OutputType> opt = m_iterator->next();
            while (opt.isSome()) {
                std::cout << opt.get() << ", ";
                opt = m_iterator->next();
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
            Option<OutputType> opt = m_iterator->next();
            while (opt.isSome()) {
                c.insert(opt.get());
                opt = m_iterator->next();
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
            Option<OutputType> opt = m_iterator->next();
            while (opt.isSome()) {
                c.push_back(opt.get());
                opt = m_iterator->next();
            }
            return c;
        }

        /**
         * @brief Return reference to inner shared_ptr
         * @return Reference to inner shared_ptr
         */
        std::shared_ptr<IteratorType> inner() { return m_iterator; }

      private:
        // Shared reference to the iterator
        std::shared_ptr<IteratorType> m_iterator;
    };

} // namespace itertools

#endif
