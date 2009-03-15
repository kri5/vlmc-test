#ifndef VLCMEDIA_H
#define VLCMEDIA_H

#include "vlc/vlc.h"

#include <QString>

#include "VLCpp.hpp"
#include "VLCException.h"
#include "VLCInstance.h"

namespace LibVLCpp
{
    class   Media : public Internal<libvlc_media_t>
    {
    public:
        Media(Instance& instance, const QString& filename);
        ~Media();
        void            addOption(const char* opt);
    private:
        Exception       _ex;
        Instance&       _instance;
    };
}

#endif // VLCMEDIA_H
