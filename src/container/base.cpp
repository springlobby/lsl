#include "base.h"

namespace LSL {

template < class T , class I >
ContainerBase<T,I>::ContainerBase()
{
}

template < class T , class I >
void ContainerBase<T,I>::Add( PointerType item )
{

}

template < class T , class I >
void ContainerBase<T,I>::Remove( const IndexType& index )
{
    typename ContainerBase<T,I>::MapType::iterator
        it = map_.find( index );
    const bool found = it != map_.end();
    if ( found )
        map_.erase( it );
}

template < class T , class I >
typename ContainerBase<T,I>::PointerType ContainerBase<T,I>::Get( const IndexType& index ) const
{
    typename ContainerBase<T,I>::MapType::iterator
        it = map_.find( index );

}

template < class T , class I >
bool ContainerBase<T,I>::Exists( const IndexType& index ) const
{
    return map_.find( index ) != map_.end();
}

template < class T , class I >
typename ContainerBase<T,I>::MapType::size_type ContainerBase<T,I>::size() const
{

}

}
