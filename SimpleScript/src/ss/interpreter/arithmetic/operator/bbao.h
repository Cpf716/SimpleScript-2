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

namespace ss {
    class bbao: public bao_t {
        //  MEMBER FIELDS
        
        std::function<double(double, double)> opr;
    public:
        //  CONSTRUCTORS
        
        bbao(const string opc, const std::function<double(double, double)> opr) {
            set_opcode(opc);
            
            this->opr = opr;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        double apply(const double lhs, const double rhs) const {
            if (is_int(lhs) || is_int(rhs))
                type_error("double", "int");
            
            return opr(lhs, rhs);
        }
    };
}

#endif /* bbao_h */
