#pragma once

/*
 *      Copyright (C) 2005-2008 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

class CFileItem;
#include "GUIDialog.h"

// defines here
#define TMSG_DIALOG_DOMODAL       100
#define TMSG_WRITE_SCRIPT_OUTPUT  101
#define TMSG_EXECUTE_SCRIPT       102
#define TMSG_EXECUTE_BUILT_IN     103

#define TMSG_MEDIA_PLAY           200
#define TMSG_MEDIA_STOP           201
#define TMSG_MEDIA_PAUSE          202
#define TMSG_MEDIA_RESTART        203

#define TMSG_PLAYLISTPLAYER_PLAY  210
#define TMSG_PLAYLISTPLAYER_NEXT  211
#define TMSG_PLAYLISTPLAYER_PREV  212

#define TMSG_PICTURE_SHOW         220
#define TMSG_PICTURE_SLIDESHOW    221
#define TMSG_SLIDESHOW_SCREENSAVER  222

#define TMSG_SHUTDOWN             300
#define TMSG_DASHBOARD            301
#define TMSG_RESTART              302
#define TMSG_RESET                303
#define TMSG_RESTARTAPP           304
#define TMSG_SWITCHTOFULLSCREEN   305
#define TMSG_SLEEPSYSTEM          307

#define TMSG_TOGGLEFULLSCREEN     320
#define TMSG_MOVE_TO_PREV_SCREEN  321
#define TMSG_MOVE_TO_NEXT_SCREEN  322
#define TMSG_ACTIVATE_VISUALIZER  323

#define TMSG_HTTPAPI              400

#define TMSG_NETWORKMESSAGE         500

#define TMSG_GUI_DO_MODAL             600
#define TMSG_GUI_SHOW                 601
#define TMSG_GUI_WIN_MANAGER_PROCESS  602
#define TMSG_GUI_WIN_MANAGER_RENDER   603
#define TMSG_GUI_ACTIVATE_WINDOW      604

#define TMSG_GUI_UPDATE_COCOA_DIALOGS 700

typedef struct
{
  DWORD dwMessage;
  DWORD dwParam1;
  DWORD dwParam2;
  std::string strParam;
  HANDLE hWaitEvent;
  LPVOID lpVoid;
}
ThreadMessage;

class CApplicationMessenger
{

public:
  ~CApplicationMessenger();

  void Cleanup();
  // if a message has to be send to the gui, use MSG_TYPE_WINDOW instead
  void SendMessage(ThreadMessage& msg, bool wait = false);
  void ProcessMessages(); // only call from main thread.
  void ProcessWindowMessages();

  void MediaPlay(std::string filename);
  void MediaStop();
  void MediaPause();
  void MediaRestart(bool bWait);

  void PlayListPlayerPlay();
  void PlayListPlayerPlay(int iSong);
  void PlayListPlayerNext();
  void PlayListPlayerPrevious();
  void PlayFile(const CFileItem &item, bool bRestart = false); // thread safe version of g_application.PlayFile()
  void PictureShow(std::string filename);
  void PictureSlideShow(std::string pathname, bool bScreensaver = false);
  void Shutdown();
  void Restart();
  void RebootToDashBoard();
  void RestartApp();
  void Reset();
  void SwitchToFullscreen(); //
  void ToggleFullscreen();
  void SleepSystem();
  void ActivateVisualizer();
  void MoveToNextScreen();
  void MoveToPrevScreen();
  
  CStdString GetResponse();
  int SetResponse(CStdString response);
  void HttpApi(std::string cmd, bool wait = false);
  void ExecBuiltIn(const CStdString &command);

  void NetworkMessage(DWORD dwMessage, DWORD dwParam = 0);

  void DoModal(CGUIDialog *pDialog, int iWindowID = WINDOW_INVALID, const CStdString &param = "");
  void Show(CGUIDialog *pDialog);
  void WindowManagerProcess(bool renderOnly = false); // will call m_gWindowManager.Process on the rendering thread
  void Render(); // will call m_gWindowManager.Render on the rendering thread
  void ActivateWindow(int windowID, const CStdString &path, bool swappingWindows);

private:
  void ProcessMessage(ThreadMessage *pMsg);

  std::queue<ThreadMessage*> m_vecMessages;
  std::queue<ThreadMessage*> m_vecWindowMessages;
  CCriticalSection m_critSection;
  CCriticalSection m_critBuffer;
  CStdString bufferResponse;
};
