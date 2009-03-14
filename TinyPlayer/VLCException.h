#ifndef VLCEXCEPTION_H
#define VLCEXCEPTION_H

#include "vlc/vlc.h"

namespace LibVLCpp
{
    class   Exception
    {
    public:
        //Todo, tenter uner surcharge d'operateur de cast generique a partir du interalPtr
        typedef libvlc_exception_t*      internalPtr;

        Exception();
        ~Exception();
        internalPtr         getInternalPtr();
        const char*         getErrorText() const;
        void                clear();
        int                 raised() const;

        static const int    isRaised = 1;
        static const int    notRaised = 0;
    private:
        internalPtr         _ex;
    };
}

#endif // VLCEXCEPTION_H
