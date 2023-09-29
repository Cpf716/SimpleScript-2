//
//  for_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 5/10/23.
//

#ifndef for_statement_h
#define for_statement_h

#include "control_statement.h"

namespace simple {
    class for_statement: public control_statement {
        //  MEMBER FIELDS
        
        size_t expressionc;
        string* expressionv = NULL;
        
        bool should_break;
        bool should_continue;
        
        string* valuev = NULL;
    public:
        //  CONSTRUCTORS
        
        for_statement(const string specifier, const size_t statementc, statement_t** statementv);
        
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

#endif /* for_statement_h */
