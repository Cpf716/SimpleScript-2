//
//  function_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 5/7/23.
//

#ifndef function_statement_h
#define function_statement_h

#include "control_statement.h"

namespace simple {
    class function_statement: public control_statement, function_t {
        //  MEMBER FIELDS
        
        size_t expressionc;
        string* expressionv = NULL;
        
        size_t optionalc = 0;
        
        string result = EMPTY;
        
        bool should_return;
        
        interpreter* ss = NULL;
        
        //  MEMBER FUNCTIONS
        
        void set_value(const string symbol, const string value);
    public:
        //  CONSTRUCTORS
        
        function_statement(const string specifier, const size_t statementc, statement_t** statementv);
        
        void close();
        
        //  MEMBER FUNCTIONS
        
        string call(const size_t argc, string* argv);
        
        bool compare(const string val) const;
        
        string evaluate(interpreter* ss);
        
        string execute(interpreter* ss);
        
        void set_break();
        
        void set_continue();
        
        void set_return(const string result);
        
        bool validate(interpreter* ss) const;
    };
}

#endif /* function_statement_h */
