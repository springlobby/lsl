#include <boost/format.hpp>

namespace LSL {

template < class T >
ContainerBase<T>::ContainerBase()
{
}

template < class T >
void ContainerBase<T>::Add( PointerType item )
{
    m_map[item->index()] = item;
}

template < class T >
void ContainerBase<T>::Remove( const KeyType& index )
{
    typename ContainerBase<T>::MapType::iterator
        it = m_map.find( index );
    const bool found = it != m_map.end();
    if ( found )
        m_map.erase( it );
}

template < class T >
const typename ContainerBase<T>::PointerType ContainerBase<T>::Get( const KeyType& index ) const
{
    typename ContainerBase<T>::MapType::const_iterator
        it = m_map.find( index );
    if ( it == m_map.end() )
        throw MissingItemException( index );
    return it->second;
}
template < class T >
typename ContainerBase<T>::PointerType ContainerBase<T>::Get( const KeyType& index )
{
    typename ContainerBase<T>::MapType::iterator
        it = m_map.find( index );
    if ( it == m_map.end() )
        throw MissingItemException( index );
    return it->second;
}

template < class T >
bool ContainerBase<T>::Exists( const KeyType& index ) const
{
    return m_map.find( index ) != m_map.end();
}

template < class T >
typename ContainerBase<T>::MapType::size_type ContainerBase<T>::size() const
{
    return m_map.size();
}

template < class T >
ContainerBase<T>::MissingItemException::MissingItemException( const typename ContainerBase<T>::KeyType& key )
    : std::runtime_error( (boost::format( "No %s found in list for item with key %s" ) % T::className() % key).str() )
{}

template < class T >
ContainerBase<T>::MissingItemException::MissingItemException( const typename ContainerBase<T>::MapType::size_type& index )
    : std::runtime_error( (boost::format( "No %s found in list for item with pseudo index %s" ) % T::className() % index).str() )
{}

template < class T >
const typename ContainerBase<T>::ConstPointerType
ContainerBase<T>::At( const typename ContainerBase<T>::MapType::size_type index) const
{
    if ((m_seekpos == SEEKPOS_INVALID) || (m_seekpos > index)) {
        m_seek = m_map.begin();
        m_seekpos = 0;
    }
    std::advance( m_seek, index - m_seekpos );
    m_seekpos = index;
    return *m_seek->second;
}

}
