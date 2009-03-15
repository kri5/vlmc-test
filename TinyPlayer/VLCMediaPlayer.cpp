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

void                            MediaPlayer::play()
{
    libvlc_media_player_play(this->_mediaPlayer, this->_ex->getInternalPtr());
}

void                            MediaPlayer::pause()
{
    libvlc_media_player_pause(this->_mediaPlayer, this->_ex->getInternalPtr());
}

qint64                          MediaPlayer::getTime() const
{
    qint64 t = libvlc_media_player_get_time(this->_mediaPlayer, this->_ex->getInternalPtr());
    assert(this->_ex->raised() == Exception::notRaised);
    return t;
}

void                            MediaPlayer::setTime(qint64 time)
{
    libvlc_media_player_set_time(this->_mediaPlayer, time, this->_ex->getInternalPtr());
    assert(this->_ex->raised() == Exception::notRaised);
}

qint64                          MediaPlayer::getLength() const
{
    qint64 length = libvlc_media_player_get_length(this->_mediaPlayer, this->_ex->getInternalPtr());
    assert(this->_ex->raised() == Exception::notRaised);
    return length;
}
