//
//  file.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/11/23.
//

#include "file.h"

namespace ss {
    //  CONSTRUCTORS

    file::file(const string filename, node<string>* parent, interpreter* ssu) {
        ifstream file;
        file.open(filename);
        
        if (!file.is_open())
            throw error("No such file: " + filename);
        
        this->filename = filename;
        
        string _filename = ::filename(filename);
        
        if (!is_symbol(_filename))
            expect_error("symbol");
        
        this->rename(_filename);
        
        size_t n = 0;
        string* src = new string[1];
        
        //  line breaks take supreme precedence
        string line;
        while (getline(file, line)) {
            string tokenv[line.length() + 1];
            size_t tokenc = tokenize(tokenv, line, "//");
            
            tokenc = merge(tokenc, tokenv, "");
            
            if (tokenc == 0)
                continue;
            
            size_t i = 0;
            while (i < tokenc && tokenv[i] != "//")
                ++i;
            
            if (i == 0)
                continue;
            
            while (tokenc > i)
                --tokenc;
            
            line = trim(tokenv[0]);
            
            for (size_t i = 1; i < tokenc; ++i)
                line += trim(tokenv[i]);
            
            if (is_pow(n, 2))
                src = resize(n, n * 2, src);
            
            src[n++] = line;
        }
        
        //  block comments are third
        size_t i = 0;
        while (i < n) {
            string* tokenv = new string[pow2(src[i].length() + 1)];
            size_t tokenc = tokenize(tokenv, src[i], "/*");
            
            tokenc = merge(tokenc, tokenv, "/*");
            
            size_t j = 0;
            while (j < tokenc) {
                string _tokenv[tokenv[j].length() + 1];
                size_t _tokenc = tokenize(_tokenv, tokenv[j], "*/");
                
                _tokenc = merge(_tokenc, _tokenv, "*/");
                
                tokenv[j] = trim(_tokenv[0]);
                
                for (size_t k = 1; k < _tokenc; ++k) {
                    if (is_pow(tokenc, 2))
                        tokenv = resize(tokenc, tokenc * 2, tokenv);
                    
                    tokenv[tokenc] = trim(_tokenv[k]);
                    
                    for (size_t l = tokenc; l > j + k; --l)
                        swap(tokenv[l], tokenv[l - 1]);
                    
                    ++tokenc;
                }
                
                j += _tokenc;
            }
            
            size_t _tokenc = tokenc;
            
            j = 0;
            while (j < tokenc) {
                if (tokenv[j] == "/*") {
                    size_t k = j + 1;
                    while (k < tokenc && tokenv[k] != "*/")
                        ++k;
                    
                    if (k == tokenc)
                        break;
                    else {
                        for (size_t l = j; l <= k; ++l) {
                            for (size_t m = j; m < tokenc - 1; ++m)
                                swap(tokenv[m], tokenv[m + 1]);
                            
                            --tokenc;
                        }
                    }
                } else
                    ++j;
            }
            
            if (tokenc != _tokenc) {
                while (tokenc > 1) {
                    tokenv[0] += tokenv[1];
                    
                    for (j = 1; j < tokenc - 1; ++j)
                        swap(tokenv[j], tokenv[j + 1]);
                    
                    --tokenc;
                }
            }
            
            if (!tokenc) {
                for (size_t j = i; j < n - 1; ++j)
                    swap(src[j], src[j + 1]);
                
                --n;
            } else {
                src[i] = tokenv[0];
                
                for (j = 1; j < tokenc; ++j) {
                    if (is_pow(n, 2))
                        src = resize(n, n * 2, src);
                    
                    src[n] = tokenv[j];
                    
                    for (size_t k = n; k > i + j; --k)
                        swap(src[k], src[k - 1]);
                    
                    ++n;
                }
                
                i += tokenc;
            }
        }
        
        i = 0;
        while (i < n) {
            if (src[i] == "/*") {
                size_t j = i + 1;
                while (j < n && src[j] != "*/")
                    ++j;
                
                if (j == n)
                    --j;
                
                for (size_t k = i; k <= j; ++k) {
                    for (size_t l = i; l < n - 1; ++l)
                        swap(src[l], src[l + 1]);
                    
                    --n;
                }
            } else
                ++i;
        }
        
        i = 0;
        while (i < n && src[i] != "*/")
            ++i;
        
        if (i != n)
            expect_error("expression");
        
        //  semicolons are fourth
        i = 0;
        while (i < n) {
            string tokenv[src[i].length() + 1];
            size_t tokenc = parse(tokenv, src[i], ";");
             
            size_t j = 0;
            while (j < tokenc) {
                if (tokenv[j].empty()) {
                    for (size_t k = j; k < tokenc - 1; ++k)
                        swap(tokenv[k], tokenv[k + 1]);
                    
                    --tokenc;
                } else
                    ++j;
            }
            
            if (!j) {
                for (size_t k = i; k < n - 1; ++k)
                    swap(src[k], src[k + 1]);
                
                --n;
            } else {
                src[i] = tokenv[0];
                
                for (size_t j = 1; j < tokenc; ++j) {
                    if (is_pow(n, 2))
                        src = resize(n, n * 2, src);
                    
                    src[n] = tokenv[j];
                    
                    for (size_t k = n; k > i + j; --k)
                        swap(src[k], src[k - 1]);
                    
                    ++n;
                }
                
                i += tokenc;
            }
        }
        
        ::node<string>* node = new ::node<string>(filename, parent);
        
        ss::array<string> arr;
        
        string buid = ssu->backup();
        
        ssu->reload();
        
        functionv = new pair<class file*, bool>*[1];
        
        for (i = 0; i < n; ++i) {
            string tokenv[src[i].length() + 1];
            size_t tokenc = tokens(tokenv, src[i]);
            
            if (tokenv[0] == "include") {
                if (tokenc == 1)
                    expect_error("expression");
                
                src[i] = ltrim(src[i].substr(7));
                
                string result = ssu->evaluate(src[i]);
                
                string valuev[result.length() + 2];
                size_t valuec = parse(valuev, result);
                
                if (valuec > 2)
                    expect_error("1 argument(s), got " + to_string(valuec));
                
                for (size_t j = 0; j < valuec; ++j) {
                    if (valuev[j].empty())
                        null_error();
                    
                    if (!is_string(valuev[j]))
                        type_error("double", "string");
                    
                    valuev[j] = decode(valuev[j]);
                }
                
                if (valuec == 1)
                    valuev[valuec++] = ::filename(valuev[0]);
                
                else if (!is_symbol(valuev[1]))
                    undefined_error("\"" + valuev[1] + "\"");
                
                size_t j = 0;
                while (j < functionc && functionv[j]->first->name() != valuev[1])
                    ++j;
                
                if (j != functionc) {
                    if (arr.index_of(valuev[1]) == -1) {
                        cout << "'" << valuev[1] << "' is defined\n";
                        
                        arr.push(valuev[1]);
                    }

                    continue;
                }
                
                //  tree ensures file cannot include itself
                ::node<string>* _parent = parent;
                
                while (_parent != NULL) {
                    if (_parent->data() == node->data())
                        throw error(_parent->data());
                    
                    _parent = _parent->parent();
                }
                    
                if (is_pow(functionc, 2)) {
                    pair<::file*, bool>** tmp = new pair<::file*, bool>*[functionc * 2];
                    for (size_t k = 0; k < functionc; ++k)
                        tmp[k] = functionv[k];
                    
                    delete[] functionv;
                    functionv = tmp;
                }
                
                if (valuev[0].length() > 1 && valuev[0][0] == '@' && valuev[0][1] == '/')
                    valuev[0] = BASE_DIR + valuev[0].substr(2) + ".txt";
                
                string _buid = ssu->backup();
                
                ssu->reload();
                
                functionv[functionc] = new pair<::file*, bool>(new ::file(valuev[0], node, ssu), true);
                functionv[functionc]->first->rename(valuev[1]);
                
                ssu->restore(_buid);
                
                size_t _functionc = functionc++;
                for (j = 0; j < functionv[_functionc]->first->functionc; ++j) {
                    size_t k = 0;
                    while (k < functionc && functionv[_functionc]->first->functionv[j]->first->name() != functionv[k]->first->name())
                        ++k;
                    
                    if (k == functionc) {
                        if (is_pow(functionc, 2)) {
                            pair<::file*, bool>** tmp = new pair<::file*, bool>*[functionc * 2];
                            for (size_t l = 0; l < functionc; ++l)
                                tmp[l] = functionv[l];
                            
                            delete[] functionv;
                            functionv = tmp;
                        }
                        
                        functionv[functionc] = new pair<::file*, bool>(functionv[_functionc]->first->functionv[j]->first, false);
                        functionv[functionc]->first->consume();
                        
                        ++functionc;
                    } else
                        //  different instance than functionv[k]->first
                        functionv[_functionc]->first->functionv[j]->first->consume();
                }
                
                continue;
            }
                
            break;
        }
        
        ssu->restore(buid);
        
        for (; i > 0; --i) {
            for (size_t j = i - 1; j < n - 1; ++j)
                swap(src[j], src[j + 1]);
            
            --n;
        }
        
        if (!n) {
            if (!functionc)
                cout << "'file' statement has empty body\n";
            
            else
                //  configuration file
                consume();
        }
        
        statementv = new statement_t*[n];
        statementc = build(statementv, src, 0, n);
        
        delete[] src;
        
        if (statementc && is_clause(statementv[statementc - 1]))
            expect_error("expression");
        
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->set_parent(this);
        
        this->ssu = ssu;
    }

