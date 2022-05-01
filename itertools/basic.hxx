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
#include <memory>

namespace itertools {
    /**
     * @brief Basic iterator class iterating over a container
     * @typeparam Container Container type
     */
    template <typename Container>
    class BasicIterator : public IIterator<typename Container::value_type>,
                          public std::enable_shared_from_this<BasicIterator<Container>> {
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
        BasicIterator(Container &container) : m_begin(container.begin()), m_end(container.end()) {}

        /**
         * @brief Delete the BasicIterator object
         */
        ~BasicIterator() {}

        /**
         * @brief Get the next element in the container
         * @return Next element in the container or None if the end is reached
         */
        Option<ValueType> next() override {
            if (m_begin != m_end) {
                Option<ValueType> opt = Option<ValueType>(*m_begin);
                ++m_begin;
                return opt;
            } else {
                return Option<ValueType>();
            }
        }

        /**
         * @brief Zip iterator with another iterator
         * @typeparam OutputType Output type of the other iterator
         * @param other Iterator to zip with.
         * @return Zip iterator
         */
        template <typename OutputType>
        std::shared_ptr<ZipIterator<ValueType, OutputType>> zip(std::shared_ptr<IIterator<OutputType>> other) {
            return std::make_shared<ZipIterator<ValueType, OutputType>>(this->shared_from_this(), other);
        }

        /**
         * @brief Apply filter on the iterated values
         * @param filter Filter to apply
         * @return New iterator with filter applied
         */
        std::shared_ptr<FilterIterator<ValueType>> filter(std::function<bool(ValueType &)> filter) {
            return std::make_shared<FilterIterator<ValueType>>(filter, this->shared_from_this());
        }

        /**
         * @brief Apply map on the iterated values
         * @typeparam OutputType Type of the mapped values
         * @param map Mapping to apply to each element
         * @return New iterator with map applied
         */
        template <typename OutputType>
        std::shared_ptr<MapIterator<ValueType, OutputType>> map(std::function<OutputType(ValueType &)> map) {
            return std::make_shared<MapIterator<ValueType, OutputType>>(map, this->shared_from_this());
        }

      private:
        // Start of the container
        ContainerIter m_begin;
        // End of the container
        ContainerIter const m_end;
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
