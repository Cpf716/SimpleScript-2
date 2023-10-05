//
//  if_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/17/23.
//

#include "if_statement.h"

namespace ss {
    if_statement::if_statement(const string expression, const size_t statementc, statement_t** statementv) {
        if (expression.empty())
            expect_error("expression");
                
        this->expression = expression;
        
        if (statementc && (statementv[statementc - 1]->compare("catch") ||
            statementv[statementc - 1]->compare("finally")))
            expect_error("expression");
        
        index = 0;
        while (index < statementc && !statementv[index]->compare("elseif") && !statementv[index]->compare("else"))
            ++index;
        
        this->statementc = statementc;
        this->statementv = statementv;
    }

    void if_statement::close() {
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool if_statement::analyze(interpreter* ssu) const {
        if (index) {
            size_t i = 0;
            while (i < index - 1 && !statementv[i]->analyze(ssu))
                ++i;
            
            if (i != index - 1)
                cout << "Unreachable code\n";
            
            statementv[index - 1]->analyze(ssu);
        }
        
        for (size_t i = index; i < statementc; ++i)
            statementv[i]->analyze(ssu);
        
        if (!index)
            cout << "'if' statement has empty body\n";
        
        return false;
    }

    bool if_statement::compare(const string val) const { return false; }

    string if_statement::evaluate(interpreter* ssu) {
        unsupported_error("evaluate()");
        return EMPTY;
    }

    string if_statement::execute(interpreter* ssu) {
        string buid = ssu->backup();
        
        if (ss::evaluate(ssu->evaluate(expression))) {
            should_break = false;
            
            for (size_t i = 0; i < index; ++i) {
                statementv[i]->execute(ssu);
                
                if (should_break)
                    break;
            }
            
            ssu->restore(buid);
        } else {
            ssu->restore(buid);
            
            size_t i;
            for (i = index; i < statementc; ++i) {
                buid = ssu->backup();
                
                bool flag = !statementv[i]->compare("elseif") || ss::evaluate(statementv[i]->evaluate(ssu));
                
                if (flag)
                    statementv[i]->execute(ssu);
                
                ssu->restore(buid);
                
                if (flag)
                    break;
            }
        }
        
        return EMPTY;
    }

    void if_statement::set_break() {
        should_break = true;
        parent->set_break();
    }

    void if_statement::set_continue() {
        should_break = true;
        parent->set_continue();
    }

    void if_statement::set_return(const string result) {
        should_break = true;
        parent->set_return(result);
    }
}
