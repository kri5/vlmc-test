#ifndef VLCINSTANCE_H
#define VLCINSTANCE_H

#include "vlc/vlc.h"
#include "VLCException.h"

namespace LibVLCpp
{
    class   Instance
    {
    public:
        typedef libvlc_instance_t*      internalPtr;

        Instance(int argc, const char** argv);
        internalPtr     getInternalPtr();
    private:
        Exception       _ex;
        internalPtr     _instance;
    };
}

#endif // VLCINSTANCE_H
