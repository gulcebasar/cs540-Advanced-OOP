#ifndef MAP_HPP
#define MAP_HPP

#define ValueType std::pair<Key_T, Mapped_T>

#include <iostream>
#include <vector>
#include <initializer_list>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <utility>

namespace cs540
{
    template <typename Key_T, typename Mapped_T> class Map
    {
    private:
        class BST
        {
        public:
            ValueType* value;
            BST* next;
            BST* prev;
            
            BST() : value(nullptr), next(nullptr), prev(nullptr) {};
            BST(ValueType newValue) : next(nullptr), prev(nullptr) { value = new ValueType(newValue); };
            ~BST() { if(value) delete value; };
          
            bool operator==(const BST &);
            bool operator!=(const BST &);
        };
        
        BST* first;
        BST* last;
        std::size_t mSize;
    public:
        class ConstIterator;
        class ReverseIterator;
        
        class Iterator
        {
        private:
            BST* curr;
        public:
            BST* getCurr() const { return curr; };
            Iterator(BST* bst) : curr(bst) {};
            Iterator(const Iterator & it) : curr(it.getCurr()) {};
            ~Iterator() { };
            
            Iterator& operator=(const Iterator &);
            Iterator &operator++();
            Iterator operator++(int);
            Iterator &operator--();
            Iterator operator--(int);
            ValueType &operator*() const;
            ValueType *operator->() const;
            bool operator==(const Iterator &);
            bool operator!=(const Iterator &);
            bool operator==(const ConstIterator &);
            bool operator!=(const ConstIterator &);
            
            friend class Map;
        };
    
        class ConstIterator
        {
        private:
            BST* curr;
        public:
            BST* getCurr() const { return curr; };
            ConstIterator(BST* bst) : curr(bst) {};
            ConstIterator(const ConstIterator & cit) : curr(cit.getCurr()) {};
            ConstIterator(const Iterator & it) : curr(it.getCurr()) {};
            ~ConstIterator() { };
            
            ConstIterator& operator=(const ConstIterator &);
            ConstIterator &operator++();
            ConstIterator operator++(int);
            ConstIterator &operator--();
            ConstIterator operator--(int);
            const ValueType &operator*() const;
            const ValueType *operator->() const;
            bool operator==(const ConstIterator &);
            bool operator!=(const ConstIterator &);
            bool operator==(const Iterator &);
            bool operator!=(const Iterator &);
            
            friend class Map;
        };
    
        class ReverseIterator
        {
        private:
            BST* curr;
        public:
            BST* getCurr() const { return curr; };
            ReverseIterator(BST* bst) : curr(bst) {};
            ReverseIterator(const ReverseIterator & rit) : curr(rit.getCurr()) {};
            ~ReverseIterator() { };
            
            ReverseIterator& operator=(const ReverseIterator &);
            ReverseIterator &operator++();
            ReverseIterator operator++(int);
            ReverseIterator &operator--();
            ReverseIterator operator--(int);
            ValueType &operator*() const;
            ValueType *operator->() const;
            bool operator==(const ReverseIterator &);
            bool operator!=(const ReverseIterator &);
            
            friend class Map;
        };
        
        Map();
        Map(const Map &);
        Map &operator=(const Map &);
        Map(std::initializer_list<std::pair<const Key_T, Mapped_T>>);
        ~Map();
        
        std::size_t size() const;
        bool empty() const;
        
        Iterator begin();
        Iterator end();
        ConstIterator begin() const;
        ConstIterator end() const;
        ReverseIterator rbegin();
        ReverseIterator rend();
        
        Iterator find(const Key_T &);
        ConstIterator find(const Key_T &) const;
        Mapped_T &at(const Key_T &);
        const Mapped_T &at(const Key_T &) const;
        Mapped_T &operator[](const Key_T &);
        
        std::pair<Iterator, bool> insert(const ValueType &);
        template <typename IT_T> void insert(IT_T range_beg, IT_T range_end);
        void erase(Iterator pos);
        void erase(const Key_T &);
        void clear();
        
        bool operator==(const Map &);
        bool operator!=(const Map &);
        bool operator<(const Map &);
        
    private:
        Iterator RecFind(const Key_T &, BST*);
        ConstIterator RecFind(const Key_T &, BST*) const;
        std::pair<Iterator, bool> insertHelp(const ValueType &, BST*);
        void eraseHelp(BST*);
    };
}
#endif


