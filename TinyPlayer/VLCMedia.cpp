#include <cassert>
#include "VLCMedia.h"

using namespace LibVLCpp;

Media::Media(Instance* instance, const QString& filename) : _instance(instance)
{
    this->_ex = new Exception();
    libvlc_media_new(instance->getInternalPtr(), filename.toLocal8Bit(), this->_ex->getInternalPtr());
    assert(this->_ex->raised() == Exception::notRaised);
}

Media::~Media()
{
    libvlc_media_release(this->_media);
}

Media::internalPtr      Media::getInternalPtr()
{
    return this->_media;
}

void                    Media::addOption(const char* opt)
{
    libvlc_media_add_option(this->_media, opt, this->_ex->getInternalPtr());
    assert(this->_ex->raised() == Exception::notRaised);
}
