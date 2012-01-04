/* Copyright (C) 2007 The SpringLobby Team. All rights reserved. */

#include <wx/tokenzr.h>
#include <wx/image.h>
#include <sstream>
#include <wx/timer.h>

#include "ibattle.h"
#include "utils/debug.h"
#include "utils/conversion.h"
#include "utils/math.h"
#include "uiutils.h"
#include "settings.h"
#include "ui.h" //only required for preset stuff
#include "spring.h"
#include "springunitsynclib.h"
#include "springlobbyapp.h"

#include <list>
#include <algorithm>
#include <cmath>
#include <set>

const unsigned int TIMER_ID         = 102;

