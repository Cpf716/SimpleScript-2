//
//  if_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/18/23.
//

#ifndef if_statement_h
#define if_statement_h

#include "control_statement.h"
#include "else_if_statement.h"
#include "else_statement.h"

namespace simple {
    class if_statement: public control_statement {
        //  MEMBER FIELDS
        
        string expression;
        
        int index;
        
        bool should_break;
    public:
        //  CONSTRUCTORS
        
        if_statement(const string expression, const size_t statementc, statement_t** statementv);
        
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

#endif /* if_statement_h */
