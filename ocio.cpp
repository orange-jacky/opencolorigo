#include <OpenColorIO/OpenColorIO.h>

#include <iostream>
#include <sstream>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    // Global
    void ClearAllCaches() { OCIO::ClearAllCaches(); }

    const char* GetVersion() { return OCIO::GetVersion(); }

    int GetVersionHex() { return OCIO::GetVersionHex(); }

    LoggingLevel GetLoggingLevel() { return (LoggingLevel)OCIO::GetLoggingLevel(); }

    void SetLoggingLevel(LoggingLevel level) { OCIO::SetLoggingLevel((OCIO::LoggingLevel)level); };

    // Config Init
    const Config* GetCurrentConfig() {
        return (Config*) new OCIO::ConstConfigRcPtr(OCIO::GetCurrentConfig());
    }

    const Config* Config_CreateFromEnv() {
        return (Config*) new OCIO::ConstConfigRcPtr(OCIO::Config::CreateFromEnv());
    }

    const Config* Config_CreateFromFile(const char* filename) {
        return (Config*) new OCIO::ConstConfigRcPtr(OCIO::Config::CreateFromFile(filename));
    }

    const Config* Config_CreateFromData(const char* data) {
        std::stringstream s;
        s << data;
        return (Config*) new OCIO::ConstConfigRcPtr(OCIO::Config::CreateFromStream(s));
    }
    
    const char* Config_getCacheID(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getCacheID();
    }

    const char* Config_getDescription(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getDescription();
    }

    // Config Resources
    const char* Config_getSearchPath(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getSearchPath();
    }

    const char* Config_getWorkingDir(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getWorkingDir();
    }

    // Config ColorSpaces
    int Config_getNumColorSpaces(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getNumColorSpaces();
    }

    const char* Config_getColorSpaceNameByIndex(Config *p, int index) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getColorSpaceNameByIndex(index);
    }

    const ColorSpace* Config_getColorSpace(Config *p, const char* name) {
        OCIO::ConstColorSpaceRcPtr ptr = static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getColorSpace(name);
        return (ColorSpace*) new OCIO::ConstColorSpaceRcPtr(ptr);
    }

    int Config_getIndexForColorSpace(Config *p, const char* name) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getIndexForColorSpace(name);
    }

    bool Config_isStrictParsingEnabled(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->isStrictParsingEnabled();
    }

    void Config_setStrictParsingEnabled(Config *p, bool enabled) {
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->setStrictParsingEnabled(enabled);
    }

    // Config Roles
    void Config_setRole(Config *p, const char* role, const char* colorSpaceName) {
        static_cast<OCIO::ConfigRcPtr*>(p)->get()->setRole(role, colorSpaceName);
    }

    int Config_getNumRoles(Config *p) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getNumRoles();
    }

    bool Config_hasRole(Config *p, const char* role) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->hasRole(role);
    }

    const char* Config_getRoleName(Config *p, int index) {
        return static_cast<OCIO::ConstConfigRcPtr*>(p)->get()->getRoleName(index);
    }

}