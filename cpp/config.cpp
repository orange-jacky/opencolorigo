#include <OpenColorIO/OpenColorIO.h>

#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

#include "ocio.h"


extern "C" {

    namespace OCIO = OCIO_NAMESPACE;

    // Config Init
    Config* Config_Create() {
        return (Config*) NEW_CONTEXT(new OCIO::ConfigRcPtr(OCIO::Config::Create()));
    }

    const Config* GetCurrentConfig() {
        Config* c = NEW_CONTEXT();
        BEGIN_CATCH_ERR
        c->handle = new OCIO::ConstConfigRcPtr(OCIO::GetCurrentConfig());
        END_CATCH_CTX_ERR(c)
        return c;
    }

    void SetCurrentConfig(Config *p) {
        BEGIN_CATCH_ERR
        OCIO::SetCurrentConfig(* static_cast<OCIO::ConstConfigRcPtr*>(p->handle));
        END_CATCH_CTX_ERR(p)
    } 

    const Config* Config_CreateFromEnv() {
        Config* c = NEW_CONTEXT();
        BEGIN_CATCH_ERR
        c->handle = new OCIO::ConstConfigRcPtr(OCIO::Config::CreateFromEnv());
        END_CATCH_CTX_ERR(c)
        return c;
    }

    const Config* Config_CreateFromFile(const char* filename) {
        Config* c = NEW_CONTEXT();
        BEGIN_CATCH_ERR
        c->handle = new OCIO::ConstConfigRcPtr(OCIO::Config::CreateFromFile(filename));
        END_CATCH_CTX_ERR(c)
        return c;
    }

    const Config* Config_CreateFromData(const char* data) {
        std::stringstream s;
        s << data;
        s.seekp(0);

        Config* c = NEW_CONTEXT();

        BEGIN_CATCH_ERR
        c->handle = new OCIO::ConstConfigRcPtr(OCIO::Config::CreateFromStream(s));
        END_CATCH_CTX_ERR(c)
        
        return c;
    }

    Config* Config_createEditableCopy(Config *p) {
        Config* cpy = NEW_CONTEXT();
        BEGIN_CATCH_ERR
        cpy->handle = new OCIO::ConfigRcPtr(static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->createEditableCopy());
        END_CATCH_CTX_ERR(p)
        return cpy;
    }

    void Config_sanityCheck(Config *p) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->sanityCheck();
        END_CATCH_CTX_ERR(p)
    }

    char* Config_serialize(Config *p) {
        std::stringstream s;
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->serialize(s);
        END_CATCH_CTX_ERR(p)

        char* cstr = new char[s.str().length()+1];
        std::strcpy(cstr, s.str().c_str());

        return cstr;
    }

    const char* Config_getCacheID(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getCacheID();
        END_CATCH_CTX_ERR(p)
    }

    const char* Config_getCacheIDWithContext(Config *p, Context *c) {
        BEGIN_CATCH_ERR
        OCIO::ConstContextRcPtr context = *(static_cast<OCIO::ConstContextRcPtr*>(c));
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getCacheID(context);
        END_CATCH_CTX_ERR(p)
    }

    const char* Config_getDescription(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getDescription();
        END_CATCH_CTX_ERR(p)
    }

    // Config Resources
    Context* Config_getCurrentContext(Config *p) {
        OCIO::ConstContextRcPtr ptr;

        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getCurrentContext();
        END_CATCH_CTX_ERR(p)
        
        return (Context*) new OCIO::ConstContextRcPtr(ptr);
    }

    const char* Config_getSearchPath(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getSearchPath();
        END_CATCH_CTX_ERR(p)
    }

    const char* Config_getWorkingDir(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getWorkingDir();
        END_CATCH_CTX_ERR(p)
    }

    // Config Processors 
    Processor* Config_getProcessor_CT_CS_CS(Config *p, Context* ct, ColorSpace* srcCS, ColorSpace* dstCS) {
        OCIO::ConstProcessorRcPtr   ptr;
        OCIO::ConstContextRcPtr     ct_ptr    = * static_cast<OCIO::ConstContextRcPtr*>(ct);
        OCIO::ConstColorSpaceRcPtr  srcCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(srcCS);
        OCIO::ConstColorSpaceRcPtr  dstCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(dstCS);

        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getProcessor(ct_ptr, srcCS_ptr, dstCS_ptr);
        END_CATCH_CTX_ERR(p)

        return (Processor*) NEW_CONTEXT(new OCIO::ConstProcessorRcPtr(ptr));
    }

    Processor* Config_getProcessor_CS_CS(Config *p, ColorSpace* srcCS, ColorSpace* dstCS) {
        OCIO::ConstProcessorRcPtr   ptr;
        OCIO::ConstColorSpaceRcPtr  srcCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(srcCS);
        OCIO::ConstColorSpaceRcPtr  dstCS_ptr = * static_cast<OCIO::ConstColorSpaceRcPtr*>(dstCS);
        
        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getProcessor(srcCS_ptr, dstCS_ptr);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return NULL; }
        return (Processor*) new OCIO::ConstProcessorRcPtr(ptr);
    }

    Processor* Config_getProcessor_S_S(Config *p, const char* srcName, const char* dstName) {
        OCIO::ConstProcessorRcPtr ptr;
        
        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getProcessor(srcName, dstName);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return NULL; }
        return (Processor*) new OCIO::ConstProcessorRcPtr(ptr);
    }

    Processor* Config_getProcessor_CT_S_S(Config *p, Context* ct, const char* srcName, const char* dstName) {
        OCIO::ConstProcessorRcPtr   ptr;
        OCIO::ConstContextRcPtr     ct_ptr = * static_cast<OCIO::ConstContextRcPtr*>(ct);

        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getProcessor(ct_ptr, srcName, dstName);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return NULL; }
        return (Processor*) new OCIO::ConstProcessorRcPtr(ptr);
    }

    // Config ColorSpaces
    int Config_getNumColorSpaces(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getNumColorSpaces();
        END_CATCH_CTX_ERR(p)
    }

    const char* Config_getColorSpaceNameByIndex(Config *p, int index) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getColorSpaceNameByIndex(index);
        END_CATCH_CTX_ERR(p)
    }

    const ColorSpace* Config_getColorSpace(Config *p, const char* name) {
        OCIO::ConstColorSpaceRcPtr ptr;

        BEGIN_CATCH_ERR
        ptr = static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getColorSpace(name);
        END_CATCH_CTX_ERR(p)

        if ( ptr == NULL) { return NULL; }
        return (ColorSpace*) new OCIO::ConstColorSpaceRcPtr(ptr);
    }

    int Config_getIndexForColorSpace(Config *p, const char* name) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getIndexForColorSpace(name);
        END_CATCH_CTX_ERR(p)
    }

    void Config_addColorSpace(Config *p, ColorSpace *cs) {
        OCIO::ConstColorSpaceRcPtr colorspace = *(static_cast<OCIO::ConstColorSpaceRcPtr*>(cs));
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p->handle)->get()->addColorSpace(colorspace);
        END_CATCH_CTX_ERR(p)
    }

    void Config_clearColorSpaces(Config *p) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p->handle)->get()->clearColorSpaces();
        END_CATCH_CTX_ERR(p)
    }

    const char* Config_parseColorSpaceFromString(Config *p, const char* str) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->parseColorSpaceFromString(str);
        END_CATCH_CTX_ERR(p)
    }

    bool Config_isStrictParsingEnabled(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->isStrictParsingEnabled();
        END_CATCH_CTX_ERR(p)
    }

    void Config_setStrictParsingEnabled(Config *p, bool enabled) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p->handle)->get()->setStrictParsingEnabled(enabled);
        END_CATCH_CTX_ERR(p)
    }

    // Config Roles
    void Config_setRole(Config *p, const char* role, const char* colorSpaceName) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p->handle)->get()->setRole(role, colorSpaceName);
        END_CATCH_CTX_ERR(p)
    }

    int Config_getNumRoles(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getNumRoles();
        END_CATCH_CTX_ERR(p)
    }

    bool Config_hasRole(Config *p, const char* role) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->hasRole(role);
        END_CATCH_CTX_ERR(p)
    }

    const char* Config_getRoleName(Config *p, int index) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getRoleName(index);
        END_CATCH_CTX_ERR(p)
    }

    // Config Display/View Registration 
    const char* Config_getDefaultDisplay(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getDefaultDisplay();
        END_CATCH_CTX_ERR(p)
    }
    
    int Config_getNumDisplays(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getNumDisplays();
        END_CATCH_CTX_ERR(p)
    }
    
    const char* Config_getDisplay(Config *p, int index) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getDisplay(index);
        END_CATCH_CTX_ERR(p)
    }
    
    const char* Config_getDefaultView(Config *p, const char* display) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getDefaultView(display);
        END_CATCH_CTX_ERR(p)
    }
    
    int Config_getNumViews(Config *p, const char* display) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getNumViews(display);
        END_CATCH_CTX_ERR(p)
    }
    
    const char* Config_getView(Config *p, const char* display, int index) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getView(display, index);
        END_CATCH_CTX_ERR(p)
    }
    
    const char* Config_getDisplayColorSpaceName(Config *p, const char* display, const char* view) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getDisplayColorSpaceName(display, view);
        END_CATCH_CTX_ERR(p)
    }
    
    const char* Config_getDisplayLooks(Config *p, const char* display, const char* view) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getDisplayLooks(display, view);
        END_CATCH_CTX_ERR(p)
    }
    
    void Config_addDisplay(Config *p, const char* display, const char* view, const char* colorSpaceName, const char* looks) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p->handle)->get()->addDisplay(display, view, colorSpaceName, looks);
        END_CATCH_CTX_ERR(p)
    }
    
    void Config_clearDisplays(Config *p) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p->handle)->get()->clearDisplays();
        END_CATCH_CTX_ERR(p)
    }
    
    void Config_setActiveDisplays(Config *p, const char* displays) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p->handle)->get()->setActiveDisplays(displays);
        END_CATCH_CTX_ERR(p)
    }
    
    const char* Config_getActiveDisplays(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getActiveDisplays();
        END_CATCH_CTX_ERR(p)
    }
    
    void Config_setActiveViews(Config *p, const char* views) {
        BEGIN_CATCH_ERR
        static_cast<OCIO::ConfigRcPtr*>(p->handle)->get()->setActiveViews(views);
        END_CATCH_CTX_ERR(p)
    }

    const char* Config_getActiveViews(Config *p) {
        BEGIN_CATCH_ERR
        return static_cast<OCIO::ConstConfigRcPtr*>(p->handle)->get()->getActiveViews();
        END_CATCH_CTX_ERR(p)
    }

}