#pragma once

namespace pds::bloomFilter
{
    template <typename T>
    LinearCounter<T>::LinearCounter()
        : _m(BIT_ARRAY_SIZE), _count(0), _bitArray(new std::bitset<BIT_ARRAY_SIZE>()) {}

    template <typename T>
    LinearCounter<T>::~LinearCounter()
    {
        delete _bitArray;
    }

    template <typename T>
    void LinearCounter<T>::init(size_t bitmapSize)
    {
        _m = bitmapSize;
        _count = 0;
        _bitArray->reset();
        _visualiser.logAction("[Init] Linear Counter initialized with bitmap size: " + std::to_string(_m));
        _visualiser.logState(*this, std::nullopt, VisualContext::INIT);
    }

    template <typename T>
    void LinearCounter<T>::insert(const T& item)
    {
        size_t hash = _hasher(item);
        size_t idx = hash % _m;
        if (!_bitArray->test(idx))
        {
            ++_count;
            _bitArray->set(idx);
        }

        _visualiser.logAction("[Insert] Item: " + item + " -> Index: " + std::to_string(idx));
        _visualiser.logState(*this, idx, VisualContext::INSERT);
    }

    template <typename T>
    std::optional<float> LinearCounter<T>::estimate() const
    {
        size_t V = _m - _bitArray->count(); // number of zero bits
        if (V == 0)
        {
            _visualiser.logAction("[Estimate] All bits are set. Cannot estimate.");
            return std::nullopt;
        }

        float n = -static_cast<float>(_m) * std::log(static_cast<float>(V) / _m);
        _visualiser.logAction("[Estimate] Unique items estimated: " + std::to_string(n));
        _visualiser.logState(*this, std::nullopt, VisualContext::QUERY);
        return std::make_optional(n);
    }

    template <typename T>
    int32_t LinearCounter<T>::getSize() const
    {
        return _count;
    }

    template <typename T>
    bool LinearCounter<T>::isEmpty() const
    {
        return _count == 0;
    }
}