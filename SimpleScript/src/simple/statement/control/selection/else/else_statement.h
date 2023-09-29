//
//  else_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/18/23.
//

#ifndef else_statement_h
#define else_statement_h

#include "control_statement.h"

namespace simple {
    class else_statement: public control_statement {
        //  MEMBER FIELDS
        
        bool should_break;
    public:
        //  CONSTRUCTORS
        
        else_statement(const size_t statementc, statement_t** statementv);
        
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

#endif /* else_statement_h */
