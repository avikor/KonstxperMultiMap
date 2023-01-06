#include "KonstxprMultiMap.hpp"

#include <cassert>
#include <iostream>

consteval void test_size_at_contains_methods()
{
    constexpr kmp::KonstxperMultiMap<char, int, 3U> kMultiMap{ 
        std::pair<char, int>{ 'b' , 1 },
        std::pair<char, int>{ 'a', 0 },
        std::pair<char, int>{ 'c', 2 }
    };

	static_assert(kMultiMap.size() == 3U);

    constexpr std::optional<int> aVal = kMultiMap.at('a');
    static_assert(*aVal == 0);

    constexpr std::optional<int> bVal = kMultiMap.at('b');
    static_assert(*bVal == 1);

    constexpr std::optional<int> cVal = kMultiMap.at('c');
    static_assert(*cVal == 2);

    constexpr std::optional<int> dVal = kMultiMap.at('d');
    static_assert(!dVal.has_value());

    static_assert(kMultiMap.contains('a'));
    static_assert(kMultiMap.contains('b'));
    static_assert(kMultiMap.contains('c'));
    static_assert(!kMultiMap.contains('d'));
}

consteval void test_lower_upper_equal_range_methods()
{
    constexpr kmp::KonstxperMultiMap<char, int, 7U> kMultiMap{
        std::pair<char, int>{ 'a', 0 },
        std::pair<char, int>{ 'a', 1 },
        std::pair<char, int>{ 'b', 2 },
        std::pair<char, int>{ 'b', 3 },
        std::pair<char, int>{ 'b', 4 },
        std::pair<char, int>{ 'c', 5 },
        std::pair<char, int>{ 'd', 6 }
    };

    static_assert(*kMultiMap.lowerBound('a') == 0U);
    static_assert(*kMultiMap.upperBound('a') == 2U);

    static_assert(*kMultiMap.lowerBound('b') == 2U);
    static_assert(*kMultiMap.upperBound('b') == 5U);

    static_assert(*kMultiMap.lowerBound('c') == 5U);
    static_assert(*kMultiMap.upperBound('c') == 6U);

    static_assert(*kMultiMap.lowerBound('d') == 6U);
    static_assert(!kMultiMap.upperBound('d').has_value());

    static_assert(!kMultiMap.lowerBound('g').has_value());
    static_assert(!kMultiMap.upperBound('g').has_value());
}

constexpr void test_equal_range_and_count_methods()
{
    constexpr kmp::KonstxperMultiMap<char, int, 7U> kMultiMap{
        std::pair<char, int>{ 'b', 0 },
        std::pair<char, int>{ 'a', -1 },
        std::pair<char, int>{ 'b', 1 },
        std::pair<char, int>{ 'c', -1 },
        std::pair<char, int>{ 'b', 2 },
        std::pair<char, int>{ 'd', -1 },
        std::pair<char, int>{ 'b', 3 }
    };

    auto bRange = kMultiMap.equalRange('b');
    int bExpectedVal{ 0 };
    for (int i : bRange)
    {
        assert(i == bExpectedVal);
        ++bExpectedVal;
    }
    assert(bExpectedVal == 4);
    assert(kMultiMap.count('b') == 4U);


    auto dRange = kMultiMap.equalRange('d');
    int dExpectedVal{ -1 };
    for (int i : dRange)
    {
        assert(i == dExpectedVal);
        ++dExpectedVal;
    }
    assert(dExpectedVal == 0);
    assert(kMultiMap.count('d') == 1U);


    auto gRange = kMultiMap.equalRange('g');
    assert(gRange.empty());
    assert(kMultiMap.count('g') == 0U);
}

int main()
{
    std::cout << "hello KonstxperMultiMap\n";

    test_size_at_contains_methods();

    test_lower_upper_equal_range_methods();

    test_equal_range_and_count_methods();

	return EXIT_SUCCESS;
}