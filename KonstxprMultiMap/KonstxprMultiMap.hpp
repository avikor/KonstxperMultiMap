﻿#include <array>
#include <optional>
#include <algorithm>
#include <ranges>


template<typename T>
concept HasLessThan = requires(T a, T b) {
    { a < a } -> std::convertible_to<bool>;
    { a < b } -> std::convertible_to<bool>;
};

template<typename Key>
concept KeyType = std::equality_comparable<Key> && HasLessThan<Key>;


template <KeyType Key, typename Value, std::size_t SIZE>
class KonstxperMultiMap
{
public:
    explicit constexpr KonstxperMultiMap() = delete;

    template<typename... KVs>
    explicit constexpr KonstxperMultiMap(KVs&&... kvs) noexcept;

    [[nodiscard]] consteval std::size_t size() const noexcept;

    [[nodiscard]] constexpr std::optional<Value> at(const Key& key) const noexcept;

    [[nodiscard]] constexpr bool contains(const Key& key) const noexcept;
    
    [[nodiscard]] constexpr std::optional<Value> lowerBound(const Key& key) const noexcept;

    [[nodiscard]] constexpr std::optional<Value> upperBound(const Key& key) const noexcept;

    [[nodiscard]] constexpr auto equalRange(const Key& key) const noexcept;

    [[nodiscard]] constexpr std::size_t count(const Key& key) const noexcept;

private:
    std::array<std::pair<Key, Value>, SIZE> m_arr;

    [[nodiscard]] constexpr std::size_t find_first_of(const Key& key) const noexcept;
    [[nodiscard]] constexpr std::size_t find_last_of(const Key& key) const noexcept;
};

template <KeyType Key, typename Value, std::size_t SIZE>
template<typename... KVs>
constexpr KonstxperMultiMap<Key, Value, SIZE>::KonstxperMultiMap(KVs&&... kvs) noexcept
    : m_arr{ kvs... }
{
    std::ranges::sort(m_arr);
}

template <KeyType Key, typename Value, std::size_t SIZE>
constexpr std::size_t KonstxperMultiMap<Key, Value, SIZE>::find_first_of(const Key& key) const noexcept
{
    // for small contiguous containers linear search might be faster than binary search
    // so throughout this class we'll use linear search rather than binary search
    
    auto it{ std::cbegin(m_arr) };
    for (; it != std::cend(m_arr); ++it)
    {
        if (it->first == key)
        {
            return it - std::cbegin(m_arr);
        }
    }

    return SIZE;
}

template <KeyType Key, typename Value, std::size_t SIZE>
constexpr std::size_t KonstxperMultiMap<Key, Value, SIZE>::find_last_of(const Key& key) const noexcept
{
    auto it{ std::crbegin(m_arr) };
    for (; it != std::crend(m_arr); ++it)
    {
        if (it->first == key)
        {
            return std::crend(m_arr) - it;
        }
    }

    return SIZE;
}

template <KeyType Key, typename Value, std::size_t SIZE>
consteval std::size_t KonstxperMultiMap<Key, Value, SIZE>::size() const noexcept
{
    return SIZE;
}

template <KeyType Key, typename Value, std::size_t SIZE>
constexpr std::optional<Value> KonstxperMultiMap<Key, Value, SIZE>::at(const Key& key) const noexcept
{
    if (find_first_of(key) != SIZE)
    {
        return { m_arr[find_first_of(key)].second };
    }
    else
    {
        return { };
    }
}

template <KeyType Key, typename Value, std::size_t SIZE>
constexpr bool KonstxperMultiMap<Key, Value, SIZE>::contains(const Key& key) const noexcept
{
    return at(key).has_value();
}

template <KeyType Key, typename Value, std::size_t SIZE>
constexpr std::optional<Value> KonstxperMultiMap<Key, Value, SIZE>::lowerBound(const Key& key) const noexcept
{
    return at(key);
}

template <KeyType Key, typename Value, std::size_t SIZE>
constexpr std::optional<Value> KonstxperMultiMap<Key, Value, SIZE>::upperBound(const Key& key) const noexcept
{
    if (find_last_of(key) != SIZE)
    {
        return { m_arr[find_last_of(key)].second };
    }
    else
    {
        return { };
    }
}

template <KeyType Key, typename Value, std::size_t SIZE>
constexpr auto KonstxperMultiMap<Key, Value, SIZE>::equalRange(const Key& key) const noexcept
{
    return std::ranges::subrange(m_arr.cbegin() + find_first_of(key), m_arr.cbegin() + find_last_of(key)) | std::views::values;
}

template <KeyType Key, typename Value, std::size_t SIZE>
constexpr std::size_t KonstxperMultiMap<Key, Value, SIZE>::count(const Key& key) const noexcept
{
    return equalRange(key).size();
}