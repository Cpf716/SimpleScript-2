//
//  uao.h
//  SimpleScript
//
//  Created by Corey Ferguson on 10/29/22.
//

#ifndef uao_h
#define uao_h

#include "operator_t.h"

namespace ss {
    class uao : public operator_t {
        //  MEMBER FIELDS
        
        std::function<const double(const double)> operation;
    public:
        //  CONSTRUCTORS
        
        uao(const string opcode, const std::function<double(const double)> operation) {
            set_opcode(opcode);
            
            this->operation = operation;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        double apply(const double rhs) const {
            return operation(rhs);
        }
    };
}

#endif /* uao_h */
