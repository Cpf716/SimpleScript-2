//
//  main.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 4/18/23.
//

#include "file.h"
#include <sstream>

#define STOPWATCH false

using namespace simple;

int main(int argc, char* argv[]) {
    if (STOPWATCH)
        cout << "Building...\n";
    
    interpreter ss;
    
    string path = argc == 1 ? BASE_DIR + "main.txt" : argv[1];
    
    time_point<steady_clock> beg;
    
    if (STOPWATCH)
        beg = steady_clock::now();
    
    node<string>* root = new node<string>(EMPTY, NULL);
    
    class file* file = new class file(path, &ss, root);
     
    root -> close();
    
    time_point<steady_clock> end;
    
    if (STOPWATCH) {
        end = steady_clock::now();
        
        cout << "Done in " << duration<double>(end - beg).count() << "s.\n";
        cout << "Running...\n";
    }
    
    ss.set_function(file);
    
    ostringstream os;
    
    os << file -> name();
    
    if (argc > 2) {
        os << "(";
        
        for (size_t i = 2; i < argc - 1; i += 1)
            os << (is_double(argv[i]) ? rtrim(stod(argv[i])) : encode(argv[i])) << ",";
        
        os << (is_double(argv[argc - 1]) ? rtrim(stod(argv[argc - 1])) : encode(argv[argc - 1]));
        
        os << ")";
    }
    
    if (STOPWATCH)
        beg = steady_clock::now();
    
    try {
        statement(os.str()).execute(&ss);
        
    } catch (simple::error e) {
        ss.print_stack_trace();
        
        throw e;
    }
    
    if (STOPWATCH) {
        end = steady_clock::now();
        
        cout << "Done in " << duration<double>(end - beg).count() << "s.\n";
    }
}
