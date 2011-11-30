/* matrix-market-array.h
 * MatrixStreamReader specialization for matrices in the MatrixMarket coordinate
 * format.
 * Dan Roche, 1-25-05
 */

#ifndef __FORMAT_MATRIX_MARKET_H
#define __FORMAT_MATRIX_MARKET_H

#include <string>
#include <sstream>
#include <linbox/util/matrix-stream.h>

/*
namespace LinBox__FORMAT_MATRIX_MARKET_H
	{ static const char* name = "Matrix Market Format";
	  static const char* shortname = "mm"; }
	  */

namespace LinBox {

#ifndef __INTEGER_H
class integer;
#endif

static bool equalCaseInsensitive(const std::string s1, const char* s2) {
	int len = s1.size();
	int counter = 0;
	while( counter < len && s2[counter] != '\0' &&
	       toupper(s1[counter]) == toupper(s2[counter]) ) ++counter;
	return( counter == len && s2[counter] == '\0' );
}

template<class Field>
class MatrixMarketReader :public MatrixStreamReader<Field> {
    public:
    	typedef typename MatrixStreamReader<Field>::Element Element;
    private:
    	int entriesLeft;
	size_t currentCol, currentRow;
	bool array;
	bool pattern;
	bool symmetric;

	MatrixStreamError readHeader() {
	        //Skip comments
		this->ms->readWhiteSpace();
		while( !this->sin->eof() && this->sin->peek() == '%' ) {
			char c;
			while( this->sin->get(c) ) {
				if( c == '\n' || c == '\r' ) {
					this->sin->putback(c);
					break;
				}
			}
			this->ms->readWhiteSpace();
		}

		*(this->sin) >> this->_m;
		this->ms->readWhiteSpace();
		*(this->sin) >> this->_n;
		this->ms->readWhiteSpace();
		if( !array ) {
			*(this->sin) >> entriesLeft;
			this->ms->readWhiteSpace();
		}

	        if( this->sin->eof() ) return END_OF_FILE;
	        if( !this->sin->good() ) return BAD_FORMAT;

		this->knowM = this->knowN = true;
		currentCol = currentRow = 1;

		if( symmetric && (this->_m != this->_n) ) return BAD_FORMAT;
		if( this->_m < 1 || this->_n < 1 ) return BAD_FORMAT;
		if( !array && (entriesLeft < 0 || 
		    (size_t)entriesLeft > this->_m*this->_n ) )
		    return BAD_FORMAT;

		return GOOD;
	}

    protected:
    	MatrixStreamError nextTripleImpl( size_t& i, size_t& j, Element& v ) {
		if( currentCol == 0 && currentRow == 0 ) {
			MatrixStreamError mse = readHeader();
			if( mse != GOOD ) return mse;
		}

		if( array ) {
			if( currentCol == this->_n+1 ) return END_OF_MATRIX;
			j = currentCol;
			i = currentRow;
			if( ++currentRow == this->_m+1 ) {
				++currentCol;
				currentRow = (symmetric ? currentCol : 1);
			}
		}
		else {
			if( --entriesLeft < 0 ) return END_OF_MATRIX;

			this->ms->readWhiteSpace();
			*(this->sin) >> i;
			if( this->sin->eof() ) return END_OF_FILE;
			if( !this->sin->good() ) return BAD_FORMAT;

			this->ms->readWhiteSpace();
			*(this->sin) >> j;
			if( this->sin->eof() ) return END_OF_FILE;
			if( !this->sin->good() ) return BAD_FORMAT;
		}

		if( pattern ) this->ms->getField().init(v,(integer)1);
		else {
			this->ms->readWhiteSpace();
			this->ms->getField().read(*(this->sin),v);
			if( this->sin->eof() ) return END_OF_FILE;
			if( !this->sin->good() ) return BAD_FORMAT;
		}

		--i;
		--j;
		if( i < 0 || i >= this->_m || j < 0 || j >= this->_n )
			return BAD_FORMAT;
		if( symmetric && (i != j) ) saveTriple(j,i,v);

		return GOOD;
	}

	MatrixStreamError initImpl( const char* firstLine ) {
		std::string st(firstLine);
		std::stringstream stin(st);

	   	if( stin.get() != '%' || stin.get() != '%' )
	    		return NO_FORMAT;
	        if( !stin.good() ) return NO_FORMAT;

		std::string s;
		stin >> s;
	        if( !stin.good() ) return NO_FORMAT;
		if( !equalCaseInsensitive(s,"MatrixMarket") ) return NO_FORMAT;
	    
		stin >> s;
	        if( !stin.good() ) return BAD_FORMAT;
		if( !equalCaseInsensitive(s,"matrix") ) return BAD_FORMAT;

		stin >> s;
	        if( !stin.good() ) return BAD_FORMAT;
		if( equalCaseInsensitive(s,"array") ) array = true;
		else if( equalCaseInsensitive(s,"coordinate") ) array = false;
		else return BAD_FORMAT;

		stin >> s;
	        if( !stin.good() ) return BAD_FORMAT;
		pattern = equalCaseInsensitive(s,"pattern");

		stin >> s;
	        if( !stin.eof() && !stin.good() ) return BAD_FORMAT;
		if( equalCaseInsensitive(s,"symmetric") ) symmetric = true;
		else if( equalCaseInsensitive(s,"general") ) symmetric = false;
		else return BAD_FORMAT;

		stin >> s;
		if( !stin.eof() ) return BAD_FORMAT;

		if( array && pattern ) return BAD_FORMAT;

		currentRow = currentCol = 0;

		return GOOD;
	}

    public:
    	MatrixMarketReader() {
		entriesLeft = -1;
		currentCol = currentRow = 0;
	}

	bool isSparse() const { return !array; }
	
	const char* getName() const 
		{ return "Matrix Market Format"; }// LinBox__FORMAT_MATRIX_MARKET_H::name; }
	
	const char* shortName() const 
		{ return "mm"; }// LinBox__FORMAT_MATRIX_MARKET_H::shortname; }
};

}

#endif