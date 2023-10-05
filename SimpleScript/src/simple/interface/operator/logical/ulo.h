//
//  ulo.h
//  SimpleScript
//
//  Created by Corey Ferguson on 10/29/22.
//

#ifndef ulo_h
#define ulo_h

#include "operator_t.h"

namespace ss {
    class ulo : public operator_t {
        //  MEMBER FIELDS
        
        std::function<double(const string s)> operation;
    public:
        //  CONSTRUCTORS
        
        ulo(const string opcode, const std::function<double(const string s)> operation) {
            set_opcode(opcode);
            
            this->operation = operation;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        double apply(const string s) const { return operation(s); }
    };
}

#endif /* ulo_h */
