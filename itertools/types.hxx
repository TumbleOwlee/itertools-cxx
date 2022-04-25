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

#ifndef _ITERTOOLS_TYPES_HXX_
#define _ITERTOOLS_TYPES_HXX_

namespace itertools {
    /**
     * @brief Basic iterator class for iterating over a container.
     * @typeparam C Container type.
     */
    template <typename Container>
    class BasicIterator;

    /**
     * @brief Iterator to iterate over a filtered container
     * @typeparam InputType Type of the filter input values
     * @typeparam OutputType Type of the filter output values
     */
    template <typename InputType, typename OutputType>
    class FilterIterator;

    /**
     * @brief Iterator to iterate over container and map each value
     */
    template <typename T, typename O, typename P>
    class MapIterator;
} // namespace itertools

#endif
