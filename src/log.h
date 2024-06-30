#pragma once

#define HE_LOG(_stream,_priority,_what) std::_stream << "["<< #_priority << "] " << _what << std::endl;
#define HE_LOG_COUT(_priority,_what) HE_LOG(cout,_priority,_what)
#define HE_LOG_CERR(_priority,_what) HE_LOG(cerr,_priority,_what)
#ifdef HE_DEBUG
  #define HE_LOG_DBG(_what) HE_LOG_COUT(debug,_what)
#else  // HE_DEBUG
  #define HE_LOG_DBG(_what)
#endif // HE_DEBUG
#define HE_LOG_NFO(_what) HE_LOG_COUT(info    ,_what)
#define HE_LOG_WRN(_what) HE_LOG_CERR(warning ,_what)
#define HE_LOG_ERR(_what) HE_LOG_CERR(error   ,_what)
#define HE_LOG_CRY(_what) HE_LOG_CERR(critical,_what)
#define HE_ABORT(_why) { HE_LOG_CRY("Application closed. " << _why); std::exit(EXIT_FAILURE); }

#ifdef HE_DEBUG
  #define RE_LOG_LINE(_caption) std::string(std::string(_caption " in file " __FILE__ " at line " RE_STR(__LINE__) " in method ") + std::string(__func__))
#else  // HE_DEBUG
  #define RE_LOG_LINE(_caption)
#endif // HE_DEBUG
