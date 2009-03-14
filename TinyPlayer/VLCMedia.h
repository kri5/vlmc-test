#ifndef VLCMEDIA_H
#define VLCMEDIA_H

#include "vlc/vlc.h"

#include <QString>
#include "VLCException.h"
#include "VLCInstance.h"

namespace LibVLCpp
{
    class   Media
    {
    public:
        typedef libvlc_media_t*     internalPtr;

        Media(Instance* instance, const QString& filename);
        ~Media();
        void            addOption(const char* opt);
        internalPtr     getInternalPtr();
    private:
        internalPtr     _media;
        Exception*      _ex;
        Instance*       _instance;
    };
}

#endif // VLCMEDIA_H
