#ifndef TDFCONTAINER_H
#define TDFCONTAINER_H

#include <utils/misc.h>
#include <utils/conversion.h>
#include <utils/type_forwards.h>

namespace LSL {

/// Todo: add TDFContainer class.
///

class TDFWriter
{
	public:
		TDFWriter( std::string &s );
		~TDFWriter();
		void EnterSection( const std::string &name );
		void LeaveSection();
		void Indent();
		std::string GetCurrentPath();
		void Append( const std::string &name, std::string value );
		template<class T>
		void Append( const std::string &name, T value );

		/// works like algorithms such as std::sort
		template<class T> void Append( const std::string &name, T begin, T end );

		void AppendLineBreak();
		void Close();
	protected:
	private:
		std::string &m_stream;
		int m_depth;
};

///
/// Parsing classes contributed by dmytry aka dizekat, copypasted from personal
/// project: render_control_new/utils/parsing.h and cpp , database.h and cpp
///

#include <sstream>
#include <vector>
#include <deque>
#include <map>

#include <utils/autopointers.h>

class Tokenizer;
class Node;
typedef RefcountedPointer<Node> PNode;
class DataList;
typedef RefcountedPointer<DataList> PDataList;
class DataLeaf;
typedef RefcountedPointer<DataLeaf> PDataLeaf;

class Node: public RefcountedContainer , public boost::noncopyable
{
		friend class DataList;
	protected:
		DataList *parent;
		Node *list_prev;
		Node *list_next;
		void ListRemove();
		void ListInsertAfter( Node *what );
		std::string name;
	public:

		std::string Name();

		// Sets the name, and updates parent if present
		bool SetName( const std::string &name_ );
		Node(): parent( NULL ), list_prev( NULL ), list_next( NULL ), name( std::string() ) {}
		virtual ~Node();
		DataList* Parent() const;// parent list
		//void SetParent(DataList *parent_);
		bool IsChildOf( DataList *what ) const;

		virtual void Save( TDFWriter &f );
		virtual void Load( Tokenizer &f );
};


/// Usage
/// Parsing:

/// int errs=0;
/// PDataList root(ParseTDF(some istream, &errs));// errs is optional, gives number of errors when loading.

/// Getting values:

/// PDataList game(root->Find(_T("GAME")))
/// if(!game.ok()){wxLogMessage(_T("Game tag is missing"));return false;}
/// std::string gamename=game->GetString(_T("Mapname"));

/// (see optional parameters for setting default and knowing if it failed)
/// and so on and so forth.
/// Saving tdf:
///


class DataList: public Node {
	protected:
		std::map<std::string, PNode> nodes;
		Node list_loop;// next is first, prev is last in the list
		typedef std::map<std::string, PNode>::iterator nodes_iterator;
	public:
		DataList();
		~DataList();

		bool Insert( PNode node );// return false if such entry already exists.
		bool InsertAt( PNode node, PNode where );// return false if such entry already exists. Inserts right before given

		// rename if such entry already exists. Names is like name!1 or name!2 etc
		void InsertRename( PNode node );
		void InsertRenameAt( PNode node, PNode where );
		bool Remove( const std::string &str );
		bool Remove( PNode node );
		bool Rename( const std::string &old_name, const std::string &new_name );
		PNode Find( const std::string &str );// find by name

		std::string Path();

		PNode FindByPath( const std::string &str );

		PNode Next( PNode what );
		PNode Prev( PNode what );
		PNode End();
		PNode First();
		PNode Last();

		virtual void Save( TDFWriter &f );
		virtual void Load( Tokenizer &f );

		int GetInt( const std::string &name, int default_value = 0, bool *it_worked = NULL );
		double GetDouble( const std::string &name, double default_value = 0, bool *it_worked = NULL );
		std::string GetString( const std::string &name, const std::string &default_value = std::string(), bool *it_worked = NULL );
		/// returns number of values that were successfully read
		int GetDoubleArray( const std::string &name, int n_values, double *values );

		lslColor GetColour( const std::string &name, const lslColor &default_value = lslColor( 0, 0, 0 ), bool *it_worked = NULL );

};

class DataLeaf: public Node {
		friend class DataList;
	protected:
		std::string value;
	public:
		std::string GetValue();
		void SetValue( const std::string &value );

