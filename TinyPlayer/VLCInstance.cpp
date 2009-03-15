#include <cassert>
#include "VLCInstance.h"
#include <QtDebug>

using namespace LibVLCpp;

Instance::Instance(int argc, const char** argv)
{
    this->_instance = libvlc_new(argc, argv, this->_ex);
    this->_ex.checkThrow();
}

Instance::internalPtr   Instance::getInternalPtr()
{
    return this->_instance;
}
