#include <cassert>
#include "VLCMedia.h"

using namespace LibVLCpp;

Media::Media(Instance& instance, const QString& filename) : _instance(instance)
{
    this->_internalPtr = libvlc_media_new(instance, filename.toLocal8Bit(), this->_ex);
    this->_ex.checkThrow();
}

Media::~Media()
{
    libvlc_media_release(this->_internalPtr);
}

void                    Media::addOption(const char* opt)
{
    libvlc_media_add_option(this->_internalPtr, opt, this->_ex);
    this->_ex.checkThrow();
}
