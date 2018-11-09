#pragma once

#include <string>
#include <mutex>
#include "nvcuvid.h" // <nvcuvid.h>
#include "cuviddec.h"
#include "cuda.h"    // <cuda.h>
#include "FrameQueue.h"
#include <thread>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
#include <libavutil/imgutils.h>
}
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
typedef HMODULE CUDADRIVER;
#else
typedef void *CUDADRIVER;
#endif

class NvDecode
{
public:
	NvDecode();
	~NvDecode();

	CUvideodecoder m_videoDecoder;
	FrameQueue*    m_pFrameQueue;
	CUVIDDECODECREATEINFO m_oVideoDecodeCreateInfo;
	virtual int start(std::string &rtspUrl);
	virtual bool deQueueFrame(unsigned char **rgbaP, int *width, int *height, unsigned long long *timestamp);
	virtual void quit();

private:
	typedef enum
	{
		ITU601 = 1,
		ITU709 = 2
	} eColorSpace;
	std::string _rtspUrl;
	bool _quitFlag = false;
	std::mutex mtx;
	CUDADRIVER hHandleDriver = 0;
	CUcontext cudaCtx;
	CUdevice device;
	CUcontext curCtx;
	CUvideoctxlock ctxLock;
	CUvideoparser  m_videoParser;
	CUdeviceptr g_pInteropFrame{0};
	unsigned char *rgbaBuf = nullptr;
	std::thread *decodeThread = nullptr;
	CUmodule module;
	CUfunction g_kernelNV12toARGB = nullptr;
	bool isFirstFrame = true;

	int videoindex = -1;
	int targetWidth, targetHeight;
	AVBitStreamFilterContext *h264bsfc = nullptr;
	AVFormatContext* pFormatCtx = nullptr;
	AVCodecContext* pCodecCtx = nullptr;

	void startDecode();
	void setColorSpaceMatrix(eColorSpace CSC, float *hueCSC, float hue);
	CUresult updateConstantMemory_drvapi(CUmodule module, float *hueCSC);

	static int CUDAAPI HandleVideoSequence(void* pUserData, CUVIDEOFORMAT* pFormat);
	static int CUDAAPI HandlePictureDecode(void* pUserData, CUVIDPICPARAMS* pPicParams);
	static int CUDAAPI HandlePictureDisplay(void* pUserData, CUVIDPARSERDISPINFO* pPicParams);
};

