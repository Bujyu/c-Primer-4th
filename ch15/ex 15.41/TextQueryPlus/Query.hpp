#include "TextQuery.hpp"

#include <set>
#include <iostream>

#ifndef _QUERY_H_
#define _QUERY_H_

class BaseQuery // Interface
{
    friend class Query;
protected:
    typedef TextQuery::line_no line_no;
    virtual ~BaseQuery() { };
private:
    virtual std::set<line_no> eval( const TextQuery& ) const = 0;
    virtual std::ostream& display( std::ostream& = std::cout ) const = 0;
};

class Query     // Handle of BaseQuery and its derived class
{
    friend Query operator~( const Query& );
    friend Query operator|( const Query&, const Query& );
    friend Query operator&( const Query&, const Query& );
    friend std::ostream& operator<<( std::ostream &os, const Query &q );

public:
    Query( const std::string& );
    Query( const Query &c ) : q( c.q ), use( c.use ) {
        ++*use;
    }
    ~Query(){ decr_use(); }

    Query& operator=( const Query& );

    std::set<TextQuery::line_no> eval( const TextQuery &t ) const
    {
        return q->eval(t);
    }

    std::ostream& display( std::ostream &os ) const
    {
        return q->display(os);
    }

private:

    Query( BaseQuery *query ) : q( query ), use( new std::size_t(1) ) { }

    BaseQuery *q;
    size_t *use;
    void decr_use(){ if( --*use == 0 ) { delete q; delete use; } }

};

class WordQuery : public BaseQuery  // Word Query
{

    friend class Query;

    WordQuery( const std::string &oper ) : word(oper) { }

    std::set<line_no> eval( const TextQuery &t ) const
    {
        return t.run_query( word );
    }

    std::ostream& display( std::ostream &os ) const
    {
        return os << word;
    };

    const std::string word;

};

class NotQuery : public BaseQuery   // Not Query
{
protected:
    friend Query operator~(const Query&);

    NotQuery( Query q ) : query(q) { }

    // Virtual function from base
    std::set<line_no> eval(const TextQuery&) const;
    std::ostream& display(std::ostream &os) const
    {
        return os << "~(" << query << ")";
    }

    const Query query;
};

class BinaryQuery : public BaseQuery    // Binary Query
{
protected:

    BinaryQuery( const Query &lop, const Query &rop, const std::string &op ) :
        lhs(lop), rhs(rop), oper(op) { }

    std::ostream& display( std::ostream &os ) const
    {
        return os << "( " << lhs << " " << oper << " " << rhs << " )";
    }

    const Query lhs, rhs;
    const std::string oper;

};

class OrQuery : public BinaryQuery      // Or Query from binary query
{
protected:
    friend Query operator|( const Query&, const Query& );
    OrQuery( const Query &lhs, const Query &rhs ) : BinaryQuery( lhs, rhs, "|" ) { }
    std::set<line_no> eval( const TextQuery &tq ) const;
};

class AndQuery : public BinaryQuery      // Or Query from binary query
{
protected:
    friend Query operator&( const Query&, const Query& );
    AndQuery( const Query &lhs, const Query &rhs ) : BinaryQuery( lhs, rhs, "&" ) { }
    std::set<line_no> eval( const TextQuery &tq ) const;
};

inline std::ostream& operator<<( std::ostream &os, const Query &q )
{
    return q.display( os );
};

inline Query operator~( const Query& oper ){
    return new NotQuery( oper );
}

inline Query operator|( const Query &lhs, const Query &rhs )
{
    return new OrQuery( lhs, rhs );
}

inline Query operator&( const Query &lhs, const Query &rhs )
{
    return new AndQuery( lhs, rhs );
}

#endif // _QUERY_H_
