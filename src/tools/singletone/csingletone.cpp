#include "tools/singletone/csingletone.h"
#include <iostream>
#include <stdlib.h>
#include "st.h"

namespace he
{

CSingleTone &CSingleTone::instance()
{
  static he::CSingleTone instance;
  return instance;
}

void CSingleTone::destruct()
{
  delete this;
}

CSingleTone:: CSingleTone() {}
CSingleTone::~CSingleTone() {}

}
