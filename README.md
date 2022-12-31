## KonstxprMultiMap
This repository contains a header-only constexpr multimap under 'KonstxprMultiMap/KonstxprMultiMap.hpp'.<br>For some toy examples look at 'KonstxprMultiMap/KonstxprMultiMapTests.cpp'.<br>This generic constexpr multimap can be used for fast compile time retrievals.
#### Some implementation details
Under the hood the multimap is implemented as a std::array of std::pair<Key, Value> where Key and Value are template parameters.<br> It also supports user implemented compare function for keys.