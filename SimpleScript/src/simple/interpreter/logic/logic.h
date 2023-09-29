//
//  logic.h
//  SimpleScript
//
//  Created by Corey Ferguson on 10/29/22.
//

#ifndef logic_h
#define logic_h

#include "arithmetic.h"
#include "blo.h"
#include "ulo.h"

using namespace std;

namespace simple {
    class logic: public arithmetic {
        //  MEMBER FUNCTIONS
        
        void analyze(const string* data, const size_t n) const;
        
        size_t merge(string* data, int n) const;
        
        size_t prefix(const string expr, string* data) const;
        
        size_t split(const string expr, string* data) const;
    protected:
        //  MEMBER FIELDS
        
        size_t loc = 4;
        operator_t** lov = NULL;
    public:
        //  CONSTRUCTORS
        
        logic();
        
        ~logic();
        
        //  MEMBER FUNCTIONS
        
        double evaluate(const string expression);
    };
}

#endif /* logic_h */
