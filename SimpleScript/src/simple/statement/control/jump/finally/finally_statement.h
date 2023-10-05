//
//  finally_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/26/23.
//

#ifndef finally_statement_h
#define finally_statement_h

#include "control_statement.h"

namespace ss {
    class finally_statement: public control_statement {
        //  MEMBER FIELDS
        
        bool should_break;
    public:
        //  CONSTRUCTORS
        
        finally_statement(const size_t statementc, statement_t** statementv);
        
        void close();
        
        //  MEMBER FUNCTIONS
        
        bool analyze(interpreter* ssu) const;
        
        bool compare(const string val) const;
        
        string evaluate(interpreter* ssu);
        
        string execute(interpreter* ssu);
        
        void set_break();
        
        void set_continue();
        
        void set_return(const string result);
    };
}

#endif /* finally_statement_h */
