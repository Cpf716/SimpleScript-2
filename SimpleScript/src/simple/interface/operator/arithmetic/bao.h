//
//  bao.h
//  SimpleScript
//
//  Created by Corey Ferguson on 10/28/22.
//

#ifndef bao_h
#define bao_h

#include "bao_t.h"

namespace ss {
    class bao : public bao_t {
        //  MEMBER FIELDS
        std::function<double(const double, const double)> operation;
        
    public:
        //  CONSTRUCTORS
        
        bao(const string opcode, const std::function<double(const double, const double)> operation) {
            set_opcode(opcode);
            this->operation = operation;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        double apply(const double lhs, const double rhs) const {
            return operation(lhs, rhs);
        }
    };
}

#endif /* bao_h */
