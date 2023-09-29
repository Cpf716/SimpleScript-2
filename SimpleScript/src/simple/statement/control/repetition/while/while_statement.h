//
//  while_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/18/23.
//

#ifndef while_statement_h
#define while_statement_h

#include "control_statement.h"

namespace simple {
    class while_statement: public control_statement {
        //  MEMBER FIELDS
        
        string expression;
        
        bool should_break;
        bool should_continue;
    public:
        //  CONSTRUCTORS
        
        while_statement(const string expression, const size_t statementc, statement_t** statementv);
        
        //  MEMBER FUNCTIONS
        
        void close();
        
        string evaluate(interpreter* ss);
        
        string execute(interpreter* ss);
        
        bool compare(const string val) const;
        
        void set_break();
        
        void set_continue();
        
        void set_return(const string result);
        
        bool validate(interpreter* ss) const;
    };
}

#endif /* while_statement_h */
