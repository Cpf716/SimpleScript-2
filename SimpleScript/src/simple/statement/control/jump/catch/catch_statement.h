//
//  catch_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/26/23.
//

#ifndef catch_statement_h
#define catch_statement_h

#include "control_statement.h"

namespace simple {
    class catch_statement: public control_statement {
        //  MEMBER FIELDS
        
        string symbol;
        
        bool should_break;
    public:
        //  CONSTRUCTORS
        
        catch_statement(const string symbol, const size_t statementc, statement_t** statementv);
        
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

#endif /* catch_statement_h */