    void file::close() {
        for (size_t i = 0; i < functionc; ++i)
            if (functionv[i]->second)
                functionv[i]->first->close();
        
        delete[] functionv;
        
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool file::analyze(interpreter* ssu) const {
        if (!statementc)
            return false;
        
        size_t i = 0;
        while (i < statementc - 1 && !statementv[i]->analyze(ssu))
            ++i;
        
        if (i != statementc - 1)
            cout << "Unreachable code\n";
        
        statementv[statementc - 1]->analyze(ssu);
        
        return false;
    }

    size_t file::build(statement_t** dst, string* src, size_t si, size_t ei) {
        size_t i = si, s = 0;
        while (i < ei) {
            string* tokenv = new string[src[i].length() + 1];
            size_t tokenc = tokens(tokenv, src[i]);
            
            if (tokenv[0] == "include") {
                delete[] tokenv;
                expect_error("expression");
            }
            
            if (tokenv[0] == "catch") {
                delete[] tokenv;
                
                if (tokenc > 2)
                    expect_error("';' after expression");
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "try") {
                        delete[] tokenv;
                        ++p;
                    } else {
                        if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "try") {
                            delete[] tokenv;
                            
                            --p;
                            
                            if (!p)
                                break;
                        } else if (p == 1) {
                            if (tokenv[0] == "catch") {
                                delete[] tokenv;
                                expect_error("'end try'");
                            }
                            
                            if (tokenv[0] == "finally") {
                                delete[] tokenv;
                                break;
                            }
                            
                            delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                }
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new catch_statement(ltrim(src[i].substr(5)), _s, _dst);
                i = k;
            } else if (tokenc > 1 && tokenv[0] == "do" && tokenv[1] == "while") {
                delete[] tokenv;
                
                size_t j;
                for (j = 0; j <= src[i].length() - 5; ++j) {
                    size_t k = 0;
                    while (k < 5 && src[i][j + k] == ("while")[k])
                        ++k;
                    
                    if (k == 5)
                        break;
                }
                
                j += 5;
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "while") {
                        delete[] tokenv;
                        ++p;
                    } else {
                        if (tokenc > 1) {
                            if (tokenv[0] == "do" && tokenv[1] == "while") {
                                delete[] tokenv;
                                ++p;
                            } else if (tokenv[0] == "end" && tokenv[1] == "while") {
                                delete[] tokenv;
                                
                                if (tokenc > 2)
                                    expect_error("';' after expression");
                                
                                --p;
                                
                                if (!p) break;
                            } else
                                delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                }
                
                if (k == ei)
                    expect_error("'end while'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new do_while_statement(ltrim(src[i].substr(j)), _s, _dst);
                i = k + 1;
            } else if (tokenc > 1 && tokenv[0] == "else" && tokenv[1] == "if") {
                delete[] tokenv;
                
                size_t j;
                for (j = 0; j <= src[i].length() - 2; ++j) {
                    size_t k = 0;
                    while (k < 2 && src[i][j + k] == ("if")[k])
                        ++k;
                    
                    if (k == 2)
                        break;
                }
                
                j += 2;
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "if") {
                        delete[] tokenv;
                        ++p;
                    } else {
                        if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "if") {
                            delete[] tokenv;
                            
                            --p;
                            
                            if (!p)
                                break;
                        } else if (p == 1) {
                            if (tokenv[0] == "else") {
                                delete[] tokenv;
                                break;
                            } else
                                delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                    
                }
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new else_if_statement(ltrim(src[i].substr(j)), _s, _dst);
                i = k;
            } else if (tokenv[0] == "else") {
                delete[] tokenv;
                
                if (tokenc > 1)
                    expect_error("';' after expression");
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "if") {
                        delete[] tokenv;
                        ++p;
                    } else {
                        if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "if") {
                            delete[] tokenv;
                            
                            --p;
                            
                            if (!p)
                                break;
                        } else if (p == 1) {
                            if (tokenv[0] == "else") {
                                delete[] tokenv;
                                expect_error("'end if'");
                            }
                            
                            delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                }
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new else_statement(_s, _dst);
                i = k;
            } else if (tokenv[0] == "finally") {
                delete[] tokenv;
                
                if (tokenc > 1)
                    expect_error("';' after expression");
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    if (tokenv[0] == "try")
                        ++p;
                    else {
                        tokenv = new string[src[k].length() + 1];
                        tokenc = tokens(tokenv, src[k]);
                        
                        if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "try") {
                            delete[] tokenv;
                            
                            --p;
                            
                            if (!p)
                                break;
                        } else if (p == 1) {
                            if (tokenv[0] == "catch" || tokenv[0] == "finally") {
                                delete[] tokenv;
                                expect_error("'end try'");
                            }
                            
                            delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                }
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new finally_statement(_s, _dst);
                i = k;
            } else if (tokenv[0] == "for") {
                delete[] tokenv;
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenc > 1 && tokenv[0] == "for") {
                        delete[] tokenv;
                        ++p;
                    } else if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "for") {
                        delete[] tokenv;
                        
                        if (tokenc > 2)
                            expect_error("';' after expression");
                        
                        --p;
                        
                        if (!p)
                            break;
                    } else
                        delete[] tokenv;
                }
                
                if (k == ei)
                    expect_error("'end for'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new for_statement(ltrim(src[i].substr(3)), _s, _dst);
                
                i = k + 1;
            } else if (tokenv[0] == "func") {
                delete[] tokenv;
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "func") {
                        delete[] tokenv;
                        ++p;
                    } else if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "func") {
                        delete[] tokenv;
                        
                        if (tokenc > 2)
                            expect_error("';' after expression");
                        
                        --p;
                        
                        if (!p)
                            break;
                    } else
                        delete[] tokenv;
                }
                
                if (k == ei)
                    expect_error("'end func'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new function_statement(ltrim(src[i].substr(4)), _s, _dst);
                i = k + 1;
            } else if (tokenv[0] == "if") {
                delete[] tokenv;
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "if") {
                        delete[] tokenv;
                        ++p;
                    } else if (tokenc >= 2 && tokenv[0] == "end" && tokenv[1] == "if") {
                        delete[] tokenv;
                        
                        if (tokenc > 2)
                            expect_error("';' after expression");
                        
                        --p;
                        
                        if (!p)
                            break;
                    } else
                        delete[] tokenv;
                }
                
                if (k == ei)
                    expect_error("'end if'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new if_statement(ltrim(src[i].substr(2)), _s, _dst);
                
                i = k + 1;
            } else if (tokenv[0] == "try") {
                delete[] tokenv;
                
                if (tokenc > 1)
                    expect_error("';' after expression");
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "try") {
                        delete[] tokenv;
                        ++p;
                    } else if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "try") {
                        delete[] tokenv;
                        
                        if (tokenc > 2)
                            expect_error("';' after expression");
                        
                        --p;
                        
                        if (!p)
                            break;
                    } else
                        delete[] tokenv;
                }
                
                if (k == ei)
                    expect_error("'end try'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new try_statement(_s, _dst);
                i = k + 1;
            } else if (tokenv[0] == "while") {
                delete[] tokenv;
        
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "while") {
                        delete[] tokenv;
                        ++p;
                    } else {
                        if (tokenc > 1) {
                            if (tokenv[0] == "do" && tokenv[1] == "while") {
                                delete[] tokenv;
                                ++p;
                            } else if (tokenv[0] == "end" && tokenv[1] == "while") {
                                delete[] tokenv;
                                
                                if (tokenc > 2)
                                    expect_error("';' after expression");
                                
                                --p;
                                
                                if (!p)
                                    break;
                            } else
                                delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                }
                
                if (k == ei)
                    expect_error("'end while'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new while_statement(ltrim(src[i].substr(5)), _s, _dst);
                i = k + 1;
            } else {
                if (tokenv[0] == "assert") {
                    delete[] tokenv;
                    dst[s] = new assert_statement(ltrim(src[i].substr(6)));
                } else if (src[i] == "break") {
                    delete[] tokenv;
                    dst[s] = new break_statement();
                } else if (tokenv[0] == "consume") {
                    delete[] tokenv;
                    dst[s] = new consume_statement(ltrim(src[i].substr(8)));
                } else if (src[i] == "continue") {
                    delete[] tokenv;
                    dst[s] = new continue_statement();
                } else if (tokenv[0] == "echo") {
                    delete[] tokenv;
                    dst[s] = new echo_statement(ltrim(src[i].substr(4)));
                } else if (tokenv[0] == "return") {
                    delete[] tokenv;
                    dst[s] = new return_statement(ltrim(src[i].substr(6)));
                } else if (tokenv[0] == "sleep") {
                    delete[] tokenv;
                    dst[s] = new sleep_statement(ltrim(src[i].substr(5)));
                } else if (tokenv[0] == "throw") {
                    delete[] tokenv;
                    dst[s] = new exception_statement(ltrim(src[i].substr(5)));
                } else {
                    delete[] tokenv;
                    dst[s] = new statement(src[i]);
                }
                
                ++i;
                ++s;
            }
        }
        
        return s;
    }

    string file::call(const size_t argc, string* argv) {
        string buid = ssu->backup();
        
        ssu->reload();
        
        string _buid = ssu->backup();
        
        ssu->set_function(this);
        
        for (size_t i = 0; i < functionc; ++i)
            ssu->set_function(functionv[i]->first);
        
        ss::array<string> arr = marshall(argc, argv);
        
        for (size_t  i = 0; i < arr.size(); ++i)
            ssu->set_array("argv", i, arr[i]);
        
        ssu->evaluate("shrink argv");
        ssu->consume("argv");
        
        string result = execute(ssu);
        
        consume();
        
        ssu->restore(_buid);
        ssu->restore(buid);
        
        return result;
    }

    bool file::compare(const string val) const {
        unsupported_error("compare()");
        return false;
    }

    string file::evaluate(interpreter* ssu) {
        unsupported_error("evaluate()");
        return EMPTY;
    }

    string file::execute(interpreter* ssu) {
        analyze(ssu);
        
        should_return = false;
        
        for (size_t i = 0; i < statementc; ++i) {
            statementv[i]->execute(ssu);
            
            if (should_return)
                break;
        }
        
        return result;
    }

    ss::array<string> file::marshall(const size_t argc, string* argv) const {
        ss::array<string> data = ss::array<string>(argc * 2 + 1);
        
        size_t j = 1;
        for (size_t i = 0; i < argc; ++i) {
            string valuev[argv[i].length() + 1];
            size_t valuec = parse(valuev, argv[i]);
            
            if (valuec > j)
                j = valuec;
            
            data.push(to_string(valuec));
            
            for (size_t k = 0; k < valuec; ++k)
                data.push(valuev[k]);
        }
        
        for (size_t i = 0; i < argc; ++i) {
            size_t k = stoi(data[i * (j + 1)]);
            
            for (size_t l = 0; l < j - k; ++l)
                data.insert(i * (j + 1) + k + l + 1, EMPTY);
        }
        
        data.insert(0, to_string(j + 1));
        data.insert(1, to_string(1));
        data.insert(2, encode(filename));
        
        for (size_t k = 1; k < j; ++k)
            data.insert(k + 2, EMPTY);
        
        return data;
    }

    void file::set_break() {
        throw error("break cannot be used outside of a loop");
    }

    void file::set_continue() {
        throw error("continue cannot be used outside of a loop");
    }

    void file::set_parent(statement_t* parent) {
        unsupported_error("set_parent()");
    }

    void file::set_return(const string result) {
        this->result = result;
        this->should_return = true;
    }
}
