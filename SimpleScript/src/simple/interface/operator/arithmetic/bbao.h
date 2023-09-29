//
//  bbao.h
//  SimpleScript
//
//  Created by Corey Ferguson on 10/28/22.
//

#ifndef bbao_h
#define bbao_h

#include "bao_t.h"
#include "utility.h"

namespace simple {
    class bbao: public bao_t {
        //  MEMBER FIELDS
        
        std::function<double(double, double)> operation;
    public:
        //  CONSTRUCTORS
        
        bbao(const string opcode, const std::function<double(double, double)> operation) {
            this -> set_opcode(opcode);
            this -> operation = operation;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        double apply(const double lhs, const double rhs) const {
            if (is_int(lhs) || is_int(rhs))
                type_error("double", "int");
            
            return operation(lhs, rhs);
        }
    };
}

#endif /* bbao_h */
