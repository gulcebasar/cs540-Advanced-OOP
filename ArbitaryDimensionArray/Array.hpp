#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

namespace cs540
{
    
    class OutOfRange : public std::runtime_error
    {
    public:
        OutOfRange() : std::runtime_error("Error!") { }
    };
    
    template <typename T, size_t S, size_t... Dims> class Array
    {
    public:
        typedef T ValueType;
        Array<T, Dims...> * arrayPtr;
        size_t dimSize;
        size_t size;
        
        class FirstDimensionMajorIterator
        {
        public:
            Array<T, S, Dims...> * arrayPtr;
            typename Array<T,Dims...>::FirstDimensionMajorIterator firstIt;
            size_t dims;
            size_t size;
            bool end;
            
            FirstDimensionMajorIterator() : arrayPtr(nullptr), dims(0), size(S), end(false) {};
            
            FirstDimensionMajorIterator(const FirstDimensionMajorIterator & fIt) : arrayPtr(fIt.arrayPtr), dims(fIt.dims), size(fIt.size), end(fIt.end), firstIt(fIt.firstIt) {};
            
            FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator & fIt)
            {
                arrayPtr = fIt.arrayPtr;
                dims = fIt.dims;
                size = fIt.size;
                end = fIt.end;
                firstIt = fIt.firstIt;
                return *this;
            };
            
            friend bool operator==(const FirstDimensionMajorIterator & fIt1, const FirstDimensionMajorIterator & fIt2)
            {
                if( (fIt1.arrayPtr != fIt2.arrayPtr) || (fIt1.size != fIt2.size) || (fIt1.dims != fIt2.dims) || (fIt1.end != fIt2.end) )
                   return false;
                
                if(fIt1.end)
                   return true;
                
                return fIt1.firstIt == fIt2.firstIt;
            };

            friend bool operator!=(const FirstDimensionMajorIterator & fIt1, const FirstDimensionMajorIterator & fIt2)
            {
                if( (fIt1.arrayPtr != fIt2.arrayPtr) || (fIt1.size != fIt2.size) || (fIt1.dims != fIt2.dims) || (fIt1.end != fIt2.end) )
                    return true;
                          
                if(fIt1.end)
                    return false;
                          
                return fIt1.firstIt != fIt2.firstIt;
            };

            void dimension()
            {
                firstIt.arrayPtr = &(arrayPtr->arrayPtr[dims]);
                firstIt.end = false;
                firstIt.dimension();
            };
            
            FirstDimensionMajorIterator &operator++()
            {
                ++firstIt;
                if( (firstIt.end) && (firstIt.dims == 0) )
                {
                    dims++;
                    if(dims == size)
                    {
                        dims = 0;
                        end = true;
                    }
                    else
                        end =false;
                    dimension();
                }
                return *this;
            };
            
            FirstDimensionMajorIterator operator++(int)
            {
                FirstDimensionMajorIterator temp = *this;
                ++(*this);
                return temp;
            };
            
            T &operator*() const { return *firstIt; };
        };
        
        class LastDimensionMajorIterator
        {
        public:
            Array<T, S, Dims...> * arrayPtr;
            typename Array<T,Dims...>::LastDimensionMajorIterator lastIt;
            size_t dims;
            size_t size;
            bool end;
            
            LastDimensionMajorIterator(): arrayPtr(nullptr), dims(0), size(S), end(false) {};
            
            LastDimensionMajorIterator(const LastDimensionMajorIterator & lIt): arrayPtr(lIt.arrayPtr), dims(lIt.dims), size(lIt.size), end(lIt.end), lastIt(lIt.lastIt) {};
            
            LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator & lIt)
            {
                arrayPtr = lIt.arrayPtr;
                dims = lIt.dims;
                size = lIt.size;
                end = lIt.end;
                lastIt = lIt.lastIt;
                return *this;
            };
            
