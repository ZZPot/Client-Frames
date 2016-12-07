#pragma once
#include <windows.h>
#include <opencv2/videostab/frame_source.hpp>

// for grabbing frames from client area of pointed window

#define WND_NAME_SELECT_WND "Click at window"

class client_frames: public cv::videostab::IFrameSource
{
public:
	client_frames(std::string wnd_name = ""); // if set it searches for specified window
	void reset();
	cv::Mat nextFrame();
protected:
	HWND GetWindowFromPoint();
protected:
	bool _first_call;
	HWND _top_level_window;
};

cv::Mat hwnd2mat(HWND hwnd);
cv::Mat RemoveChannel(cv::Mat mat);
void MousePoint(int mouse_event, int x, int y, int flags, void* param);