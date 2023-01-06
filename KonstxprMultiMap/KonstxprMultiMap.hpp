#include <array>
#include <optional>
#include <algorithm>
#include <ranges>


namespace kmp
{
    template<typename T>
    concept HasLessThan = requires(T a, T b) {
        { a < b } -> std::convertible_to<bool>;
    };

    template<typename Key>
    concept KeyType = std::equality_comparable<Key> && HasLessThan<Key>;


    template <KeyType Key, typename Value, std::size_t SIZE>
    class KonstxperMultiMap
    {
    public:
        explicit constexpr KonstxperMultiMap() = delete;

        template<std::same_as<std::pair<Key, Value>>... KVs>
        explicit constexpr KonstxperMultiMap(KVs&&... kvs) noexcept;

        [[nodiscard]] consteval std::size_t size() const noexcept;

        // in most cases sizeof(Key) is small,
        // so we'll pass key by value rather than by reference

        [[nodiscard]] constexpr std::optional<Value> at(const Key key) const noexcept;

        [[nodiscard]] constexpr bool contains(const Key key) const noexcept;

        [[nodiscard]] constexpr std::optional<Value> lowerBound(const Key key) const noexcept;

        [[nodiscard]] constexpr std::optional<Value> upperBound(const Key key) const noexcept;

        [[nodiscard]] constexpr auto equalRange(const Key key) const noexcept;

        [[nodiscard]] constexpr std::size_t count(const Key key) const noexcept;

    private:
        std::array<std::pair<Key, Value>, SIZE> arr_;

        [[nodiscard]] constexpr std::size_t find_first_of(const Key key) const noexcept;
        [[nodiscard]] constexpr std::size_t find_last_of(const Key key) const noexcept;
    };

    template <KeyType Key, typename Value, std::size_t SIZE>
    template<std::same_as<std::pair<Key, Value>> ... KVs>
    constexpr KonstxperMultiMap<Key, Value, SIZE>::KonstxperMultiMap(KVs&&... kvs) noexcept
        : arr_{ kvs... }
    {
        std::ranges::sort(arr_);
    }

    template <KeyType Key, typename Value, std::size_t SIZE>
    constexpr std::size_t KonstxperMultiMap<Key, Value, SIZE>::find_first_of(const Key key) const noexcept
    {
        // for small contiguous containers linear search might be faster than binary search
        // so throughout this class we'll use linear search rather than binary search

        const auto begin{ std::cbegin(arr_) };
        const auto end{ std::cend(arr_) };

        auto it{ begin };
        for (; it != end; ++it)
        {
            if (it->first == key)
            {
                return it - begin;
            }
        }

        return SIZE;
    }

    template <KeyType Key, typename Value, std::size_t SIZE>
    constexpr std::size_t KonstxperMultiMap<Key, Value, SIZE>::find_last_of(const Key key) const noexcept
    {
        const auto rend{ std::crend(arr_) };
        auto it{ std::crbegin(arr_) };
        for (; it != rend; ++it)
        {
            if (it->first == key)
            {
                return rend - it;
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
    constexpr std::optional<Value> KonstxperMultiMap<Key, Value, SIZE>::at(const Key key) const noexcept
    {
        const std::size_t firstIdx{ find_first_of(key) };
        if (firstIdx != SIZE) [[likely]]
        {
            return { arr_[firstIdx].second };
        }
        else [[unlikely]]
        {
            return { };
        }
    }

    template <KeyType Key, typename Value, std::size_t SIZE>
    constexpr bool KonstxperMultiMap<Key, Value, SIZE>::contains(const Key key) const noexcept
    {
        return at(key).has_value();
    }

    template <KeyType Key, typename Value, std::size_t SIZE>
    constexpr std::optional<Value> KonstxperMultiMap<Key, Value, SIZE>::lowerBound(const Key key) const noexcept
    {
        return at(key);
    }

    template <KeyType Key, typename Value, std::size_t SIZE>
    constexpr std::optional<Value> KonstxperMultiMap<Key, Value, SIZE>::upperBound(const Key key) const noexcept
    {
        const std::size_t lastIdx{ find_last_of(key) };
        if (lastIdx != SIZE) [[likely]]
        {
            return { arr_[lastIdx].second };
        }
        else [[unlikely]]
        {
            return { };
        }
    }

    template <KeyType Key, typename Value, std::size_t SIZE>
    constexpr auto KonstxperMultiMap<Key, Value, SIZE>::equalRange(const Key key) const noexcept
    {
        return std::ranges::subrange(arr_.cbegin() + find_first_of(key), arr_.cbegin() + find_last_of(key)) | std::views::values;
    }

    template <KeyType Key, typename Value, std::size_t SIZE>
    constexpr std::size_t KonstxperMultiMap<Key, Value, SIZE>::count(const Key key) const noexcept
    {
        return equalRange(key).size();
    }
}