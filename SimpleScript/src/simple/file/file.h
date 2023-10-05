//
//  file.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/27/23.
//

#ifndef file_h
#define file_h

#include "file_statement.h"
#include "node.h"

using namespace chrono;
using namespace ss;

namespace ss {
    class file: public function_t {
        //  MEMBER FIELDS
        
        size_t filec = 0;
        pair<file*, bool>** filev = new pair<file*, bool>*[1];
        
        string filepath;
        
        interpreter* ssu = NULL;
        
        file_statement* target = NULL;
        
        //  MEMBER FUNCTIONS
        
        ss::array<string> marshall(const size_t argc, string* argv) const;
    public:
        //  CONSTRUCTORS
        
        file(const string filepath, interpreter* ssu, node<string>* parent);
        
        void close();
        
        //  MEMBER FUNCTIONS
        
        string call(const size_t argc, string* argv);
    };
}

#endif /* file_h */
