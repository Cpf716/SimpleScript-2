//
//  tuo.h
//  SimpleScript
//
//  Created by Corey Ferguson on 6/1/23.
//

#ifndef tuo_h
#define tuo_h

#include "operator_t.h"

namespace ss {
    class tuo: public operator_t {
        //  MEMBER FIELDS
        
        std::function<string(const string, const string, const string)> operation;
    public:
        //  CONSTRUCTORS
        
        tuo(const string opcode, const std::function<string(const string, const string, const string)> operation) {
            this->set_opcode(opcode);
            this->operation = operation;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        string apply(const string lhs, const string ctr, const string rhs) const {
            return operation(lhs, ctr, rhs);
        }
    };
}

#endif /* tuo_h */
