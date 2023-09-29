//
//  if_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/17/23.
//

#include "if_statement.h"

namespace simple {
    if_statement::if_statement(const string expression, const size_t statementc, statement_t** statementv) {
        if (expression.empty())
            expect("expression");
                
        this -> expression = expression;
        
        if (statementc && (statementv[statementc - 1] -> compare("catch") ||
            statementv[statementc - 1] -> compare("finally")))
            expect("expression");
        
        index = 0;
        while (index < statementc && !statementv[index] -> compare("elseif") && !statementv[index] -> compare("else"))
            ++index;
        
        this -> statementc = statementc;
        this -> statementv = statementv;
    }

    void if_statement::close() {
        for (size_t i = 0; i < statementc; ++i)
            statementv[i] -> close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool if_statement::compare(const string val) const { return false; }

    string if_statement::evaluate(interpreter* ss) {
        unsupported_error("evaluate()");
        return EMPTY;
    }

    string if_statement::execute(interpreter* ss) {
        string buid = ss -> backup();
        
        if (simple::evaluate(ss -> evaluate(expression))) {
            should_break = false;
            
            for (size_t i = 0; i < index; ++i) {
                statementv[i] -> execute(ss);
                
                if (should_break)
                    break;
            }
            
            ss -> restore(buid);
        } else {
            ss -> restore(buid);
            
            size_t i;
            for (i = index; i < statementc; ++i) {
                buid = ss -> backup();
                
                bool flag = !statementv[i] -> compare("elseif") || simple::evaluate(statementv[i] -> evaluate(ss));
                
                if (flag)
                    statementv[i] -> execute(ss);
                
                ss -> restore(buid);
                
                if (flag)
                    break;
            }
        }
        
        return EMPTY;
    }

    void if_statement::set_break() {
        should_break = true;
        parent -> set_break();
    }

    void if_statement::set_continue() {
        should_break = true;
        parent -> set_continue();
    }

    void if_statement::set_return(const string result) {
        should_break = true;
        parent -> set_return(result);
    }

    bool if_statement::validate(interpreter* ss) const {
        if (index) {
            size_t i = 0;
            while (i < index - 1 && !statementv[i] -> validate(ss))
                ++i;
            
            if (i != index - 1)
                cout << "Unreachable code\n";
            
            statementv[index - 1] -> validate(ss);
        }
        
        for (size_t i = index; i < statementc; ++i)
            statementv[i] -> validate(ss);
        
        if (!index)
            cout << "'if' statement has empty body\n";
        
        return false;
    }
}
