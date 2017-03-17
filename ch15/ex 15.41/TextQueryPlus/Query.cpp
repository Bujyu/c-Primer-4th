#include "Query.hpp"


Query::Query( const std::string &oper ) : q( new WordQuery( oper ) ), use( new size_t(1) ) {}

Query& Query::operator=( const Query& rhs ){
    ++*rhs.use;   // Avoid self assignment
    decr_use();
    q = rhs.q;
    use = rhs.use;
    return *this;
}

