//
//  file_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 10/14/23.
//

#ifndef file_statement_h
#define file_statement_h

#include "function_t.h"
#include "statement_t.h"

namespace ss {
    class file_statement: public statement_t {
        //  MEMBER FIELDS
        
        string result;
        
        bool should_return;
        
        size_t statementc;
        statement_t** statementv = NULL;
    public:
        //  CONSTRUCTORS
        
        file_statement(const size_t statementc, statement_t** statementv);
        
        void close();
        
        //  MEMBER FUNCTIONS
        
        bool analyze(interpreter* ssu) const;

        bool compare(const string val) const;

        string evaluate(interpreter* ssu);

        string execute(interpreter* ssu);

        void set_break();

        void set_continue();

        void set_parent(statement_t* parent);

        void set_return(const string result);
    };
}

#endif /* file_statement_h */