            friend bool operator==(const LastDimensionMajorIterator & lIt1, const LastDimensionMajorIterator & lIt2)
            {
                if( (lIt1.arrayPtr != lIt2.arrayPtr) || (lIt1.size != lIt2.size) || (lIt1.dims != lIt2.dims) || (lIt1.end != lIt2.end) )
                    return false;
                
                if(lIt1.end)
                    return true;
                
                return lIt1.lastIt == lIt2.lastIt;
            };
            
            friend bool operator!=(const LastDimensionMajorIterator & lIt1, const LastDimensionMajorIterator & lIt2)
            {
                if( (lIt1.arrayPtr != lIt2.arrayPtr) || (lIt1.size != lIt2.size) || (lIt1.dims != lIt2.dims) || (lIt1.end != lIt2.end) )
                    return true;
                
                if(lIt1.end)
                    return false;
                
                return lIt1.lastIt != lIt2.lastIt;
            };

            void dimension()
            {
                lastIt.arrayPtr = &(arrayPtr->arrayPtr[dims]);
                lastIt.dimension();
                end = lastIt.end;
            };
            
            LastDimensionMajorIterator &operator++()
            {
                dims = (dims+1) % size;
                
                if(dims == 0)
                {
                    ++lastIt;
                    end = true;
                }
                dimension();
                return *this;
            };
            
            LastDimensionMajorIterator operator++(int)
            {
                LastDimensionMajorIterator temp = *this;
                ++(*this);
                return temp;
            };
            
