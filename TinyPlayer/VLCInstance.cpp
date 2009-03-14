#include <cassert>
#include "VLCInstance.h"
#include <QtDebug>

using namespace LibVLCpp;

Instance::Instance(int argc, const char** argv)
{
    this->_ex = new Exception();
    this->_instance = libvlc_new(argc, argv, this->_ex->getInternalPtr());
    assert(this->_ex->raised() == Exception::notRaised);
}

Instance::internalPtr   Instance::getInternalPtr()
{
    return this->_instance;
}
