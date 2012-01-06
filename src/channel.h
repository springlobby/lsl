#ifndef CHANNEL_H
#define CHANNEL_H

#include <utils/global_interfaces.h>

namespace LSL {

class Channel : public HasKey< std::string >
{
public:
    Channel();

    std::string index() const { return "dummy"; }
    static std::string className() { return "Channel"; }

	std::string Name() const { return "dummy"; }
};

} // namespace LSL {

#endif // CHANNEL_H
