#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <queue>
#include <tuple>

#include <assert.h>

#ifdef _WIN32
#include <SDKDDKVer.h>
#endif //_WIN32

#include <stdexcept>
#include <type_traits>

#include "ErrorDef.h"

using boost::asio::ip::tcp;

#define MAXLENGTH 1024

#define SENDDATACHUNKSIZE 1024