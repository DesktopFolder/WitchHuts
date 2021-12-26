#pragma once
/**
 * A simple argument parsing toolkit.
 *
 * Header-only, not because it's more efficient, but because I don't care.
 */

#include <cassert>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

/* A few basic algorithms, because we can */
// This should use sfinae/concepts for a few specializations to avoid references.
// Or to use references for values if they're heavy.
template <typename C, typename V>
[[nodiscard]] bool contains(const C& container, V value)
{
    // Simple shortcircuiting implementation
    for (auto v : container)
        if (value == v) return true;
    return false;
}

[[nodiscard]] std::string_view lstrip(const std::string& str, std::string_view to_strip)
{
    // Apparently the appropriate constructor is 20 only and my compiler doesn't ship it.
    // Or something. I dunno. Whatever. We'll do this the obscenely ugly way.
    auto begin = std::find_if(str.begin(), str.end(),
                              [to_strip](char c) { return not contains(to_strip, c); });
    auto end   = str.end();
    return std::string_view{str}.substr(begin - str.begin(), end - begin);
}

[[nodiscard]] bool starts_with(const std::string& str, char c)
{
    return not str.empty() && str[0] == c;
}

struct Argument
{
    const std::string name;
    std::vector<std::string> values;

    [[nodiscard]] std::string_view name_clean() const { return lstrip(name, "-"); }

    [[nodiscard]] bool empty() const { return name.empty() && values.empty(); }
};

struct ArgumentParser
{
    [[nodiscard]] bool done() const { return pos == args.size(); }

    [[nodiscard]] Argument consume()
    {
        if (pos == args.size()) return Argument{};

        // Now... consume.
        // Assume this is a proper argument.
        // Don't allow = for now.
        // We're not supporting features. That's not fun!
        Argument ret{args[pos], {}};

        assert(not ret.name.empty());

        pos++;

        if (starts_with(ret.name, '-'))
        {
            // We can also gather arguments.
            while (pos < args.size())
            {
                if (not starts_with(args[pos], '-'))
                {
                    ret.values.push_back(args[pos]);
                    pos++;
                }
                else
                    break;
            }
        }

        return ret;
    }

    ArgumentParser(int argc, char* argv[]) : args(argv + 1, argv + argc) {}

private:
    size_t pos{};
    std::vector<std::string> args;
};
