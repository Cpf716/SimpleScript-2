//
//  try_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/17/23.
//

#include "try_statement.h"

namespace ss {
    //  CONSTRUCTORS

    try_statement::try_statement(const size_t statementc, statement_t** statementv) {
        index = 0;
        while (index < statementc && !statementv[index]->compare("catch"))
            ++index;
        
        if (index == statementc)
            expect_error("'catch'");
        
        if (index == statementc - 1) {
            if (statementc == 1)
                logger_write("'try' statement has empty body\n");
            
        } else if (statementc == 2)
            logger_write("'try' statement has empty body\n");
        
        this->statementc = statementc;
        this->statementv = statementv;
    }

    void try_statement::close() {
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool try_statement::analyze(interpreter* ssu) const {
        if (index) {
            size_t i = 0;
            while (i < index - 1 && !statementv[i]->analyze(ssu))
                ++i;
            
            if (i != index - 1)
                logger_write("Unreachable code\n");
            
            statementv[index - 1]->analyze(ssu);
        }
        
        for (size_t i = index; i < statementc; ++i)
            statementv[i]->analyze(ssu);
        
        return false;
    }

    bool try_statement::compare(const string value) const { return false; }

    string try_statement::evaluate(interpreter* ssu) {
        unsupported_error("evaluate()");
        return EMPTY;
    }

    string try_statement::execute(interpreter* ssu) {
        try {
            should_break = false;
            
            string buid = ssu->backup();
            
            for (size_t i = 0; i < index; ++i) {
                statementv[i]->execute(ssu);
                
                if (should_break)
                    break;
            }
            
            ssu->restore(buid);
            
        } catch (exception& e) {
            size_t i = statementc - 1;
            
            if (statementv[i]->compare("finally"))
                --i;
            
            string buid = ssu->backup();
            string symbol = decode(statementv[i]->evaluate(ssu));
            
            if (ssu->is_defined(symbol))
                ssu->drop(symbol);
            
            ssu->set_string(symbol, encode(e.what()));
            
            statementv[i]->execute(ssu);
            
            ssu->drop(symbol);
            
            string symbolv[1];
            
            symbolv[0] = symbol;
            
            ssu->restore(buid, true, 1, symbolv);
            
            if (i != statementc - 1)
                statementv[statementc - 1]->execute(ssu);
        }
        
        return EMPTY;
    }

    void try_statement::set_break() {
        should_break = true;
        parent->set_break();
    }

    void try_statement::set_continue() {
        should_break = true;
        parent->set_continue();
    }

    void try_statement::set_return(const string result) {
        should_break = true;
        parent->set_return(result);
    }
}
