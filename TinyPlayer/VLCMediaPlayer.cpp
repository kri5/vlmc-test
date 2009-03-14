#include <cassert>
#include "VLCMediaPlayer.h"

using namespace LibVLCpp;

MediaPlayer::MediaPlayer(Media* media)
{
    this->_ex = new Exception();
    this->_mediaPlayer = libvlc_media_player_new_from_media(media->getInternalPtr(), this->_ex->getInternalPtr());
    assert(this->_ex->raised() == Exception::notRaised);
}

MediaPlayer::internalPtr        MediaPlayer::getInternalPtr()
{
    return this->_mediaPlayer;
}
