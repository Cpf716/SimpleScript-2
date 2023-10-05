//
//  for_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/12/23.
//

#include "for_statement.h"

namespace ss {
    //  CONSTRUCTORS

    for_statement::for_statement(const string specificer, const size_t statementc, statement_t** statementv) {
        string tokenv[specificer.length() * 2 + 1];
        size_t tokenc = tokens(tokenv, specificer);
        
        size_t e = 0, s = e;   int p = 0;
        for (; e < tokenc; ++e) {
            if (tokenv[e] == "(")
                ++p;
            else if (tokenv[e] == ")")
                --p;
            else if (!p && tokenv[e] == ",") {
                for (size_t i = e; i > s + 1; --i) {
                    tokenv[s] += " " + tokenv[s + 1];
                    
                    for (size_t j = s + 1; j < tokenc - 1; ++j)
                        swap(tokenv[j], tokenv[j + 1]);
                    
                    --tokenc;    --e;
                }
                
                s = e + 1;
            }
        }
        
        for (size_t i = e; i > s + 1; --i) {
            tokenv[s] += " " + tokenv[s + 1];
            
            for (size_t j = s + 1; j < tokenc - 1; ++j)
                swap(tokenv[j], tokenv[j + 1]);
            
            --tokenc;
        }
        
        if (tokenc == 1) {
            tokenc = tokens(tokenv, tokenv[0]);
            
            if (tokenc < 3 || !is_symbol(tokenv[0]) || tolower(tokenv[1]) != "in")
                expect_error("',' in 'for' statement specifier");
            
            expressionv = new string[expressionc = 2];
            
            expressionv[0] = tokenv[0];
            expressionv[1] = tokenv[2];
            
            for (size_t i = 3; i < tokenc; ++i)
                expressionv[1] += " " + tokenv[i];
        } else {
            for (size_t i = 0, n = (size_t)floor(tokenc / 2) + 1; i < n; ++i) {
                if (tokenv[i * 2] == ",") {
                    tokenv[tokenc] = EMPTY;
                    
                    for (size_t j = tokenc; j > i * 2; --j)
                        swap(tokenv[j], tokenv[j - 1]);
                    
                    ++tokenc;
                }
            }
            
            if (tokenv[tokenc - 1] == ",")
                tokenv[tokenc++] = EMPTY;
            
            if (tokenc < 5)
                expect_error("',' in 'for' statement specifier");
            
            if (tokenc > 5)
                expect_error("';' after expression");
            
            expressionv = new string[expressionc = 3];
            
            for (size_t i = 0; i < 3; ++i)
                expressionv[i] = tokenv[i * 2];
        }
        
        if (statementc && is_clause(statementv[statementc - 1]))
            expect_error("expression");
        
        this->statementc = statementc;
        this->statementv = statementv;
    }

    void for_statement::close() {
        delete[] expressionv;
        
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->close();
        
        delete[] statementv;
        
        if (valuev != NULL)
            delete[] valuev;
        
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool for_statement::analyze(interpreter* ssu) const {
        if (!statementc) {
            cout << "'for' statement has empty body\n";
            
            return false;
        }
        
        size_t i = 0;
        while (i < statementc - 1 && !statementv[i]->analyze(ssu))
            ++i;
        
        if (i != statementc - 1)
            cout << "Unreachable code\n";
                
        if (statementv[i]->analyze(ssu) &&
            (statementv[i]->compare("break") ||
             statementv[i]->compare("return")))
            cout << "'for' statement will execute at most once\n";
        
        return false;
    }

    bool for_statement::compare(const string val) const { return false; }

    string for_statement::evaluate(interpreter* ssu) {
        unsupported_error("evaluate()");
        
        return EMPTY;
    }

    string for_statement::execute(interpreter* ssu) {
        should_break = false;
        
        string buid = ssu->backup();
        size_t valuec = 0;
        
        if (expressionc == 2) {
            if (ssu->is_defined(expressionv[0]))
                ssu->drop(expressionv[0]);
            
            string result = ssu->evaluate(expressionv[1]);
            
            valuev = new string[result.length() + 1];
            valuec = parse(valuev, result);
        } else {
            string tokenv[expressionv[0].length() + 1];
            size_t tokenc = tokens(tokenv, expressionv[0]);
            
            size_t i = 0;
            while (i < tokenc && tokenv[i] == "(")
                ++i;
            
            if (i < tokenc && tolower(tokenv[i]) == "const")
                ++i;
            
            if (i < tokenc && tolower(tokenv[i]) == "array")
                ++i;
            
            if (i < tokenc - 1 && is_symbol(tokenv[i]) && tokenv[i + 1] == "=" && ssu->is_defined(tokenv[i])) {
                valuev = new string[valuec = 1];
                valuev[0] = tokenv[i];
                
                ssu->drop(valuev[0]);
            }
            
            //  available to every iteration
            ssu->evaluate(expressionv[0]);
        }
        
        size_t index = 0;
        while (1) {
            string _buid = ssu->backup();
            
            if (expressionc == 2) {
                if (index == valuec) {
                    ssu->restore(_buid, true, 1, expressionv);
                    
                    break;
                }
                
                if (valuev[index].empty() || is_string(valuev[index]))
                    ssu->set_string(expressionv[0], valuev[index]);
                else
                    ssu->set_number(expressionv[0], stod(valuev[index]));
                
                ++index;
                
            } else if (!expressionv[1].empty() && !ss::evaluate(ssu->evaluate(expressionv[1]))) {
                //  available for one iteration
                ssu->restore(_buid);
                
                break;
            }
            
            should_continue = false;
            
            for (size_t j = 0; j < statementc; ++j) {
                statementv[j]->execute(ssu);
                
                if (should_break || should_continue)
                    break;
            }
            
            if (should_break) {
                if (expressionc == 2)
                    ssu->restore(_buid, true, 1, expressionv);
                else
                    ssu->restore(_buid);
                
                break;
            }
            
            if (expressionc == 2)
                ssu->restore(_buid, true, 1, expressionv);
            else {
                //  available once
                ssu->evaluate(expressionv[2]);
                ssu->restore(_buid);
            }
        }
        
        if (expressionc == 2 || !valuec)
            ssu->restore(buid);
        else
            ssu->restore(buid, true, 1, valuev);
        
        if (valuev != NULL) {
            delete[] valuev;
            
            valuev = NULL;
        }
        
        return EMPTY;
    }

    void for_statement::set_break() { should_break = true; }

    void for_statement::set_continue() { should_continue = true; }

    void for_statement::set_return(const string result) {
        should_break = true;
        parent->set_return(result);
    }
}
