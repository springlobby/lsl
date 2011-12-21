#ifndef LIBSPRINGLOBBY_HEADERGUARD_CONTAINERBASE_H
#define LIBSPRINGLOBBY_HEADERGUARD_CONTAINERBASE_H


#include <boost/smart_ptr.hpp>
#include <map>

namespace LSL {

template < class ItemImp , class IndexImp >
class ContainerBase {
public:
    typedef ItemImp
        ItemType;
    typedef IndexImp
        IndexType;
    typedef boost::shared_ptr< ItemType >
        PointerType;
protected:
    typedef std::map< const IndexType, PointerType >
        MapType;
public:
    ContainerBase();

    void Add( PointerType item );
    void Remove( const IndexType& index );
    PointerType Get( const IndexType& index ) const;
    bool Exists( const IndexType& index ) const;

    typename MapType::size_type size() const;

private:
    MapType map_;
};

} //namespace LSL

#include "base.cc"

#endif // #ifndef LIBSPRINGLOBBY_HEADERGUARD_CONTAINERBASE_H
