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

#ifndef _ITERTOOLS_OPTIONAL_HXX_
#define _ITERTOOLS_OPTIONAL_HXX_

#include <assert.h>

namespace itertools {
    /**
     * @brief Optional type
     * @typeparam T Type of the optional value
     */
    template <typename ValueType>
    class Option {
        public:
            /**
             * @brief Static None value
             */
            static Option<ValueType> const None;

            /**
             * @brief Construct a new mepty Option object
             */
            Option()
                : m_hasValue(false)
                , m_value() { }

            /**
             * @brief Construct a new Option object from given value
             * @param value The value to be stored in the Option
             */
            Option(ValueType value)
                : m_hasValue(true)
                , m_value(value) { }

            /**
             * @brief Check if the Option has a value
             * @return True if the Option has a value, false otherwise
             */
            bool isSome() {
                return m_hasValue;
            }
            
            /**
             * @brief Check if the Option has no value
             * @return True if the Option has no value, false otherwise
             */
            bool isNone() {
                return !isSome();
            }

            /**
             * @brief Get the value of the Option
             * @return The value of the optional type
             */
            ValueType& get() {
                assert(m_hasValue);
                return m_value;
            }

        private:
            // True if the Option has a value, false otherwise
            bool m_hasValue;
            // The value of the Option
            ValueType m_value;
    };
}

#endif
