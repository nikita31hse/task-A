#include <bits/stdc++.h>
using namespace std;
template<class KeyType, class ValueType, class Hash = std::hash<KeyType> > class HashMap {
private:
    Hash Hasher;
    vector<pair<KeyType,ValueType>> Data;
    vector<bool> IsUsed;
    vector<bool> Removed;
    size_t Size;
    size_t RealSize;
    void ReHash() {
        if (Size * 2 + 2 >= Data.size()) {
            size_t n = Data.size() * 2 + 2;
            vector<pair<KeyType,ValueType>> Data1(n);
            vector<bool> IsUsed1(n);
            for (size_t i = 0; i < Data.size(); ++i) {
                if (IsUsed[i] && Removed[i] == 0) {
                    size_t pos = Hasher(Data[i].first) % n;
                    while (IsUsed1[pos]) {
                        pos = (pos + 1) % n;
                    }
                    Data1[pos] = Data[i];
                    IsUsed1[pos] = 1;
                }
            }
            Size = RealSize;
            Removed.clear();
            Removed.resize(n, false);
            swap(Data1, Data);
            swap(IsUsed1, IsUsed);
        }
    }
    size_t myfind(const KeyType& x) const {
        if (Data.size() == 0) {
            return Data.size();
        }
        size_t counter = 0;
        size_t pos = Hasher(x) % Data.size();
        while (counter <= Data.size() + 1 && IsUsed[pos] == 1) {
            if (Data[pos].first == x) {
                if (!Removed[pos])
                    return pos;
                return Data.size();
            }
            pos = (pos + 1) % Data.size();
            ++counter;
        }
        if (IsUsed[pos] && Data[pos].first == x && Removed[pos] == 0)
            return pos;
        return Data.size();
    }
public:
    HashMap(Hash Hasher = Hash()): Hasher(Hasher), Size(0), RealSize(0) {};
    void insert(const pair <KeyType,ValueType> &x) {
        ReHash();
        size_t pos = Hasher(x.first) % Data.size();
        while (IsUsed[pos]) {
            if (Data[pos].first == x.first) {
                if (Removed[pos]) {
                    Removed[pos] = false;
                    Data[pos].second = x.second;
                    RealSize += 1;
                }
                return;
            }
            pos = (pos + 1) % Data.size();
        }
        IsUsed[pos] = 1;
        Data[pos] = x;
        Size += 1;
        RealSize += 1;
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
        return RealSize;
    }
    bool empty() const {
        return (RealSize == 0);
    }
    Hash hash_function() const {
        return Hasher;
    }
    void erase(const KeyType &x) {
        if (Data.size() == 0) {
            return;
        }
        size_t pos = Hasher(x) % Data.size();
        size_t counter = 0;
        while (counter <= Data.size() + 1 && IsUsed[pos] == 1) {
            if (Data[pos].first == x) break;
            pos = (pos + 1) % Data.size();
            counter++;
        }
        if (IsUsed[pos] == 1 && !Removed[pos] && Data[pos].first == x) {
            Removed[pos] = 1;
            RealSize -= 1;
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
            if (ind >= ptr->Data.size()) {
                ind = ptr->Data.size();
                return;
            }
            while(ptr->IsUsed[ind] == 0 || ptr->Removed[ind] == 1) {
                ind = ind + 1;
                if (ind >= ptr->Data.size()) {
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
        pair<const KeyType, ValueType>& operator *() {
            return reinterpret_cast<pair<const KeyType, ValueType>&>(ptr->Data[ind]);
        }
        pair<const KeyType, ValueType>* operator ->() {
            return reinterpret_cast<pair<const KeyType, ValueType>*>(&ptr->Data[ind]);
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
        return iterator(*this, Data.size());
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
            if (ind >= ptr->Data.size()) {
                ind = ptr->Data.size();
                return;
            }
            while(ptr->IsUsed[ind] == 0 || ptr->Removed[ind] == 1) {
                ind = ind + 1;
                if (ind >= ptr->Data.size()) {
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
        const pair<const KeyType, ValueType>& operator *() const {
            return reinterpret_cast<const pair<const KeyType, ValueType>&>(ptr->Data[ind]);
        }
        const pair<const KeyType, ValueType>* operator ->() const {
            return reinterpret_cast<const pair<const KeyType, ValueType>*>(&ptr->Data[ind]);
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
        return const_iterator(*this, Data.size());
    }
    ValueType& operator [] (const KeyType& x) {
        insert({x, ValueType()});
        return Data[myfind(x)].second;
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
        return Data[myfind(x)].second;
    }
    void clear() {
        Data.clear();
        IsUsed.clear();
        Removed.clear();
        Size = 0;
        RealSize = 0;
    }
};
