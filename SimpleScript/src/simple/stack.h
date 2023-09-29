//
//  stack.h
//  SimpleScript
//
//  Created by Corey Ferguson on 6/7/23.
//

#ifndef stack_h
#define stack_h

#include "utility.h"

using namespace std;

namespace simple {
    template <typename T>
    class stack {
        //  MEMBER FIELDS
        
        T* data = new T[1];
        size_t n = 0;
    public:
        //  CONSTRUCTORS
        
        ~stack() { delete[] data; }
        
        //  MEMBER FUNCTIONS
        
        bool empty() const { return !size(); }
        
        T pop() {
            T _top = top();
            
            for (size_t i = 0; i < n - 1; ++i)
                std::swap(data[i], data[i + 1]);
            
            --n;
            
            return _top;
        }
        
        void push(const T val) {
            if (is_pow(n, 2)) {
                T* tmp = new T[n * 2];
                
                for (size_t i = 0; i < n; ++i)
                    tmp[i] = data[i];
                
                delete[] data;
                data = tmp;
            }
            
            data[n] = val;
            
            for (size_t i = n; i > 0; --i)
                std::swap(data[i], data[i - 1]);
            
            ++n;
        }
        
        size_t size() const { return n; }
        
        void swap(stack& x) {
            T* _data = new T[pow2(size())];
            size_t _n = size();
            
            for (size_t i = 0; i < size(); ++i)
                _data[i] = data[i];
            
            delete[] data;
            
            n = x.size();
            data = new T[pow2(size())];
            
            for (size_t i = 0; i < size(); ++i)
                data[i] = x.data[i];
            
            delete[] x.data;
            
            x.n = _n;
            x.data = new T[x.size()];
            
            for (size_t i = 0; i < x.size(); ++i)
                x.data[i] = _data[i];
            
            delete[] _data;
        }
        
        T top() const {
            if (empty())
                throw error("Underflow");
            
            return data[0];
        }
    };
}

#endif /* stack_h */
