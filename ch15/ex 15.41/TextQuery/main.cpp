#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include"TextQuery.hpp"

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

    while( true ){
        std::cout << "Enter word to look for, or q to quit: ";
        std::string s;
        std::cin >> s;
        if( !std::cin || s == "q" ) break;
        std::set<TextQuery::line_no> locs = tq.run_query( s );  // Get the set of line no
        print_results( locs, s, tq );
    }

    return 0;

}
