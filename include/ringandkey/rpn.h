/*
MIT License

Copyright (c) 2019 ringandkey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef RINGANDKEY_REVERSE_POLISH_NOTATION_H_
#define RINGANDKEY_REVERSE_POLISH_NOTATION_H_

#include <string>
#include <vector>

namespace ringandkey {
    class rpn {
        private:
            // Prioritize operators 
            int priority( char token ) {
                if( token == '*' || token == '/' || token == '%' ) { return 100; }
                else if( token == '+' || token == '-' ) { return 10; }
                return 1;
            }

            // Determine whether it is a token you want to ignore
            bool is_ignore(char token) {
                return token == ' ' || token== '\t' || token=='\n' || token=='\r';
            }

            // Determine whether the given token is operator
            bool is_operator(std::string token) {
                return token == "+" || token=="-" || token=="*" || token=="/" || token=="%";
            }

            // Determine whether it is a numeric value
            bool is_numeric(const std::string& token) {
                try {
                    int n = std::stoi(token);
                    return true;
                }
                catch(...) {
                    return false;
                }
            }

            // Pop data from the stack
            // (The stack is defined as a vector type)
            template<typename T> T pop_as(std::vector<T>& src) {
                T v = src.back();
                src.pop_back();
                return v;
            }

            std::vector<std::string> buffer;
        public:
            rpn() {}
            rpn(const std::string& src) {
                parse(src);
            }

            //
            // Convert the given formula to reverse Polish notation
            // The result is stored in the buffer as an array for each element
            //
            void parse( const std::string& src ) {
                std::vector<std::string> stack;
                std::string digit;

                const int num = src.size();
                for(int i=0; i<num; i++ ) {
                    // Retrieve one token from the input mathematical expression
                    const char token = src[i];

                    // Ignore whitespace and line breaks
                    if( is_ignore(token) ) { continue; }

                    if( std::isdigit( token ) ) {
                        digit += token;
                        continue;
                    }
                    else {
                        // If it is not a digit, digit string is added to the buffer and cleared
                        if( !digit.empty() ) {
                            buffer.push_back(digit);
                            digit.clear();
                        }
                    }

                    if( token == ')' ) {
                        // When the token is ')'
                        // if mathematical expressions where parentheses are correctly matched, then ( is already stacked on the stack
                        // save the contents up to that point in the buffer.
                        while( stack.back() != "(" ) {
                            buffer.push_back( pop_as<std::string>(stack) );
                        }

                        // throw away '('
                        if( stack.back() == "(") {
                            stack.pop_back();
                        }
                    }
                    else if( token == '(' ) {
                        // If the token is '(' unconditionally pile up on the stack
                        stack.push_back( std::string()+token);
                    }
                    else {
                        // Tokens that have come up to this point are operators such as +-*/
                        while(true) {
                            if( stack.empty() ) {
                                stack.push_back(std::string()+token);
                                break;
                            }

                            if( priority(token) <= priority( stack.back()[0] ) ){
                                // If the priority of the token is less than the priority of the operator at the top of the stack
                                // move the operator at the top of the stack to the buffer
                                buffer.push_back( pop_as<std::string>(stack) );
                            }
                            else {
                                stack.push_back(std::string()+token);
                                break;
                            }
                        }
                    }
                }

                // If there are numerical values, push them to the storage buffer
                if( !digit.empty() ) { buffer.push_back(digit); }

                // If there are tokens remaining in the operator stack, push them all to the storage buffer
                while( !stack.empty() ) {
                    buffer.push_back( pop_as<std::string>(stack) );
                }
            }

            // Evaluate mathematical expressions parsed in reverse Polish notation and return result
            int eval() {
                std::vector<int> stack;
                for(auto v: buffer) {
                    if( is_numeric(v) ) {
                        stack.push_back( std::stoi(v));
                    }
                    else if( is_operator(v) ) {
                        int a = pop_as<int>(stack);
                        int b = pop_as<int>(stack);

                        if(v=="+") { stack.push_back(b+a); }
                        else if(v=="-") { stack.push_back(b-a); }
                        else if(v=="*") { stack.push_back(b*a); }
                        else if(v=="/") {
                            if( a== 0) {
                                throw std::range_error("ERROR: division by zero.");
                            }
                            stack.push_back(b/a);
                        }
                        else if(v=="%") {
                            if( a==0) {
                                throw std::range_error("ERROR: calculated the remainder with zero");
                            }
                            stack.push_back(b%a);
                        }
                    }
                }
                return stack.back();
            }

            static int calc(const std::string& src) {
                rpn r(src);
                return r.eval();
            }
    };
}

#endif // REVERSE_POLISH_NOTATION_H_
