#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#include <string>
#include <fstream>

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
    DebugPrinter<true>::instance().print_sequence(filename, first, last);
}

#endif // DEBUGUTILS_H
