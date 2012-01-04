#ifndef LIBSPRINGLOBBY_HEADERGUARD_CONTAINERBASE_H
#define LIBSPRINGLOBBY_HEADERGUARD_CONTAINERBASE_H


#include <boost/smart_ptr.hpp>
#include <map>
#include <stdexcept>

namespace LSL {

//! common base class for *List classes
template < class ItemImp >
class ContainerBase {
public:
    typedef ItemImp
        ItemType;
	typedef typename ItemType::KeyType
		KeyType;
    typedef boost::shared_ptr< ItemType >
        PointerType;
	typedef boost::shared_ptr< const ItemType >
		ConstPointerType;

protected:
	typedef std::map< const KeyType, PointerType >
		MapType;

public:
    //! putting this here makes it inherently distinguishable on a per *List basis
	struct MissingItemException : public std::runtime_error {
		MissingItemException( const KeyType& key );
		MissingItemException( const typename MapType::size_type& idx );
    };

public:
    ContainerBase();

    void Add( PointerType item );
	void Remove( const KeyType& key );
	//! throws MissingItemException if no item at \param key
	const PointerType Get( const KeyType& key ) const;
	PointerType Get( const KeyType& key );
	bool Exists( const KeyType& key ) const;

    typename MapType::size_type size() const;

private:
	typename MapType::const_iterator begin() const { return m_map.begin(); }
	typename MapType::iterator begin() { return m_map.begin(); }
	typename MapType::const_iterator end() const { return m_map.end(); }
	typename MapType::iterator end() { return m_map.end(); }

public:
	const ConstPointerType At( const typename MapType::size_type index ) const;
	const ConstPointerType operator[]( typename MapType::size_type index ) const { return At(index); }

private:
	MapType m_map;
	// The following are used as internal cache to speed up random access:
	mutable typename MapType::const_iterator m_seek;
	mutable typename MapType::size_type m_seekpos;
	static const typename MapType::size_type SEEKPOS_INVALID = typename MapType::size_type(-1);
};

} //namespace LSL

#include "base.cc"

#endif // #ifndef LIBSPRINGLOBBY_HEADERGUARD_CONTAINERBASE_H
