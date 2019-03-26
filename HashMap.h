#include <bits/stdc++.h>
using std::vector;
using std::pair;
using std::swap;
using std::initializer_list;
using std::out_of_range;
template<class KeyType, class ValueType, class Hash = std::hash<KeyType> > class HashMap {
typedef pair<const KeyType, ValueType> mypair;
 private:
    Hash Hasher;
    vector<pair<KeyType,ValueType>> data;
    vector<bool> isUsed;
    vector<bool> removed;
    size_t length;
    size_t realSize;
    size_t THRESHOLD = 2;
    void ReHash() {
        if (length * THRESHOLD + THRESHOLD >= data.size()) {
            size_t n = data.size() * THRESHOLD + THRESHOLD;
            vector<pair<KeyType,ValueType>> buffer(n);
            vector<bool> isUsed_buffer(n);
            for (size_t i = 0; i < data.size(); ++i) {
                if (isUsed[i] && removed[i] == 0) {
                    size_t pos = Hasher(data[i].first) % n;
                    while (isUsed_buffer[pos]) {
                        pos = (pos + 1) % n;
                    }
                    buffer[pos] = data[i];
                    isUsed_buffer[pos] = 1;
                }
            }
            length = realSize;
            removed.clear();
            removed.resize(n, false);
            swap(buffer, data);
            swap(isUsed_buffer, isUsed);
        }
    }
    size_t myfind(const KeyType& key) const {
        if (data.size() == 0) {
            return data.size();
        }
        size_t counter = 0;
        size_t pos = Hasher(key) % data.size();
        while (counter <= data.size() + 1 && isUsed[pos] == 1) {
            if (data[pos].first == key) {
                if (!removed[pos])
                    return pos;
                return data.size();
            }
            pos = (pos + 1) % data.size();
            ++counter;
        }
        if (isUsed[pos] && data[pos].first == key && removed[pos] == 0)
            return pos;
        return data.size();
    }
public:
    HashMap(Hash Hasher = Hash()): Hasher(Hasher), length(0), realSize(0) {};
    void insert(const pair <KeyType,ValueType> &x) {
        ReHash();
        size_t pos = Hasher(x.first) % data.size();
        while (isUsed[pos]) {
            if (data[pos].first == x.first) {
                if (removed[pos]) {
                    removed[pos] = false;
                    data[pos].second = x.second;
                    realSize += 1;
                }
                return;
            }
            pos = (pos + 1) % data.size();
        }
        isUsed[pos] = 1;
        data[pos] = x;
        length += 1;
        realSize += 1;
    }
    template <typename Iter> HashMap(Iter begin, Iter end, Hash Hasher = Hash()): HashMap(Hasher) {
        while (begin != end) {
            insert(*begin);
            ++begin;
        }
    }
    HashMap (const initializer_list<pair <KeyType,ValueType>> &List, Hash Hasher = Hash()): HashMap(Hasher) {
        auto Begin = List.begin();
        auto End = List.end();
        while (Begin != End) {
            insert(*Begin);
            ++Begin;
        }
    }
    size_t size() const {
        return realSize;
    }
    bool empty() const {
        return (realSize == 0);
    }
    Hash hash_function() const {
        return Hasher;
    }
    void erase(const KeyType &x) {
        if (data.size() == 0) {
            return;
        }
        size_t pos = Hasher(x) % data.size();
        size_t counter = 0;
        while (counter <= data.size() + 1 && isUsed[pos] == 1) {
            if (data[pos].first == x) break;
            pos = (pos + 1) % data.size();
            counter++;
        }
        if (isUsed[pos] == 1 && !removed[pos] && data[pos].first == x) {
            removed[pos] = 1;
            realSize -= 1;
        }
    }
    class iterator {
     friend class HashMap;
     private:
        HashMap* ptr;
        size_t ind;
        void forward() {
            if (ptr == nullptr) {
                return;
            }
            if (ind >= ptr->data.size()) {
                ind = ptr->data.size();
                return;
            }
            while(ptr->isUsed[ind] == 0 || ptr->removed[ind] == 1) {
                ind = ind + 1;
                if (ind >= ptr->data.size()) {
                    return;
                }
            }
        }
     public:
        iterator() :ptr(nullptr), ind(0) {};
        iterator(HashMap& cur, size_t ind): ptr(&cur), ind(ind) {
            forward();
        };
        iterator& operator ++() {
            ++ind;
            forward();
            return *this;
        }
        iterator operator ++(int) {
            auto tmp(*this);
            ++ind;
            forward();
            return tmp;
        }
        mypair& operator *() {
            return reinterpret_cast<mypair&>(ptr->data[ind]);
        }
        mypair* operator ->() {
            return reinterpret_cast<mypair*>(&ptr->data[ind]);
        }
        bool operator == (const iterator& other) const {
            return (other.ptr == ptr && other.ind == ind);
        }
        bool operator != (const iterator& other) const {
            return !(other.ptr == ptr && other.ind == ind);
        }
    };
    iterator begin() {
        return iterator(*this, 0);
    }
    iterator end() {
        return iterator(*this, data.size());
    }
    class const_iterator {
     friend class HashMap;
     private:
        const HashMap* ptr;
        size_t ind;
        void forward() {
            if (ptr == nullptr) {
                return;
            }
            if (ind >= ptr->data.size()) {
                ind = ptr->data.size();
                return;
            }
            while(ptr->isUsed[ind] == 0 || ptr->removed[ind] == 1) {
                ind = ind + 1;
                if (ind >= ptr->data.size()) {
                    return;
                }
            }
        }
     public:
        const_iterator() :ptr(nullptr), ind(0) {};
        const_iterator(const HashMap& cur, size_t ind): ptr(&cur), ind(ind) {
            forward();
        };
        const_iterator& operator ++() {
            ++ind;
            forward();
            return *this;
        }
        const_iterator operator ++(int) {
            auto tmp(*this);
            ++ind;
            forward();
            return tmp;
        }
        const mypair& operator *() const {
            return reinterpret_cast<const mypair&>(ptr->data[ind]);
        }
        const mypair* operator ->() const {
            return reinterpret_cast<const mypair*>(&ptr->data[ind]);
        }
        bool operator == (const const_iterator& other) const {
            return (other.ptr == ptr && other.ind == ind);
        }
        bool operator != (const const_iterator& other) const {
            return !(other.ptr == ptr && other.ind == ind);
        }
    };
    const_iterator begin() const {
        return const_iterator(*this, 0);
    }
    const_iterator end() const {
        return const_iterator(*this, data.size());
    }
    ValueType& operator [] (const KeyType& x) {
        insert({x, ValueType()});
        return data[myfind(x)].second;
    }
    iterator find(const KeyType& x) {
        return iterator(*this, myfind(x));
    }
    const_iterator find(const KeyType& x) const {
        return const_iterator(*this, myfind(x));
    }
    const ValueType& at(const KeyType& x) const {
        if (find(x) == end()) {
            throw out_of_range("no");
        }
        return data[myfind(x)].second;
    }
    void clear() {
        data.clear();
        isUsed.clear();
        removed.clear();
        length = 0;
        realSize = 0;
    }
};
