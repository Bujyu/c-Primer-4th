#include <algorithm>

#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>

#ifndef _TEXT_QUERY_H
#define _TEXT_QUERY_H

class TextQuery{

public:
    typedef std::vector<std::string>::size_type line_no;

    void read_file( std::ifstream &is ){
        store_file( is ); build_map();
    }
    std::set<line_no> run_query( const std::string &str ) const;
    std::string text_line( line_no lineNo ) const;

    static bool comp( char x ){ return std::ispunct(x); }

private:

    void store_file( std::ifstream& is );
    void build_map();

    std::vector<std::string> lines_of_text;
    std::map< std::string, std::set<line_no> > word_map;

};

#endif // _TEXT_QUERY_H
