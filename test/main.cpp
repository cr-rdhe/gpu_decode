#include "NvDecode.h"
#include "opencv.hpp"
#include <iostream>

int main()
{
	NvDecode decod;//rtsp://admin:123ABCabc@192.168.2.236:554/Streaming/Channels/102?transportmode=unicast&profile=Profile_1
	//rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov
	decod.start(std::string("rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov"));
	unsigned char *rgbaPtr = nullptr;
	int width = 0, height = 0;
	unsigned long long timestamp = 0;
	while (!decod.m_pFrameQueue->isEndOfDecode())
	{
		if (decod.deQueueFrame(&rgbaPtr, &width, &height, &timestamp)) {
			cv::Mat frame(height, width, CV_8UC4, rgbaPtr);
			cv::imshow("video", frame);
			cv::waitKey(30);
		}
		else {
			cv::waitKey(20);
			continue;
		}
	}
	return 0;
}