template <typename Key_T, typename Mapped_T> cs540::Map<Key_T, Mapped_T>::Map() : first(nullptr), last(new BST()),  mSize(0)
{ }

template <typename Key_T, typename Mapped_T> cs540::Map<Key_T, Mapped_T>::Map(const Map & nMap)
{
    mSize = 0;
    last = new BST();
    first=nullptr;
    
    for (auto it = nMap.begin(); it != nMap.end(); it++)
        insert(*it);
}
    
template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::Map & cs540::Map<Key_T, Mapped_T>::operator=(const Map & nMap)
{
    if(*this == nMap)
        return *this;
    
    clear();
    
    for (auto it = nMap.begin(); it != nMap.end(); it++)
        insert(*it);
    
    return *this;
}

template <typename Key_T, typename Mapped_T> cs540::Map<Key_T, Mapped_T>::Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> values)
{
    mSize = 0;
    last = new BST();
    first=nullptr;
    
    for (auto it = values.begin(); it != values.end(); it++)
    {
        insert(*it);
    }
}

template <typename Key_T, typename Mapped_T> cs540::Map<Key_T, Mapped_T>::~Map()
{
    clear();
    if(last)
        delete last;
}

template <typename Key_T, typename Mapped_T> size_t cs540::Map<Key_T, Mapped_T>::size() const
{
    return mSize;
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::empty() const
{
    return mSize==0;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::Iterator cs540::Map<Key_T, Mapped_T>::begin()
{
    if (mSize == 0)
        return Iterator(last);
    return Iterator(first);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::Iterator cs540::Map<Key_T, Mapped_T>::end()
{
    return Iterator(last);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ConstIterator cs540::Map<Key_T, Mapped_T>::begin() const
{
    if (mSize == 0)
        return ConstIterator(last);
    return ConstIterator(first);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ConstIterator cs540::Map<Key_T, Mapped_T>::end() const
{
    return ConstIterator(last);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ReverseIterator cs540::Map<Key_T, Mapped_T>::rbegin()
{
    if (mSize == 0)
        return ReverseIterator(last);
    return ReverseIterator(last->prev);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ReverseIterator cs540::Map<Key_T, Mapped_T>::rend()
{
    return ReverseIterator(last);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::Iterator cs540::Map<Key_T, Mapped_T>::find(const Key_T & key)
{
    if(mSize==0)
        return end();
    
    return RecFind(key,first);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::Iterator cs540::Map<Key_T, Mapped_T>::RecFind(const Key_T & key, BST* bst)
{
    if( bst == last )
        return end();
    
    if(bst->value->first == key)
        return Iterator(bst);
    else
        return RecFind(key, bst->next);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ConstIterator cs540::Map<Key_T, Mapped_T>::find(const Key_T & key) const
{
    if(mSize==0)
        return end();
    return RecFind(key,first);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ConstIterator cs540::Map<Key_T, Mapped_T>::RecFind(const Key_T & key, BST* bst) const
{
    if( bst == last )
        return end();
    
    if(bst->value->first == key)
        return ConstIterator(bst);
    else
        return RecFind(key, bst->next);
}

template <typename Key_T, typename Mapped_T> Mapped_T & cs540::Map<Key_T, Mapped_T>::at(const Key_T & key)
{
    auto it = find(key);
    if( it.getCurr() == last )
        throw std::out_of_range("Out Of Range");
    else
        return it.getCurr()->value->second;
}

template <typename Key_T, typename Mapped_T> const Mapped_T & cs540::Map<Key_T, Mapped_T>::at(const Key_T & key) const
{
    auto it = find(key);
    if( it.getCurr() == last )
        throw std::out_of_range("Out Of Range");
    else
        return it.getCurr()->value->second;
}

template <typename Key_T, typename Mapped_T> Mapped_T & cs540::Map<Key_T, Mapped_T>::operator[](const Key_T & key)
{
    auto it = find(key);
    
    if( it.getCurr() != last )
        return it.getCurr()->value->second;
    
    auto it2 = insert(std::make_pair(key, Mapped_T{}));
    
    return it2.first.getCurr()->value->second;
}

template <typename Key_T, typename Mapped_T> std::pair<typename cs540::Map<Key_T, Mapped_T>::Iterator, bool> cs540::Map<Key_T, Mapped_T>::insert(const ValueType & value)
{
    if (!first) // if bst empty
    {
        first = new BST(value);
        
        last->prev=first;
        last->next=first;
        
        first->prev = last;
        first->next = last;
        
        mSize++;
        
        return std::make_pair(Iterator(first), true);
    }
    else
        return insertHelp(value,first); //starts from the first node
}

template <typename Key_T, typename Mapped_T> std::pair<typename cs540::Map<Key_T, Mapped_T>::Iterator, bool> cs540::Map<Key_T, Mapped_T>::insertHelp(const ValueType & value, BST* checkBST)
{
    if( (checkBST == last))
    {
        BST* newBST = new BST(value);
        
        newBST->next = checkBST;
        newBST->prev = checkBST->prev;
        
        checkBST->prev = newBST;
        newBST->prev->next = newBST;
        
        mSize++;
        
        return std::pair<Iterator, bool>(Iterator(newBST), true);
 
    }
    if( checkBST->value->first == value.first )
    {
        return std::pair<Iterator, bool>(Iterator(checkBST), false);
    }
    else if((value.first < checkBST->value->first))
    {
        BST* newBST = new BST(value);
        
        newBST->next = checkBST;
        newBST->prev = checkBST->prev;
        
        checkBST->prev = newBST;
        newBST->prev->next = newBST;
        
        if (checkBST == first)
            first = newBST;
        
        mSize++;
        
        return std::pair<Iterator, bool>(Iterator(newBST), true);
    }
    else
        return insertHelp(value,checkBST->next);
}

template <typename Key_T, typename Mapped_T> template <typename IT_T> void cs540::Map<Key_T, Mapped_T>::insert(IT_T range_beg, IT_T range_end)
{
    for (auto it = range_beg; it != range_end; it++)
        insert(*it);
}

template <typename Key_T, typename Mapped_T> void cs540::Map<Key_T, Mapped_T>::erase(Iterator pos)
{
    eraseHelp(pos.getCurr());
}

template <typename Key_T, typename Mapped_T> void cs540::Map<Key_T, Mapped_T>::eraseHelp(BST* bst)
{
    if(mSize == 1)
    {
        first = nullptr;
        last->prev = first;
        last->next = first;
        
        delete bst;
        mSize=0;
    }
    else
    {
        bst->next->prev = bst->prev;
        bst->prev->next = bst->next;
        
        if( first == bst )
            first = bst->next;
        
        delete bst;
        mSize--;
    }
}

template <typename Key_T, typename Mapped_T> void cs540::Map<Key_T, Mapped_T>::erase(const Key_T & key)
{
    auto it = find(key);
    if(it == end())
        throw std::out_of_range("Out Of Range");
    else
        erase(it);
}

template <typename Key_T, typename Mapped_T> void cs540::Map<Key_T, Mapped_T>::clear()
{
    while (first)
    {
        eraseHelp(first);
    }
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::Iterator& cs540::Map<Key_T, Mapped_T>::Iterator::operator=(const Iterator & it)
{
    if (curr == it.getCurr())
        return *this;
   
    curr = it.getCurr();
    return *this;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::Iterator & cs540::Map<Key_T, Mapped_T>::Iterator::operator++()
{
    curr = curr->next;
    return *this;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::Iterator cs540::Map<Key_T, Mapped_T>::Iterator::operator++(int)
{
    auto it = *this;
    curr = curr->next;
    return it;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::Iterator & cs540::Map<Key_T, Mapped_T>::Iterator::operator--()
{
    curr = curr->prev;
    return *this;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::Iterator cs540::Map<Key_T, Mapped_T>::Iterator::operator--(int)
{
    auto it = *this;
    curr = curr->prev;
    return it;
}

template <typename Key_T, typename Mapped_T> ValueType & cs540::Map<Key_T, Mapped_T>::Iterator::operator*() const
{
    return *(curr->value);
}

template <typename Key_T, typename Mapped_T> ValueType * cs540::Map<Key_T, Mapped_T>::Iterator::operator->() const
{
    return (curr->value);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ConstIterator& cs540::Map<Key_T, Mapped_T>::ConstIterator::operator=(const ConstIterator & it)
{
    if (curr == it.getCurr())
        return *this;
    curr = it.getCurr();
    
    return *this;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ConstIterator & cs540::Map<Key_T, Mapped_T>::ConstIterator::operator++()
{
    curr = curr->next;
    return *this;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ConstIterator cs540::Map<Key_T, Mapped_T>::ConstIterator::operator++(int)
{
    auto it = *this;
    curr = curr->next;
    return it;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ConstIterator & cs540::Map<Key_T, Mapped_T>::ConstIterator::operator--()
{
    curr = curr->prev;
    return *this;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ConstIterator cs540::Map<Key_T, Mapped_T>::ConstIterator::operator--(int)
{
    auto it = *this;
    curr = curr->prev;
    return it;
}

template <typename Key_T, typename Mapped_T> const ValueType & cs540::Map<Key_T, Mapped_T>::ConstIterator::operator*() const
{
    return *(curr->value);
}

template <typename Key_T, typename Mapped_T> const ValueType * cs540::Map<Key_T, Mapped_T>::ConstIterator::operator->() const
{
    return (curr->value);
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ReverseIterator& cs540::Map<Key_T, Mapped_T>::ReverseIterator::operator=(const ReverseIterator & it)
{
    if (curr == it.getCurr())
        return *this;
    curr = it.getCurr();
    
    return *this;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ReverseIterator & cs540::Map<Key_T, Mapped_T>::ReverseIterator::operator++()
{
    curr = curr->prev;
    return *this;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ReverseIterator cs540::Map<Key_T, Mapped_T>::ReverseIterator::operator++(int)
{
    auto it = *this;
    curr = curr->prev;
    return it;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ReverseIterator & cs540::Map<Key_T, Mapped_T>::ReverseIterator::operator--()
{
    curr = curr->next;
    return *this;
}

template <typename Key_T, typename Mapped_T> typename cs540::Map<Key_T, Mapped_T>::ReverseIterator cs540::Map<Key_T, Mapped_T>::ReverseIterator::operator--(int)
{
    auto it = *this;
    curr = curr->next;
    return it;
}

template <typename Key_T, typename Mapped_T> ValueType & cs540::Map<Key_T, Mapped_T>::ReverseIterator::operator*() const
{
    return *(curr->value);
}

template <typename Key_T, typename Mapped_T> ValueType * cs540::Map<Key_T, Mapped_T>::ReverseIterator::operator->() const
{
    return (curr->value);
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::operator==(const Map & map2)
{
    if(mSize != map2.size())
        return false;
    
    auto it1=begin();
    for(auto it2=map2.begin(); it1!=end(), it2!=map2.end(); it1++, it2++)
    {
        if( it1->first == it2->first )
        {
            if( it1->second == it2->second ) {}
            else return false;
        }
        else
            return false;
    }
    return true;
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::operator!=(const Map & map2)
{
    return !((*this)== map2);
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::operator<(const Map & map2)
{
    auto it1=begin();
    for(auto it2=map2.begin(); it1!=end(), it2!=map2.end(); it1++, it2++)
        if( (*it1) < (*it2) )
            return true;
    if(mSize < map2.size())
        return true;
    return false;
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::Iterator::operator==(const Iterator & it2)
{
    return curr == it2.getCurr();
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::ConstIterator::operator==(const ConstIterator & it2)
{
    return (curr == it2.getCurr());
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::Iterator::operator==(const ConstIterator & it2)
{
    return (curr == it2.getCurr());
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::ConstIterator::operator==(const Iterator & it2)
{
    return (curr == it2.getCurr());
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::Iterator::operator!=(const Iterator & it2)
{
    return !((*this)==it2);
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::ConstIterator::operator!=(const ConstIterator & it2)
{
    return !((*this)==it2);
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::Iterator::operator!=(const ConstIterator & it2)
{
    return !((*this)==it2);
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::ConstIterator::operator!=(const Iterator & it2)
{
    return !((*this)==it2);
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::ReverseIterator::operator==(const ReverseIterator & it2)
{
    return (curr == it2.getCurr());
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::ReverseIterator::operator!=(const ReverseIterator & it2)
{
    return !((*this)==it2);
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::BST::operator==(const BST & bst)
{
    if( (bst.value==value) && (bst.next==next) && (bst.prev==prev) )
        return true;
    return false;
}

template <typename Key_T, typename Mapped_T> bool cs540::Map<Key_T, Mapped_T>::BST::operator!=(const BST & bst)
{
    return (*this==bst);
}