		virtual void Save( TDFWriter &f );
		virtual void Load( Tokenizer &f );
};
} // end namespace SL

inline bool IsLetter( char c ) {
	return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c == '_' );
}
inline bool IsNum( char c ) {
	return c >= '0' && c <= '9';
}
inline bool IsNumDot( char c ) {
	return ( c >= '0' && c <= '9' ) || c == '.';
}
inline bool IsAlphaNum( char c ) {
	return IsLetter( c ) || IsNum( c );
}
inline bool IsWhitespace( char c ) {
	return ( c == ' ' ) || ( c == 10 ) || ( c == 13 ) || ( c == '\t' );
}
struct Token {
	enum TokenType {
		type_none,
		type_section_name,
		type_enter_section,
		type_leave_section,
		type_entry_name,
		type_entry_value,
		type_semicolon,
		type_eof
	};
	TokenType type;
	std::string value_s;

	std::string pos_string;// for error reporting

	bool IsEOF() const {
		return ( type == type_eof );
	}
	Token(): type( type_eof )
	{
	}

};

class Tokenizer {
		/// todo: clean up, move to CPP file

		/// simple reference counted pointer to stream.
		struct IncludeCacheEntry {
			std::string name; ///< used for error reporting
			int line;
			int column;
			std::istream *stream;
			//bool must_delete;
			int *refcount;

			IncludeCacheEntry( std::istream *stream_, bool must_delete_ = false ):
					line( 1 ),
					column( 1 ),
					stream( stream_ ),
					refcount( NULL )
			{
				if ( must_delete_ ) {
					refcount = new int;
					( *refcount ) = 1;
				}
			}
			IncludeCacheEntry( const IncludeCacheEntry &other ):
					line( other.line ),
					column( other.column ),
					stream( other.stream ),
					refcount( other.refcount )
			{
				stream = other.stream;
				refcount = other.refcount;
				if ( refcount )( *refcount ) += 1;
			}
			~IncludeCacheEntry()
			{
				if ( refcount ) {
					( *refcount )--;
					if ( ( *refcount ) <= 0 ) {
						delete stream;
						delete refcount;
					}
				}
			}
		};
		std::vector<IncludeCacheEntry> include_stack;
		void UnwindStack();

		std::deque<Token> token_buffer;
		//size_t max_buffer_size;
		size_t buffer_pos;

		bool skip_eol;
		char GetNextChar();
		char PeekNextChar();

		void ReadToken( Token &token );
		void SkipSpaces();

		int errors;

	public:
		Tokenizer(): buffer_pos( 0 ), skip_eol( false ), errors( 0 )
		{
		}

		void EnterStream( std::istream& stream_, const std::string& name = "" );

		Token GetToken( int i = 0 );
		void Step( int i = 1 );
		inline Token TakeToken() {
			Token result = GetToken();
			Step();
			return result;
		}

		bool Good();

		void ReportError( const Token& t, const std::string& err );

		int NumErrors() const {
			return errors;
		}
};

inline Tokenizer &operator >>( Tokenizer& tokenizer, Token& token ) {
	token = tokenizer.TakeToken();
	return tokenizer;
}

PDataList ParseTDF( std::istream &s, int *error_count = NULL );

//Defintions to not clutter up the class declaration
template<class T> void TDFWriter:: Append( const std::string &name, T value )
{
	Append( name, Util::ToString( value ) );
}

template<class T>
void TDFWriter::Append( const std::string &name, T begin, T end ) {
	Indent();
	m_stream << name << _T( "=" );
	for ( T it = begin;it != end;++it ) {
		if ( it != begin )m_stream << _T( " " );
		m_stream << ( *it );
	}
	m_stream << _T( ";\n" );
}

} // namespace LSL {

#endif // TDFCONTAINER_H

/**
    This file is part of SpringLobby,
    Copyright (C) 2007-2011

    SpringLobby is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as published by
    the Free Software Foundation.

    SpringLobby is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SpringLobby.  If not, see <http://www.gnu.org/licenses/>.
**/

