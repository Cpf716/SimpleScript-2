//
//  blo.h
//  SimpleScript
//
//  Created by Corey Ferguson on 10/29/22.
//

#ifndef blo_h
#define blo_h

#include "operator_t.h"

namespace ss {
    class blo : public operator_t {
        //  MEMBER FIELDS
        
        std::function<double(const string, const string)> operation;
    public:
        //  CONSTRUCTORS
        
        blo(const string opcode, const std::function<double(const string, const string)> operation) {
            set_opcode(opcode);
            
            this->operation = operation;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        double apply(const string lhs, const string rhs) const { return operation(lhs, rhs); }
    };
}

#endif /* blo_h */
