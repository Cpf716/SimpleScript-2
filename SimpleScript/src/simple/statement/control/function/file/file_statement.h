//
//  file_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/20/23.
//

#ifndef file_statement_h
#define file_statement_h

#include "assert_statement.h"
#include "break_statement.h"
#include "catch_statement.h"
#include "continue_statement.h"
#include "do_while_statement.h"
#include "echo_statement.h"
#include "else_statement.h"
#include "else_if_statement.h"
#include "exception_statement.h"
#include "finally_statement.h"
#include "for_statement.h"
#include "function_statement.h"
#include "if_statement.h"
#include "return_statement.h"
#include "statement.h"
#include "consume_statement.h"
#include "try_statement.h"
#include "while_statement.h"
#include "sleep_statement.h"

namespace ss {
    class file_statement: public statement_t {
        //  MEMBER FIELDS
        
        size_t functionc;
        function_t** functionv = NULL;
        
        string result = encode("undefined");
        
        bool should_return;
        
        size_t statementc = 0;
        statement_t** statementv = NULL;
        
        //  MEMBER FUNCTIONS
        
        size_t build(statement_t** dst, string* src, size_t si, size_t ei);
    public:
        //  CONSTRUCTORS
        
        file_statement(const size_t n, string* src, const size_t functionc, function_t** functionv);
        
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
