#ifndef VST_OSTREAM_HPP
#define VST_OSTREAM_HPP

#include <ostream>
#include "pluginterfaces/vst2.x/aeffectx.h"

/// Implements streaming to ostream for VstParameterProperties
std::ostream &operator<<(std::ostream &os, const VstParameterProperties& obj);

/// Implements streaming to ostream for VstPinProperties
std::ostream &operator<<(std::ostream &os, const VstPinProperties& obj);

/// Implements streaming to ostream for VstTimeInfo
std::ostream &operator<<(std::ostream &os, const VstTimeInfo &obj);

#endif
