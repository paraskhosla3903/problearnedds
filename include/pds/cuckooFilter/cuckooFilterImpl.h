#pragma once

namespace pds::bloomFilter
{

    template <typename T>
    CuckooFilter<T>::CuckooFilter()
        : _numBuckets(0), _entriesPerBucket(0), _fingerprintSize(0), _maxKicks(0),
        _count(0), _table(nullptr) {}

    template <typename T>
    CuckooFilter<T>::~CuckooFilter()
    {
        delete _table;
    }

    template <typename T>
    void CuckooFilter<T>::init(size_t numBuckets, size_t entriesPerBucket, size_t fingerprintSize, size_t maxKicks)
    {
        _numBuckets = numBuckets;
        _entriesPerBucket = entriesPerBucket;
        _fingerprintSize = fingerprintSize;
        _maxKicks = maxKicks;
        _count = 0;

        _table = new std::vector<Bucket>(_numBuckets, Bucket(_entriesPerBucket, 0));
        _visualiser.logAction("[Init] Cuckoo Filter initialized.");
        _visualiser.logState(*this, std::nullopt, VisualContext::INIT);
    }

    template <typename T>
    uint8_t CuckooFilter<T>::fingerprint(const T& item) const
    {
        return static_cast<uint8_t>(_hasher(item) & ((1 << _fingerprintSize) - 1)) + 1; // non-zero fingerprint
    }

    template <typename T>
    size_t CuckooFilter<T>::hash1(const T& item) const
    {
        return _hasher(item) % _numBuckets;
    }

    template <typename T>
    size_t CuckooFilter<T>::hash2(size_t h1, uint8_t fp) const
    {
        return (h1 ^ _hasher(std::to_string(fp))) % _numBuckets;
    }

    template <typename T>
    bool CuckooFilter<T>::insert(const T& item)
    {
        uint8_t fp = fingerprint(item);
        size_t i1 = hash1(item);
        size_t i2 = hash2(i1, fp);

        for (size_t i : {i1, i2})
        {
            for (auto& slot : (*_table)[i])
            {
                if (slot == 0)
                {
                    slot = fp;
                    _count++;
                    _visualiser.logAction("[Insert] " + item);
                    _visualiser.logState(*this, i, VisualContext::INSERT);
                    return true;
                }
            }
        }

        size_t index = (rand() % 2 == 0) ? i1 : i2;
        uint8_t curFp = fp;

        for (size_t n = 0; n < _maxKicks; ++n)
        {
            size_t victimIdx = rand() % _entriesPerBucket;
            std::swap(curFp, (*_table)[index][victimIdx]);
            index = hash2(index, curFp);

            for (auto& slot : (*_table)[index])
            {
                if (slot == 0)
                {
                    slot = curFp;
                    _count++;
                    _visualiser.logAction("[Insert Kick] " + item);
                    _visualiser.logState(*this, index, VisualContext::INSERT);
                    return true;
                }
            }
        }

        _visualiser.logAction("[Insert Failed] " + item);
        return false;
    }

    template <typename T>
    std::optional<bool> CuckooFilter<T>::query(const T& item) const
    {
        uint8_t fp = fingerprint(item);
        size_t i1 = hash1(item);
        size_t i2 = hash2(i1, fp);

        for (uint8_t slot : (*_table)[i1])
        {
            if (slot == fp)
            {
                _visualiser.logAction("[Query Hit] " + item);
                _visualiser.logState(*this, i1, VisualContext::QUERY);
                return true;
            }
        }

        for (uint8_t slot : (*_table)[i2])
        {
            if (slot == fp)
            {
                _visualiser.logAction("[Query Hit] " + item);
                _visualiser.logState(*this, i2, VisualContext::QUERY);
                return true;
            }
        }

        _visualiser.logAction("[Query Miss] " + item);
        _visualiser.logState(*this, i1, VisualContext::QUERY);
        return std::nullopt;
    }

    template <typename T>
    bool CuckooFilter<T>::erase(const T& item)
    {
        uint8_t fp = fingerprint(item);
        size_t i1 = hash1(item);
        size_t i2 = hash2(i1, fp);

        for (auto& slot : (*_table)[i1])
        {
            if (slot == fp)
            {
                slot = 0;
                _count--;
                _visualiser.logAction("[Erase] " + item);
                _visualiser.logState(*this, i1, VisualContext::ERASE);
                return true;
            }
        }

        for (auto& slot : (*_table)[i2])
        {
            if (slot == fp)
            {
                slot = 0;
                _count--;
                _visualiser.logAction("[Erase] " + item);
                _visualiser.logState(*this, i2, VisualContext::ERASE);
                return true;
            }
        }

        _visualiser.logAction("[Erase Miss] " + item);
        return false;
    }

    template <typename T>
    int32_t CuckooFilter<T>::getSize() const
    {
        return _count;
    }

    template <typename T>
    bool CuckooFilter<T>::isEmpty() const
    {
        return _count == 0;
    }

}