/* Copyright 2014 - 2016 Evgenii Moiseenko.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <string>
#include <fstream>

namespace trikSound {

template <bool enabled>
class DebugPrinter
{
public:
    static const DebugPrinter& instance()
    {
        static DebugPrinter<enabled> dprinter;
        return dprinter;
    }

    /**
     * Print sequence of values to file
     */
    template <typename T>
    void print_sequence(const std::string& filename, T first, T last) const
    {
        if (enabled) {
            std::ofstream file;
            file.open(filename);
            if (!file.is_open()) {
                std::cerr << "Cannot open file " << filename << std::endl;
                return;
            }

            for (T i = first; i != last; ++i) {
                file << *i << '\n';
            }
            file.flush();
        }
    }

private:
    DebugPrinter() {}
    DebugPrinter(const DebugPrinter& other);
    DebugPrinter& operator=(const DebugPrinter& other);
};

template <typename T>
void dprint_sequence(const std::string& filename, T first, T last)
{
    DebugPrinter<false>::instance().print_sequence(filename, first, last);
}

}