            T &operator*() const { return *lastIt; };
        };
        
        Array() : arrayPtr(nullptr), dimSize(sizeof...(Dims)), size(S)
        {
            static_assert( S>0, "Wrong implementation!");
            arrayPtr = new Array<T, Dims...>[S];
        };
        
        Array(const Array & arr) : dimSize(sizeof...(Dims)), size(S)
        {
            if( (dimSize != arr.dimSize) || (size != arr.size) )
                throw OutOfRange();
            
            arrayPtr = new Array<T, Dims...>[size];
            
            for (size_t i=0; i<size; i++)
                arrayPtr[i] = arr.arrayPtr[i];
        };
        
        template <typename U> Array(const Array<U, S, Dims...> & arr) : dimSize(sizeof...(Dims)), size(S)
        {
            arrayPtr = new Array<T, Dims...>[size];
            
            for (size_t i=0; i<size; i++)
                arrayPtr[i] = arr.arrayPtr[i];

        };
        
        ~Array() { delete[] arrayPtr; };
        
        Array &operator=(const Array & arr)
        {
            if( (dimSize != arr.dimSize) || (size != arr.size) )
                throw OutOfRange();
            
            if (arrayPtr == arr.arrayPtr)
                return *this;
            
            for (size_t i=0; i<size; i++)
                arrayPtr[i] = arr.arrayPtr[i];
            
            return *this;
        };
        
        template <typename U> Array &operator=(const Array<U, S, Dims...> & arr)
        {
            for (size_t i=0; i<size; i++)
                arrayPtr[i] = arr.arrayPtr[i];
            
            return *this;
        };
        
        Array<T, Dims... > &operator[](const size_t i_D)
        {
            if( i_D >= size )
                throw OutOfRange();
                
            return arrayPtr[i_D];
        };
        
        const Array<T, Dims... > &operator[](const size_t i_D) const
        {
            if( i_D >= size )
                throw OutOfRange();
                
            return arrayPtr[i_D];
        }
        
        FirstDimensionMajorIterator fmbegin()
        {
            FirstDimensionMajorIterator fIt;
            fIt.arrayPtr = this;
            fIt.end = false;
            fIt.firstIt = arrayPtr[0].fmbegin();
            return fIt;
        };
        
        FirstDimensionMajorIterator fmend()
        {
            FirstDimensionMajorIterator fIt;
            fIt.arrayPtr = this;
            fIt.end = true;
            fIt.firstIt = arrayPtr[0].fmend();
            return fIt;
        };
        
        LastDimensionMajorIterator lmbegin()
        {
            LastDimensionMajorIterator lIt;
            lIt.arrayPtr = this;
            lIt.end = false;
            lIt.lastIt = arrayPtr[0].lmbegin();
            return lIt;
        };
        
        LastDimensionMajorIterator lmend()
        {
            LastDimensionMajorIterator lIt;
            lIt.arrayPtr = this;
            lIt.end = true;
            lIt.lastIt = arrayPtr[0].lmend();
            return lIt;
        };
    };
    
    template <typename T, size_t S> class Array<T,S>
    {
    public:
        typedef T ValueType;
        T * arrayPtr;
        size_t size;
        
        class FirstDimensionMajorIterator
        {
        public:
            Array<T, S> * arrayPtr;
            T * firstIt;
            size_t dims;
            size_t size;
            bool end;
            
            FirstDimensionMajorIterator() : arrayPtr(nullptr), firstIt(nullptr), dims(0), size(S), end(false) {};
            
            FirstDimensionMajorIterator(const FirstDimensionMajorIterator & fIt) : arrayPtr(fIt.arrayPtr), dims(fIt.dims), size(fIt.size), end(fIt.end), firstIt(fIt.firstIt) {};
            
            FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator & fIt)
            {
                arrayPtr = fIt.arrayPtr;
                dims = fIt.dims;
                size = fIt.size;
                end = fIt.end;
                firstIt = fIt.firstIt;
                return *this;
            };
            
            friend bool operator==(const FirstDimensionMajorIterator & fIt1, const FirstDimensionMajorIterator & fIt2)
            {
                if( (fIt1.arrayPtr != fIt2.arrayPtr) || (fIt1.size != fIt2.size) || (fIt1.dims != fIt2.dims) || (fIt1.end != fIt2.end) )
                    return false;
                
                if(fIt1.end)
                    return true;
                
                return fIt1.firstIt == fIt2.firstIt;
            };

            friend bool operator!=(const FirstDimensionMajorIterator & fIt1, const FirstDimensionMajorIterator & fIt2)
            {
                if( (fIt1.arrayPtr != fIt2.arrayPtr) || (fIt1.size != fIt2.size) || (fIt1.dims != fIt2.dims) || (fIt1.end != fIt2.end) )
                    return true;
                
                if(fIt1.end)
                    return false;
                
                return fIt1.firstIt != fIt2.firstIt;
            };
            
            void dimension() { firstIt = &(arrayPtr->arrayPtr[dims]); };
            
            FirstDimensionMajorIterator &operator++()
            {
                dims++;
                if(dims == size)
                {
                    dims = 0;
                    end = true;
                }
                else
                    end =false;
                dimension();
                
                return *this;
            };
            
            FirstDimensionMajorIterator operator++(int)
            {
                FirstDimensionMajorIterator temp = *this;
                ++(*this);
                return temp;
            };
            
            T &operator*() const { return *firstIt; };
        };
        
        class LastDimensionMajorIterator
        {
        public:
            Array<T, S> * arrayPtr;
            T * lastIt;
            size_t dims;
            size_t size;
            bool end;
            
            LastDimensionMajorIterator(): arrayPtr(nullptr), lastIt(nullptr), dims(0), size(S), end(false) {};
            
            LastDimensionMajorIterator(const LastDimensionMajorIterator & lIt): arrayPtr(lIt.arrayPtr), dims(lIt.dims), size(lIt.size), end(lIt.end), lastIt(lIt.lastIt) {};
            
            LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator & lIt)
            {
                arrayPtr = lIt.arrayPtr;
                dims = lIt.dims;
                size = lIt.size;
                end = lIt.end;
                lastIt = lIt.lastIt;
                return *this;
            };
            
            friend bool operator==(const LastDimensionMajorIterator & lIt1, const LastDimensionMajorIterator & lIt2)
            {
                if( (lIt1.arrayPtr != lIt2.arrayPtr) || (lIt1.size != lIt2.size) || (lIt1.dims != lIt2.dims) || (lIt1.end != lIt2.end) )
                    return false;
                
                if(lIt1.end)
                    return true;
                
                return lIt1.lastIt == lIt2.lastIt;
            };

            friend bool operator!=(const LastDimensionMajorIterator & lIt1, const LastDimensionMajorIterator & lIt2)
            {
                if( (lIt1.arrayPtr != lIt2.arrayPtr) || (lIt1.size != lIt2.size) || (lIt1.dims != lIt2.dims) || (lIt1.end != lIt2.end) )
                    return true;
                
                if(lIt1.end)
                    return false;
                
                return lIt1.lastIt == lIt2.lastIt;
            };
            
            void dimension() { lastIt = &(arrayPtr->arrayPtr[dims]); };
            
            LastDimensionMajorIterator &operator++()
            {
                dims++;
                
                if(dims == size)
                {
                    dims = dims % size;
                    end = true;
                }
                dimension();
                return *this;
            };
            
            LastDimensionMajorIterator operator++(int)
            {
                LastDimensionMajorIterator temp = *this;
                ++(*this);
                return temp;
            };
            
            T &operator*() const { return *lastIt; };
        };
        
        Array() : arrayPtr(nullptr), size(S)
        {
            static_assert( S>0, "Wrong implementation!");
            arrayPtr = new T[S];
        };
        
        Array(const Array & arr) : size(S)
        {
            if( size != arr.size )
                throw OutOfRange();
            
            arrayPtr = new T[size];
            
            for (size_t i=0; i<size; i++)
                arrayPtr[i] = arr.arrayPtr[i];
        };
        
        template <typename U> Array(const Array<U, S> & arr) : size(S)
        {
            arrayPtr = new T[size];
            
            for (size_t i=0; i<size; i++)
                arrayPtr[i] = arr.arrayPtr[i];
        };
        
        ~Array() { delete[] arrayPtr; };
        
        Array &operator=(const Array & arr)
        {
            if( size != arr.size )
                throw OutOfRange();
            
            if (arrayPtr == arr.arrayPtr)
                return *this;
            
            for (size_t i=0; i<size; i++)
                arrayPtr[i] = arr.arrayPtr[i];
            
            return *this;
        };
        
        template <typename U> Array &operator=(const Array<U, S> & arr)
        {
            for (size_t i=0; i<size; i++)
                arrayPtr[i] = arr.arrayPtr[i];
            
            return *this;
        };
        
        T &operator[](size_t i_D)
        {
            if( i_D >= size )
                throw OutOfRange();
            return arrayPtr[i_D];
        };
        
        const T &operator[](size_t i_D) const
        {
            if( i_D >= size )
                throw OutOfRange();
            return arrayPtr[i_D];
        }
        
        FirstDimensionMajorIterator fmbegin()
        {
            FirstDimensionMajorIterator fIt;
            fIt.arrayPtr = this;
            fIt.end = false;
            fIt.firstIt = &arrayPtr[0];
            return fIt;
        };
               
        FirstDimensionMajorIterator fmend()
        {
            FirstDimensionMajorIterator fIt;
            fIt.arrayPtr = this;
            fIt.end = true;
            fIt.firstIt = &arrayPtr[0];
            return fIt;
        };
        
        LastDimensionMajorIterator lmbegin()
        {
            LastDimensionMajorIterator lIt;
            lIt.arrayPtr = this;
            lIt.end = false;
            lIt.lastIt = &arrayPtr[0];
            return lIt;
        };
        
        LastDimensionMajorIterator lmend()
        {
            LastDimensionMajorIterator lIt;
            lIt.arrayPtr = this;
            lIt.end = true;
            lIt.lastIt = &arrayPtr[0];
            return lIt;
        };
    };


}
#endif















