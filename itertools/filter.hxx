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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
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

#include "types.hxx"
#include "optional.hxx"
#include <functional>
#include <iostream>

namespace itertools {
    /**
     * @brief Filtered iterator over the elements of an iterator
     * @typeparam ValueType Type of the elements
     * @typeparam ParentType Type of the parent iterator
     */
    template<typename ValueType, typename ParentType>
    class FilterIterator {
        public:
            /**
             * @brief Construct a new FilterIterator object
             * @param filter Filter function
             * @param parent Parent iterator
             */
            FilterIterator(std::function<bool (ValueType&)> filter, ParentType* parent)
                : m_parent(parent), m_filter(filter) { 
                assert(parent != nullptr);
            }

            /**
             * @brief Get the next element in the filtered container
             * @return Next element in the filtered container or None if the end is reached
             */
            Option<ValueType> next() {
                if (m_parent != nullptr) {
                    do {
                        Option<ValueType> opt = m_parent->next();
                        if (opt.isNone()) {
                            return opt;
                        } else {
                            if (m_filter(opt.get())) {
                                return opt;
                            }
                        }
                    } while(true);
                }
                return Option<ValueType>();
            }

            /**
             * @brief Apply mapping iterator to filtered elements
             * @typeparam OutputType Type of the mapped elements
             * @param map Mapping function
             * @return Mapped iterator
             */
            template<typename OutputType>
            MapIterator<ValueType, OutputType, FilterIterator<ValueType, ParentType>> map(std::function<OutputType (ValueType&)> map) {
                return MapIterator<ValueType, OutputType, FilterIterator<ValueType, ParentType>>(map, new FilterIterator(*this));
            }

            /**
             * @brief Apply filtering iterator to filtered elements
             * @param filter Filter function
             * @return Filtered iterator
             */
            FilterIterator<ValueType, FilterIterator<ValueType, ParentType>> filter(std::function<bool (ValueType&)> filter) {
                return FilterIterator<ValueType, FilterIterator<ValueType, ParentType>>(filter, new FilterIterator(*this));
            }

            /**
             * @brief Print all elements of the filtered container
             */
            void print() {
                std::cout << "{ ";
                while (Option<ValueType> opt = next() && opt != Option<ValueType>::None) {
                    std::cout << opt.get() << ", " << std::endl;
                }
                std::cout << "}" << std::endl;
            }

        private:
            // Parent iterator
            ParentType* m_parent = nullptr;
            // Filter function
            std::function<bool (ValueType&)> m_filter;
    };
}

#endif
