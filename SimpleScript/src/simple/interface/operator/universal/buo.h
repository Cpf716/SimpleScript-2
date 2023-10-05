//
//  buo.h
//  SimpleScript
//
//  Created by Corey Ferguson on 11/8/22.
//

#ifndef buo_h
#define buo_h

#include "operator_t.h"

namespace ss {
    class buo: public operator_t {
        //  MEMBER FIELDS
        
        std::function<string(const string, const string)> operation;
    public:
        //  CONSTRUCTORS
        
        buo(const string opcode, const std::function<string(const string, const string)> operation) {
            this->set_opcode(opcode);
            this->operation = operation;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        string apply(const string lhs, const string rhs) const {
            return operation(lhs, rhs);
        }
    };
}

#endif /* buo_h */
