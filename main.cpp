#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "ScreenFrames/PartFrames/WindowFrames/WindowFrames.h"

int main()
{
	window_frames cf;
	while(cv::waitKey(15) != 27)
	{
		cv::Mat frame = cf.nextFrame();
		if(frame.empty())
			break;
		cv::imshow("testWnd", frame);
	}
	return 0;
}