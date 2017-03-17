#include "Query.hpp"

std::set<AndQuery::line_no>
AndQuery::eval( const TextQuery &tq ) const
{

    std::set<line_no>
        ret_lines,
        lset = lhs.eval( tq ),
        rset = rhs.eval( tq );

    std::set_intersection
    (lset.begin(), lset.end(),
     rset.begin(), rset.end(),
     std::insert_iterator< std::set<line_no> >( ret_lines, ret_lines.begin() )
    );

    return ret_lines;

}


std::set<OrQuery::line_no>
OrQuery::eval( const TextQuery &tq ) const
{
    std::set<line_no>
        ret_lines = lhs.eval( tq ),
        rset = rhs.eval( tq );

    ret_lines.insert( rset.begin(), rset.end() );

    return ret_lines;

}

extern std::ostream& operator<<( std::ostream &os, const Query &q );
