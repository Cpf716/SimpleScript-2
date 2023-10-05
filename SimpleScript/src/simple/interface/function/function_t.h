//
//  function_t.h
//  SimpleScript
//
//  Created by Corey Ferguson on 12/12/22.
//

#ifndef function_t_h
#define function_t_h

#include <iostream>

using namespace std;

namespace ss {
    class function_t {
        //  MEMBER FIELDS
        
        size_t _count = 0;
        string _name;
    public:
        //  CONSTRUCTORS
        
        virtual void close() = 0;
        
        //  MEMBER FUNCTIONS
        
        virtual string call(const size_t argc, string* argv) = 0;
        
        size_t count() const { return _count; }
        
        void consume() { _count++; }
        
        string name() const { return _name; }
        
        void rename(const string name) { _name = name; }
    };
}

#endif /* function_t_h */
