#ifndef TASSERVER_H
#define TASSERVER_H

namespace LSL {

class CommandDictionary;

class TASServer {
public:
    TASServer(int TASServerMode);
private:
    //! hide actual imp behind an opaque pointer
    CommandDictionary* m_cmd_dict;
};

} //namespace LSL

#endif // TASSERVER_H
