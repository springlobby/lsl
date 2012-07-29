#ifndef SPRINGLOBBY_HEADERGUARD_NONPORTABLE_H
#define SPRINGLOBBY_HEADERGUARD_NONPORTABLE_H

#if defined(__WXMSW__)
  #define USYNC_CALL_CONV __stdcall
  #define SPRING_BIN _T("spring.exe")
  #define DOS_TXT true
#elif defined(__WXGTK__) || defined(__WXX11__)
  #define SPRING_VERSION_PARAM _T("-V")
  #define CHOOSE_EXE _("Any file (*)|*")
  #define USYNC_CALL_CONV
  #define SPRING_BIN _T("spring")
  #define DOS_TXT false
#elif defined(__WXMAC__)
  #define SPRING_VERSION_PARAM _T("-V")
  #define CHOOSE_EXE _("App Bundles (*.app)|*.app|Any File (*.*)|*.*")
  #define USYNC_CALL_CONV
  #define SPRING_BIN _T("Spring.app")
  #define DOS_TXT false
#endif

/**
 * \file nonportable.h
 * \section LICENSE
Copyright 2012 by The libSpringLobby team. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#endif // SPRINGLOBBY_HEADERGUARD_NONPORTABLE_H
