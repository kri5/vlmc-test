#include "VLCException.h"

using namespace LibVLCpp;

Exception::Exception()
{
    this->_ex = new libvlc_exception_t;
    libvlc_exception_init(this->_ex);
}

Exception::~Exception()
{
    this->clear();
    delete this->_ex;
}

Exception::internalPtr  Exception::getInternalPtr()
{
    return this->_ex;
}

const char* Exception::getErrorText() const
{
    return libvlc_exception_get_message(this->_ex);
}

void        Exception::clear()
{
    libvlc_exception_clear(this->_ex);
}

int         Exception::raised() const
{
    return libvlc_exception_raised(this->_ex);
}
