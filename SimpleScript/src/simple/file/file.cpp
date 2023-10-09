//
//  file.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/11/23.
//

#include "file.h"

namespace ss {
    //  CONSTRUCTORS

    file::file(const string filepath, interpreter* ssu, node<string>* parent) {
        ifstream file;
        file.open(filepath);
        
        if (!file.is_open())
            throw error("No such file: " + filepath);
        
        this->filepath = filepath;
        
        string filename = ::filename(filepath);
        
        if (!is_symbol(filename))
            expect_error("symbol");
        
        this->rename(filename);
        
        string* src = new string[1];
        size_t n = 0;
        
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
        
        ::node<string>* node = new ::node<string>(filepath, parent);
        
        ss::array<string> arr;
        
        string buid = ssu->backup();
        
        ssu->reload();
        
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
                while (j < filec && filev[j]->first->name() != valuev[1])
                    ++j;
                
                if (j != filec) {
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
                    
                if (is_pow(filec, 2)) {
                    pair<::file*, bool>** tmp = new pair<::file*, bool>*[filec * 2];
                    for (size_t k = 0; k < filec; ++k)
                        tmp[k] = filev[k];
                    
                    delete[] filev;
                    filev = tmp;
                }
                
                if (valuev[0].length() > 1 && valuev[0][0] == '@' && valuev[0][1] == '/')
                    valuev[0] = BASE_DIR + valuev[0].substr(2) + ".txt";
                
                string _buid = ssu->backup();
                
                ssu->reload();
                
                filev[filec] = new pair<::file*, bool>(new ::file(valuev[0], ssu, node), true);
                filev[filec]->first->rename(valuev[1]);
                
                ssu->restore(_buid);
                
                size_t _filec = filec++;
                for (j = 0; j < filev[_filec]->first->filec; ++j) {
                    size_t k = 0;
                    while (k < filec && filev[_filec]->first->filev[j]->first->name() != filev[k]->first->name())
                        ++k;
                    
                    if (k == filec) {
                        if (is_pow(filec, 2)) {
                            pair<::file*, bool>** tmp = new pair<::file*, bool>*[filec * 2];
                            for (size_t l = 0; l < filec; ++l)
                                tmp[l] = filev[l];
                            
                            delete[] filev;
                            filev = tmp;
                        }
                        
                        filev[filec] = new pair<::file*, bool>(filev[_filec]->first->filev[j]->first, false);
                        filev[filec]->first->consume();
                        
                        ++filec;
                    } else
                        //  different instance than filev[k]->first
                        filev[_filec]->first->filev[j]->first->consume();
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
            if (!filec)
                cout << "'file' statement has empty body\n";
            
            else
                //  configuration file
                consume();
        }
        
        function_t** _filev = new function_t*[filec];
        
        for (i = 0; i < filec; ++i)
            _filev[i] = filev[i]->first;
        
        this->target = new file_statement(n, src, filec, _filev);
        this->ssu = ssu;
        
        delete[] src;
    }

    //  MEMBER FUNCTIONS

    string file::call(const size_t argc, string* argv) {        
        string buid = ssu->backup();
        
        ssu->reload();
        
        string _buid = ssu->backup();
        
        ssu->set_function(this);
        
        for (size_t i = 0; i < filec; ++i)
            ssu->set_function(filev[i]->first);
        
        ss::array<string> arr = marshall(argc, argv);
        
        for (size_t  i = 0; i < arr.size(); ++i)
            ssu->set_array("argv", i, arr[i]);
        
        ssu->consume("argv");
        ssu->evaluate("shrink argv");
        
        string result = target->execute(ssu);
        
        consume();
        
        ssu->restore(_buid);
        ssu->restore(buid);
        
        return result;
    }

    void file::close() {
        for (size_t i = 0; i < filec; ++i)
            if (filev[i]->second)
                filev[i]->first->close();
        
        delete[] filev;
        
        target->close();
        
        delete this;
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
        data.insert(2, encode(filepath));
        
        for (size_t k = 1; k < j; ++k)
            data.insert(k + 2, EMPTY);
        
        return data;
    }
}
