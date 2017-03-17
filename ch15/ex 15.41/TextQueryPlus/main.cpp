#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include"TextQuery.hpp"
#include "Query.hpp"

std::ifstream& open_file( std::ifstream& fs, std::string fileName ){
    fs.close(); // Ensure the previous stream is closed
    fs.clear(); // Clear previous state
    fs.open( fileName.c_str() );
    return fs;
}

std::string make_plural( std::size_t ctr, const std::string& str, const std::string& ending ){
    return ( ctr == 1 ? str : str + ending );
}

void print_results( const std::set<TextQuery::line_no>& locs,
                    const std::string& sought, const TextQuery& file ){

    typedef std::set<TextQuery::line_no> lineNumbers;

    lineNumbers::size_type size = locs.size();
    std::cout << "\n" << sought << " occurs " << size << " " << make_plural( size, "time", "s" ) << std::endl;

    lineNumbers::const_iterator it = locs.begin();
    while( it != locs.end() ){
        std::cout << "\t(line " << (*it)+1 << ") " << file.text_line(*it) << std::endl;
        ++it;
    }
}

int main( int argc, char** argv ){

    std::ifstream infile;
    if( argc < 2 || !open_file( infile, argv[1] ) ){
        std::cerr << "File open error!" << std::endl;
        return EXIT_FAILURE;
    }

    TextQuery tq;
    tq.read_file( infile ); // Generate the word map

    Query q = Query("fiery") & Query("bird") | Query("wind");
    std::ostringstream oss;
    oss << q;
    print_results( q.eval(tq), oss.str() , tq );

    return 0;

}
