#include <cassert>
#include "VLCMediaPlayer.h"

using namespace LibVLCpp;

MediaPlayer::MediaPlayer(Media* media)
{
    this->_mediaPlayer = libvlc_media_player_new_from_media(media->getInternalPtr(), this->_ex);
    this->_ex.checkThrow();
}

MediaPlayer::internalPtr        MediaPlayer::getInternalPtr()
{
    return this->_mediaPlayer;
}

void                            MediaPlayer::play()
{
    libvlc_media_player_play(this->_mediaPlayer, this->_ex);
    this->_ex.checkThrow();
}

void                            MediaPlayer::pause()
{
    libvlc_media_player_pause(this->_mediaPlayer, this->_ex);
    this->_ex.checkThrow();
}

qint64                          MediaPlayer::getTime()
{
    qint64 t = libvlc_media_player_get_time(this->_mediaPlayer, this->_ex);
    this->_ex.checkThrow();
    return t;
}

void                            MediaPlayer::setTime(qint64 time)
{
    libvlc_media_player_set_time(this->_mediaPlayer, time, this->_ex);
    this->_ex.checkThrow();
}

qint64                          MediaPlayer::getLength()
{
    qint64 length = libvlc_media_player_get_length(this->_mediaPlayer, this->_ex);
    this->_ex.checkThrow();
    return length;
}
