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

    //! putting this here makes it inherently distinguishable on a per *List basis
    struct MissingItemException : public std::out_of_range {
		MissingItemException( const KeyType& index );
    };

protected:
	typedef std::map< const KeyType, PointerType >
        MapType;

public:
    ContainerBase();

    void Add( PointerType item );
	void Remove( const KeyType& index );
    //! throws MissingItemException if no item at \param index
	const PointerType Get( const KeyType& index ) const;
	PointerType Get( const KeyType& index );
	bool Exists( const KeyType& index ) const;

    typename MapType::size_type size() const;

	typename MapType::const_iterator begin() const { return map_.begin(); }
	typename MapType::iterator begin() { return map_.begin(); }
	typename MapType::const_iterator end() const { return map_.end(); }
	typename MapType::iterator end() { return map_.end(); }

private:
    MapType map_;
};

} //namespace LSL

#include "base.cc"

#endif // #ifndef LIBSPRINGLOBBY_HEADERGUARD_CONTAINERBASE_H
