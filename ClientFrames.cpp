#include "ClientFrames.h"
#include <opencv2/highgui.hpp>
#include <opencv2/tracking/tracker.hpp>

client_frames::client_frames(std::string wnd_name)
{
	_first_call = true;
	_top_level_window = NULL;
	if(wnd_name.length())
	{
		_top_level_window = FindWindow(NULL, wnd_name.c_str());
		if(_top_level_window != NULL)
			_first_call = false;
	}
}
void client_frames::reset()
{
	_first_call = true;
}
cv::Mat client_frames::nextFrame()
{
	if(_first_call)
		_top_level_window = GetWindowFromPoint();
	cv::Mat res;
	if(_top_level_window != NULL)
	{
		res = hwnd2mat(_top_level_window);
	}
	return res;
}
HWND client_frames::GetWindowFromPoint()
{
	HWND hwndDesktop = GetDesktopWindow();
	cv::Mat dt = hwnd2mat(hwndDesktop);
	cv::imshow(WND_NAME_SELECT_WND, dt);
	cv::Point mouse_point(-1, -1);
	cv::setMouseCallback(WND_NAME_SELECT_WND, MousePoint, &mouse_point);
	while(1)
	{
		int key = cv::waitKey(0);
		if(key == 27 || key == -1)
			break;
	}
	if(mouse_point.x == -1 && mouse_point.y == -1)
		return NULL;
	POINT mp = {mouse_point.x, mouse_point.y};
	HWND res = WindowFromPoint(mp);
	if(res != NULL)
	{
		_first_call = false;
	}
	return res;
}

cv::Mat hwnd2mat(HWND hwnd)
{
	if(!IsWindow(hwnd))
		return cv::Mat();
    HDC hwindowDC,hwindowCompatibleDC;

    int height,width,srcheight,srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    BITMAPINFOHEADER  bi;

    hwindowDC=GetDC(hwnd);
    hwindowCompatibleDC=CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC,COLORONCOLOR);

    RECT windowsize;    // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    srcheight = windowsize.bottom;
    srcwidth = windowsize.right;
    height = windowsize.bottom/1;  //change this to whatever size you want to resize to
    width = windowsize.right/1;

    src.create(height,width,CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap( hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
    StretchBlt( hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject (hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);
    return RemoveChannel(src);
}
cv::Mat RemoveChannel(cv::Mat mat)
{
	std::vector<cv::Mat> channels;
	cv::split(mat, channels);
	channels.resize(3);
	cv::Mat img;
	cv::merge(channels, img);
	return img;
}
void MousePoint(int mouse_event, int x, int y, int flags, void* param)
{
	cv::Point* mouse_point = (cv::Point*) param;
	if(mouse_event == cv::EVENT_LBUTTONUP)
	{
		mouse_point->x = x;
		mouse_point->y = y;
		cv::destroyWindow(WND_NAME_SELECT_WND);
	}
}