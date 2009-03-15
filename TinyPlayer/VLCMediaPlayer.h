#ifndef VLCMEDIAPLAYER_H
#define VLCMEDIAPLAYER_H

#include "vlc/vlc.h"

#include "VLCMedia.h"
#include "VLCException.h"

namespace   LibVLCpp
{
    class   MediaPlayer
    {
    public:
        typedef libvlc_media_player_t*      internalPtr;

        MediaPlayer(Media* media);
        internalPtr                         getInternalPtr();
        void                                play();
        void                                pause();
        qint64                              getTime();
        void                                setTime(qint64 time);
        qint64                              getLength();
    private:
        internalPtr                 _mediaPlayer;
        Exception                   _ex;
    };
}

#endif // VLCMEDIAPLAYER_H
