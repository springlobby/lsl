#ifndef LSL_CHANNEL_H
#define LSL_CHANNEL_H

/** \file channel.h
    \copyright GPL v2 **/

#include <lslutils/global_interfaces.h>
#include <lslutils/type_forwards.h>

namespace LSL {

//! minimal channel model
class Channel : public HasKey< std::string >
{
public:
    Channel();
    Channel(const std::string& name);

    //! the HasKey mandated identifier, must be unique
    std::string key() const { return Name(); }
    static std::string className() { return "Channel"; }

	std::string Name() const { return "dummy"; }

    void OnChannelJoin( const ConstUserPtr user );

    void SetNumUsers( size_t numusers );
    void SetTopic( const std::string topic);

};

} // namespace LSL {

#endif // LSL_CHANNEL_H
