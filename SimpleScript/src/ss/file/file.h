//
//  file.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/27/23.
//

#ifndef file_h
#define file_h

#include "assert_statement.h"
#include "break_statement.h"
#include "catch_statement.h"
#include "consume_statement.h"
#include "continue_statement.h"
#include "do_while_statement.h"
#include "echo_statement.h"
#include "else_statement.h"
#include "else_if_statement.h"
#include "exception_statement.h"
#include "finally_statement.h"
#include "for_statement.h"
#include "function_statement.h"
#include "function_t.h"
#include "if_statement.h"
#include "node.h"
#include "return_statement.h"
#include "sleep_statement.h"
#include "statement.h"
#include "statement_t.h"
#include "try_statement.h"
#include "while_statement.h"

using namespace ss;

namespace ss {
    class file: public function_t, statement_t {
        //  MEMBER FIELDS
        
        string filename;
        
        size_t functionc = 0;
        pair<file*, bool>** functionv = NULL;
        
        string result = encode("undefined");
        
        bool should_return;
        
        interpreter* ssu = NULL;
        
        size_t statementc = 0;
        statement_t** statementv = NULL;
        
        //  MEMBER FUNCTIONS
        
        size_t build(statement_t** dst, string* src, size_t si, size_t ei);
        
        ss::array<string> marshall(const size_t argc, string* argv) const;
    public:
        //  CONSTRUCTORS
        
        file(const string filename, node<string>* parent, interpreter* ssu);
        
        void close();
        
        //  MEMBER FUNCTIONS
        
        bool analyze(interpreter* ssu) const;
        
        string call(const size_t argc, string* argv);

        bool compare(const string val) const;

        string evaluate(interpreter* ssu);

        string execute(interpreter* ssu);

        void set_break();

        void set_continue();

        void set_parent(statement_t* parent);

        void set_return(const string result);
    };
}

#endif /* file_h */
