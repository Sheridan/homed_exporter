#pragma once

#define HE_SIMPLE_GETTER(_type,_name) const _type &_name() const { return m_##_name; };
