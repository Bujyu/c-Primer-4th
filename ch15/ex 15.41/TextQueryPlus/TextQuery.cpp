#include "TextQuery.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>
#include <cctype>

std::set<TextQuery::line_no> TextQuery::run_query( const std::string &str ) const
{

    std::map< std::string, std::set<line_no> >::const_iterator it = word_map.find( str );
    if( it == word_map.end() )
        return std::set<line_no>(); // Not found, return empty set
    else
        return it->second;
}

std::string TextQuery::text_line( TextQuery::line_no lineNo ) const
{
    if( lineNo < lines_of_text.size() )
        return lines_of_text[lineNo];
    throw std::out_of_range( "line number out of range" );
}

void TextQuery::store_file( std::ifstream& is )
{
    std::string str;

    while( std::getline( is, str ) )
        lines_of_text.push_back( str );

}

void TextQuery::build_map(){

    for( line_no no = 0 ; no != lines_of_text.size() ; ++no ){
        std::istringstream iss( lines_of_text[no] );
        std::string word;
        while( iss >> word ){
            word.erase(std::remove_if( word.begin(), word.end(), comp ), word.end());  // Remove punctuation character
            word_map[word].insert(no);    // Find word in map and add the line number to set
        }
    }

}
