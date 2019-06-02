#pragma once

#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
#include <memory>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <cmath>
#include <limits>

#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <list>
#include <array>
#include <initializer_list>

#include "spdlog/fmt/ostr.h"

#include "src/common/timer.h"
#include "src/common/error.h"
#include "src/common/log.h"
#include "src/common/singleton.h"
#include "src/common/stronghandle.h"
#include "src/common/handledobject.h"
#include "src/common/assert.h"

#include "src/events/event.h"
#include "src/events/keyevent.h"
#include "src/events/mouseevent.h"
#include "src/events/applicationevent.h"