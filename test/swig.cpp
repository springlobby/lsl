#include <boost/signals2.hpp>
#include <boost/function.hpp>

namespace LSL {
    boost::signals2::signal<void ()> battleSig;

    struct User
    {
        User()
        {
            battleSig.connect( *this );
        }
      void operator()() const
      {
        std::cout << "I was called" << std::endl;
      }
    };

    struct Battle {
        void update() {
            battleSig();
        }
    };
}

int main(int,char**)
{
    using namespace LSL;
    Battle battle;
    User user;
    battle.update();
}
