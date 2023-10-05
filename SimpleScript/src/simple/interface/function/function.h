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
        
        std::function<string(size_t, string *)> operation;
    public:
        //  CONSTRUCTORS
        
        function(const string name, const std::function<string(size_t, string *)> operation) {
            this->rename(name);
            this->operation = operation;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        string call(const size_t argc, string* argv) {
            //  consume();
            
            return operation(argc, argv);
        }
    };
}

#endif /* function_h */
