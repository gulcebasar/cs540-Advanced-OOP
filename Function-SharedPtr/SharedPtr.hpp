#include <iostream>
#include <atomic>


#ifndef SHAREDPTR_HPP
#define SHAREDPTR_HPP

namespace cs540
{
    class Helper
    {
        template <typename T>
        friend class SharedPtr;
    private:
        std::atomic<int>* count;
    public:
        Helper() { count = new std::atomic<int>(); };
        virtual ~Helper() { delete count; };
        virtual int increment() { return ++(*count); };
        virtual int decrement() { return --(*count); };
    };
    
    template <typename G>
    class Helper2 : public Helper
    {
    private:
        G* ptr;
    public:
        Helper2(G* g) : ptr(g) {};
        virtual ~Helper2() { delete ptr; ptr=nullptr; };
    };
    
    template <typename T> class SharedPtr
    {
    public:
        T* ptr;
        Helper* hPtr;
        
        SharedPtr() : ptr(nullptr), hPtr(nullptr) {};
        
        template <typename U> explicit SharedPtr(U *p)
        {
            ptr = static_cast<T*>(p);
            hPtr = new Helper2<U>(p);
            hPtr->increment();
        };
        
        SharedPtr(const SharedPtr &p)
        {
            ptr = static_cast<T*>( p.get() );
            hPtr = p.hPtr;
            if (hPtr != nullptr)
                hPtr->increment();
        };
        
        template <typename U> SharedPtr(const SharedPtr<U> &p)
        {
            ptr = static_cast<T*>( p.get() );
            hPtr = p.hPtr;
            if (hPtr != nullptr)
                hPtr->increment();
        };
        
        SharedPtr(SharedPtr &&p)
        {
            ptr = static_cast<T*>( p.get() );
            hPtr = p.hPtr;
            p.ptr = nullptr;
            p.hPtr = nullptr;
        };
        
        template <typename U> SharedPtr(SharedPtr<U> &&p)
        {
            ptr = static_cast<T*>( p.get() );
            hPtr = p.hPtr;
            p.ptr = nullptr;
            p.hPtr = nullptr;
        };
        
        SharedPtr &operator=(const SharedPtr &p)
        {
            if( p == *this )
                return *this;
            
            if( (hPtr != nullptr) && (hPtr->decrement() == 0) )
            {
                delete hPtr;
                hPtr = nullptr;
                ptr = nullptr;
            }
            
            hPtr = p.hPtr;
            if(hPtr != nullptr)
                hPtr->increment();
            
            ptr = p.get();
            
            return *this;
        };
        
        template <typename U> SharedPtr<T> &operator=(const SharedPtr<U> &p)
        {
            if( (SharedPtr<T>)p == *this )
                return *this;
            
            if( (hPtr != nullptr) && (hPtr->decrement() == 0) )
            {
                delete hPtr;
                hPtr = nullptr;
                ptr = nullptr;
            }
            
            hPtr = (Helper2<T>*)p.hPtr;
            if(hPtr != nullptr)
                hPtr->increment();
            
            ptr = static_cast<T*>(p.get());
            
            return *this;
        };
        
        SharedPtr &operator=(SharedPtr &&p)
        {
            if( (hPtr != nullptr) && (hPtr->decrement() == 0) )
            {
                delete hPtr;
                hPtr = nullptr;
                ptr = nullptr;
            }
            ptr = static_cast<T*>( p.get() );
            hPtr = p.hPtr;
            p.ptr = nullptr;
            p.hPtr = nullptr;
            
            return *this;
        };
        
        template <typename U> SharedPtr &operator=(SharedPtr<U> &&p)
        {
            if( (hPtr != nullptr) && (hPtr->decrement() == 0) )
            {
                delete hPtr;
                hPtr = nullptr;
                ptr = nullptr;
            }
            ptr = static_cast<T*>( p.get() );
            hPtr = p.hPtr;
            p.ptr = nullptr;
            p.hPtr = nullptr;
            
            return *this;
        };
        
        ~SharedPtr()
        {
            if( (hPtr != nullptr) && (hPtr->decrement() == 0) )
            {
                delete hPtr;
                hPtr = nullptr;
                ptr = nullptr;
            }
        };
        
        void reset()
        {
            if(hPtr != nullptr)
            {
                if(hPtr->decrement() == 0)
                    delete hPtr;
                
                hPtr = nullptr;
                ptr = nullptr;
            }
        };
        
        template <typename U> void reset(U *p)
        {
            if(hPtr != nullptr)
            {
                if(hPtr->decrement() == 0)
                    delete hPtr;
                
                hPtr = nullptr;
                ptr = nullptr;
            }
            ptr = static_cast<T*>(p);
            hPtr = new Helper2<U>(p);
            hPtr->increment();
        };
        
        T *get() const { return ptr; };
        T &operator*() const { return (*ptr); };
        T *operator->() const { return ptr; };
        explicit operator bool() const { return (ptr != nullptr); };
    };
    
    //free funcs
    template <typename T1, typename T2> bool operator==(const SharedPtr<T1> & p1, const SharedPtr<T2> & p2)
    {
        return ( (p1.get()==p2.get()) || ( (p1==nullptr) && (p2==nullptr) ) );
    };
    
    template <typename T> bool operator==(const SharedPtr<T> &p, std::nullptr_t n)
    {
        return !p;
    };
    
    template <typename T> bool operator==(std::nullptr_t n, const SharedPtr<T> &p)
    {
        return !p;
    };
    
    template <typename T1, typename T2> bool operator!=(const SharedPtr<T1>& p1, const SharedPtr<T2> & p2)
    {
        return !(p1==p2);
    };
    
    template <typename T> bool operator!=(const SharedPtr<T> &p, std::nullptr_t n)
    {
        return !(p==n);
    };
    
    template <typename T> bool operator!=(std::nullptr_t n, const SharedPtr<T> &p)
    {
        return !(n==p);
    };
    
    template <typename T, typename U> SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp)
    {
        return SharedPtr<T>(sp);
    };
    
    template <typename T, typename U> SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp)
    {
        SharedPtr<T> temp;
        temp.ptr = dynamic_cast<T*>(sp.ptr);
        temp.hPtr = sp.hPtr;
        if (temp.hPtr != nullptr)
            temp.hPtr->increment();
        return temp;
    };
}
#endif