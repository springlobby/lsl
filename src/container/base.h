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
    typedef typename ItemType::IndexType
        IndexType;
    typedef boost::shared_ptr< ItemType >
        PointerType;

    //! putting this here makes it inherently distinguishable on a per *List basis
    struct MissingItemException : public std::out_of_range {
        MissingItemException( const IndexType& index );
    };

protected:
    typedef std::map< const IndexType, PointerType >
        MapType;

public:
    ContainerBase();

    void Add( PointerType item );
    void Remove( const IndexType& index );
    //! throws MissingItemException if no item at \param index
    const PointerType Get( const IndexType& index ) const;
    PointerType Get( const IndexType& index );
    bool Exists( const IndexType& index ) const;

    typename MapType::size_type size() const;

private:
    MapType map_;
};

} //namespace LSL

#include "base.cc"

#endif // #ifndef LIBSPRINGLOBBY_HEADERGUARD_CONTAINERBASE_H
