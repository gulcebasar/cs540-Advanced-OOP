#include <iostream>

#ifndef FUNCWRAPPER_HPP
#define FUNCWRAPPER_HPP

namespace cs540
{
    class BadFunctionCall
    {
    public:
        BadFunctionCall() {};
    };
    
    template <typename> class Function;
    
    template <typename FunctionType, typename ResultType, typename... ArgumentTypes>
    class HelpFunction;
    
    template <typename FunctionType, typename ResultType, typename... ArgumentTypes>
    class HelpFunction<FunctionType, ResultType (ArgumentTypes...)> : public Function<ResultType (ArgumentTypes...)>
    {
    public:
        FunctionType func;
        
        HelpFunction (FunctionType func) : func(func) { }
        
        ResultType operator()(ArgumentTypes... args) override
        {
            if(this)
                return func(args...);
            throw BadFunctionCall();
        }
        
        explicit operator bool() const
        {
            if(this)
                return true;
            return false;
        };
    };

    
    template <typename ResultType, typename ... ArgumentTypes>
    class Function<ResultType(ArgumentTypes...)>
    {
    public:
        Function<ResultType(ArgumentTypes...)>* _func;
        
        Function() : _func(nullptr) {};
        
        template <typename FunctionType> Function(FunctionType f) : _func(new HelpFunction<FunctionType, ResultType (ArgumentTypes...)>(f)) { };
        
        Function(const Function & f) : _func(f._func) { };
        
        Function &operator=(const Function & f)
        {
            _func = f._func;
            return *this;
        };
        
        ~Function()
        {
            //if (_func)
                //delete _func;
        };
        
        virtual ResultType operator()(ArgumentTypes... args)
        {
            if( _func )
               return (*_func)(args...);
            throw BadFunctionCall();
        }
        
        explicit operator bool() const
        {
            if( _func )
                return true;
            return false;
        };
    };
    
    
    template <typename ResultType, typename ... ArgumentTypes>
    bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t)
    { return !f; };
    
    template <typename ResultType, typename ... ArgumentTypes>
    bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f)
    { return !f; };
    
    template <typename ResultType, typename ... ArgumentTypes>
    bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t)
    { return bool(f); };
    
    template <typename ResultType, typename ... ArgumentTypes>
    bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f)
    { return bool(f); };
}
#endif