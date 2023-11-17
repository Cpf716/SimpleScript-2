//
//  function.h
//  SimpleScript
//
//  Created by Corey Ferguson on 12/12/22.
//

#ifndef function_h
#define function_h

#include "function_t.h"

namespace ss {
    class function: public function_t {
        //  MEMBER FIELDS
        
        std::function<string(size_t, string *)> _function;
    public:
        //  CONSTRUCTORS
        
        function(const string name, const std::function<string(size_t, string *)> function) {
            rename(name);
            
            _function = function;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        string call(const size_t argc, string* argv) {
            //  consume();
            
            return _function(argc, argv);
        }
    };
}

#endif /* function_h */
