#pragma once

#include "log.h"

#define HE_STN_DEFINITION_TMPL(_type,_name) \
    public: _type &_name()

#define HE_STN_DECLARATION_TMPL(_type) \
    { \
        static _type value = _type(); \
        return value; \
    }

#define HE_STN_DEFINITION(_type,_name) \
    HE_STN_DEFINITION_TMPL(_type,_name);

#define HE_STN_DECLARATION(_target_type,_st_type,_name) \
    _target_type &_st_type::_name() \
    HE_STN_DECLARATION_TMPL(_target_type)

#define HE_SIMPLE_SINGLTON_DECLARATION(_type,_name) \
    HE_STN_DEFINITION_TMPL(_type,_name) \
    HE_STN_DECLARATION_TMPL(_type)
