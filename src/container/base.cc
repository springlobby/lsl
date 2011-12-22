#include <boost/format.hpp>

namespace LSL {

template < class T >
ContainerBase<T>::ContainerBase()
{
}

template < class T >
void ContainerBase<T>::Add( PointerType item )
{
    map_[item->index()] = item;
}

template < class T >
void ContainerBase<T>::Remove( const IndexType& index )
{
    typename ContainerBase<T>::MapType::iterator
        it = map_.find( index );
    const bool found = it != map_.end();
    if ( found )
        map_.erase( it );
}

template < class T >
const typename ContainerBase<T>::PointerType ContainerBase<T>::Get( const IndexType& index ) const
{
    typename ContainerBase<T>::MapType::const_iterator
        it = map_.find( index );
    if ( it == map_.end() )
        throw MissingItemException( index );
    return it->second;
}
template < class T >
typename ContainerBase<T>::PointerType ContainerBase<T>::Get( const IndexType& index )
{
    typename ContainerBase<T>::MapType::iterator
        it = map_.find( index );
    if ( it == map_.end() )
        throw MissingItemException( index );
    return it->second;
}

template < class T >
bool ContainerBase<T>::Exists( const IndexType& index ) const
{
    return map_.find( index ) != map_.end();
}

template < class T >
typename ContainerBase<T>::MapType::size_type ContainerBase<T>::size() const
{
    return map_.size();
}

template < class T >
ContainerBase<T>::MissingItemException::MissingItemException( const typename ContainerBase<T>::IndexType& index )
    :std::out_of_range( (boost::format( "No %s found in list for item %s" ) % T::className() % index).str() )
{}

}
