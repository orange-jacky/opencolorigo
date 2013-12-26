package opencolorigo

// #include "stdlib.h"
//
// #include "ocio.h"
//
import "C"

import (
    "runtime"
    "unsafe"
)

// A Config defines all the colorspaces available at runtime.
type Config struct {
    ptr unsafe.Pointer
}

/*
Config Initialization
*/

func newConfig(p unsafe.Pointer) *Config {
    cfg := &Config{p}
    runtime.SetFinalizer(cfg, deleteConfig)
    return cfg
}

func deleteConfig(c *Config) {
    C.free(c.ptr)
}

// Get the current configuration.
// If a current config had not yet been set, it will be automatically
// initialized from the environment.
func GetCurrentConfig() (*Config, error) {
    c, err := C.GetCurrentConfig()
    if err != nil {
        return nil, err
    }
    return newConfig(c), err
}

// Create a Config by checking the OCIO environment variable
func ConfigCreateFromEnv() (*Config, error) {
    c, err := C.Config_CreateFromEnv()
    if err != nil {
        return nil, err
    }
    return newConfig(c), err
}

// Create a Config from an existing yaml Config file
func ConfigCreateFromFile(filename string) (*Config, error) {
    c_str := C.CString(filename)
    defer C.free(unsafe.Pointer(c_str))

    c, err := C.Config_CreateFromFile(c_str)
    if err != nil {
        return nil, err
    }
    return newConfig(c), err
}

// Create a Config from a valid yaml string
func ConfigCreateFromData(data string) (*Config, error) {
    c_str := C.CString(data)
    defer C.free(unsafe.Pointer(c_str))

    c, err := C.Config_CreateFromData(c_str)
    if err != nil {
        return nil, err
    }
    return newConfig(c), err
}

/*
This will produce a hash of the all colorspace definitions, etc.
All external references, such as files used in FileTransforms, etc.,
will be incorporated into the cacheID. While the contents of the files
are not read, the file system is queried for relavent information (mtime, inode)
so that the Config’s cacheID will change when the underlying luts are updated.
If a context is not provided, the current Context will be used.
If a null context is provided, file references will not be taken into account
(this is essentially a hash of Config::serialize).
*/
func (c *Config) GetCacheID() (string, error) {
    id, err := C.Config_getCacheID(c.ptr)
    if err != nil {
        return "", err
    }
    return C.GoString(id), err
}

func (c *Config) GetDescription() (string, error) {
    d, err := C.Config_getDescription(c.ptr)
    if err != nil {
        return "", err
    }
    return C.GoString(d), err
}

func (c *Config) IsStrictParsingEnabled() bool {
    enabled, err := C.Config_isStrictParsingEnabled(c.ptr)
    if err != nil {
        return false
    }
    return bool(enabled)
}

func (c *Config) SetStrictParsingEnabled(enabled bool) error {
    _, err := C.Config_setStrictParsingEnabled(c.ptr, C.bool(enabled))
    return err
}

/*
Config Resources
*/

// Given a lut src name, where should we find it?
func (c *Config) GetSearchPath() (string, error) {
    path, err := C.Config_getSearchPath(c.ptr)
    if err != nil {
        return "", err
    }
    return C.GoString(path), err
}

// Given a lut src name, where should we find it?
func (c *Config) GetWorkingDir() (string, error) {
    dir, err := C.Config_getWorkingDir(c.ptr)
    if err != nil {
        return "", err
    }
    return C.GoString(dir), err
}

/*
Config ColorSpaces
*/

func (c *Config) GetNumColorSpaces() int {
    num, err := C.Config_getNumColorSpaces(c.ptr)
    if err != nil {
        return 0
    }
    return int(num)
}

// This will null if an invalid index is specified
func (c *Config) GetColorSpaceNameByIndex(index int) (string, error) {
    name, err := C.Config_getColorSpaceNameByIndex(c.ptr, C.int(index))
    if err != nil {
        return "", err
    }
    return C.GoString(name), err
}

// Accepts either a color space OR role name. (Colorspace names take precedence over roles.)
func (c *Config) GetIndexForColorSpace(name string) (int, error) {
    c_str := C.CString(name)
    defer C.free(unsafe.Pointer(c_str))

    idx, err := C.Config_getIndexForColorSpace(c.ptr, c_str)
    if err != nil {
        return -1, err
    }
    return int(idx), err
}

/*
Config Roles

A role is like an alias for a colorspace. You can query the colorspace
corresponding to a role using the normal getColorSpace fcn.
*/

// Setting the colorSpaceName name to a null string unsets it.
func (c *Config) SetRole(role, colorSpaceName string) error {
    c_role := C.CString(role)
    defer C.free(unsafe.Pointer(c_role))

    var c_space *C.char
    if colorSpaceName != "" {
        c_space = C.CString(colorSpaceName)
        defer C.free(unsafe.Pointer(c_space))
    }

    _, err := C.Config_setRole(c.ptr, c_role, c_space)
    return err
}

func (c *Config) GetNumRoles() int {
    num, err := C.Config_getNumRoles(c.ptr)
    if err != nil {
        return 0
    }
    return int(num)
}

// Return true if the role has been defined.
func (c *Config) HasRole(role string) bool {
    c_str := C.CString(role)
    defer C.free(unsafe.Pointer(c_str))

    has, err := C.Config_hasRole(c.ptr, c_str)
    if err != nil {
        return false
    }
    return bool(has)
}

// Get the role name at index, this will return values like ‘scene_linear’,
// ‘compositing_log’. Return empty string if index is out of range.
func (c *Config) GetRoleName(index int) (string, error) {
    name, err := C.Config_getRoleName(c.ptr, C.int(index))
    if err != nil {
        return "", err
    }
    return C.GoString(name), err
}