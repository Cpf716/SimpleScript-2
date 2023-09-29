//
//  try_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/26/23.
//

#ifndef try_statement_h
#define try_statement_h

#include "control_statement.h"

namespace simple {
    class try_statement: public control_statement {
        //  MEMBER FIELDS
        
        int index;
        
        bool should_break;
    public:
        //  CONSTRUCTORS
        
        try_statement(const size_t statementc, statement_t** statementv);
        
        void close();
        
        //  MEMBER FUNCTIONS
        
        bool compare(const string val) const;
        
        string evaluate(interpreter* ss);
        
        string execute(interpreter* ss);
        
        void set_break();

        void set_continue();
        
        void set_return(const string result);
        
        bool validate(interpreter* ss) const;
    };
}

#endif /* try_statement_h */
