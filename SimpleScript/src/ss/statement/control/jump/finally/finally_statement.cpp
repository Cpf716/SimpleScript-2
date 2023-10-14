//
//  finally_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/17/23.
//

#include "finally_statement.h"

namespace ss {
    //  CONSTRUCTORS

    finally_statement::finally_statement(const size_t statementc, statement_t** statementv) {
        if (statementc && (statementv[statementc - 1]->compare("else") ||
            statementv[statementc - 1]->compare("elseif")))
            expect_error("expression");
        
        this->statementc = statementc;
        this->statementv = statementv;
    }

    void finally_statement::close() {
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool finally_statement::analyze(interpreter* ssu) const {
        cout << "statementc:\t" << statementc << endl;
        
        if (!statementc) {
            cout << "'finally' statement has empty body\n";
            
            return false;
        }
        
        size_t i = 0;
        while (i < statementc - 1 && !statementv[i]->analyze(ssu))
            ++i;
        
        if (i != statementc - 1)
            cout << "Unreachable code\n";
        
        statementv[statementc - 1]->analyze(ssu);
        
        return false;
    }

    bool finally_statement::compare(const string val) const { return val == "finally"; }

    string finally_statement::evaluate(interpreter* ssu) {
        unsupported_error("evaluate()");
        return EMPTY;
    }

    string finally_statement::execute(interpreter* ssu) {
        should_break = true;
        
        string buid = ssu->backup();
        
        for (size_t i = 0; i < statementc; ++i) {
            statementv[i]->execute(ssu);
            
            if (should_break)
                break;
        }
        
        ssu->restore(buid);
        
        return EMPTY;
    }

    void finally_statement::set_break() {
        should_break = true;
        parent->set_break();
    }

    void finally_statement::set_continue() {
        should_break = true;
        parent->set_continue();
    }

    void finally_statement::set_return(const string result) {
        should_break = true;
        parent->set_return(result);
    }
}
