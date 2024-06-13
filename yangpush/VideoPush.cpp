#include <yangpush/VideoPush.h>
#include <yangpush/YangPushCommon.h>
#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangFile.h>
#include <yangutil/sys/YangSocket.h>
#include <yangutil/sys/YangIni.h>
#include <yangutil/sys/YangString.h>
#include <yangpush/YangPushFactory.h>

#include <iostream>
#include <chrono>
#include <thread>

VideoPush::VideoPush()
{
    m_context = new YangContext();
    m_context->init((char *)"yang_config.ini");
    m_context->avinfo.video.videoEncoderFormat = YangI420;
#if Yang_Enable_Openh264
    m_context->avinfo.enc.createMeta = 0;
#else
    m_context->avinfo.enc.createMeta = 1;
#endif

#if Yang_Enable_GPU_Encoding
    // using gpu encode
    m_context->avinfo.video.videoEncHwType = YangV_Hw_Nvdia; // YangV_Hw_Intel,  YangV_Hw_Nvdia,
    m_context->avinfo.video.videoEncoderFormat = YangI420;   // YangI420;//YangArgb;
    m_context->avinfo.enc.createMeta = 0;
#endif

    init();
    yang_setLogLevel(m_context->avinfo.sys.logLevel);
    yang_setLogFile(m_context->avinfo.sys.enableLogFile);
    m_context->avinfo.sys.mediaServer = Yang_Server_Whip_Whep; // Yang_Server_Srs/Yang_Server_Zlm/Yang_Server_Whip_Whep
    m_context->avinfo.rtc.rtcLocalPort = 10000 + yang_random() % 15000;

    m_videoType = Yang_VideoSrc_Camera; // Yang_VideoSrc_Camera/Yang_VideoSrc_Screen;

    m_isStartpush = 0;

    char s[128] = {0};
    memset(m_context->avinfo.rtc.localIp, 0, sizeof(m_context->avinfo.rtc.localIp));
    yang_getLocalInfo(m_context->avinfo.sys.familyType, m_context->avinfo.rtc.localIp);
    // sprintf(s, "http://%s:1985/rtc/v1/whip/?app=live&stream=livestream", m_context->avinfo.rtc.localIp);
    // sprintf(s, "http://%s:1985/rtc/v1/whip/?app=live&stream=livestream", "175.178.181.116");
    m_screenInternal = 33;
    // m_hasAudio = m_videoType == Yang_VideoSrc_Screen ? false : true;
     m_hasAudio = m_videoType == yangfalse;
    m_context->avinfo.video.videoEncoderType = Yang_VED_H264; // Yang_VED_265;
    if (m_context->avinfo.video.videoEncoderType == Yang_VED_H265)
    {
        m_context->avinfo.enc.createMeta = 1;
    }
    m_context->avinfo.rtc.iceCandidateType = YangIceHost;

    // srs do not use audio fec
    m_context->avinfo.audio.enableAudioFec = yangfalse;
}

VideoPush::~VideoPush()
{
    closeAll();
}

void VideoPush::closeAll()
{

    if (m_context)
    {
        yang_delete(m_message);
        yang_delete(m_context);
    }
}

void VideoPush::receiveSysMessage(YangSysMessage *mss, int32_t err)
{
    printf("msgcode:%d\n", mss->messageId);

    switch (mss->messageId)
    {
    case YangM_Push_Connect:
    {
        if (err)
        {
            m_isStartpush = !m_isStartpush;
            // QMessageBox::about(NULL,  "error","push error("+QString::number(err)+")!");
        }
    }
    break;
    case YangM_Push_Disconnect:
        break;
    case YangM_Push_StartScreenCapture:
        if (!m_context->avinfo.video.videoEncoderFormat == YangArgb)
            // YangVideoBuffer *buffer = m_pushfactory.getPreVideoBuffer(m_message);
        break;
    case YangM_Push_StartVideoCapture:
    {
        // YangVideoBuffer *buffer = m_pushfactory.getPreVideoBuffer(m_message);
        m_isStartpush = !m_isStartpush;
        printf("url:%s\n",m_url.c_str());
        std::this_thread::sleep_for(std::chrono::microseconds(200));
        yang_post_message(YangM_Push_Connect_Whip, 0, NULL, (void *)m_url.c_str());
        break;
    }
    case YangM_Push_StartOutCapture:
    {
        // YangVideoBuffer *buffer = m_pushfactory.getPreVideoBuffer(m_message);
        break;
    }
    case YangM_Push_SwitchToCamera:
        // YangVideoBuffer *buffer = m_pushfactory.getPreVideoBuffer(m_message);
        break;
        // case YangM_Push_SwitchToScreen:
        //     YangVideoBuffer *buffer = m_pushfactory.getPreVideoBuffer(m_message);
        //     break;
    }
}

void VideoPush::initPreview()
{
    if (m_videoType == Yang_VideoSrc_Screen)
    {

        yang_post_message(YangM_Push_StartScreenCapture, 0, NULL);
    }
    else if (m_videoType == Yang_VideoSrc_Camera)
    {
        yang_post_message(YangM_Push_StartVideoCapture, 0, NULL);
    }
    else if (m_videoType == Yang_VideoSrc_OutInterface)
    {
        yang_post_message(YangM_Push_StartOutCapture, 0, NULL);
    }
}

void VideoPush::init()
{
    m_context->avinfo.audio.enableMono = yangfalse;
    m_context->avinfo.audio.sample = 48000;
    m_context->avinfo.audio.channel = 2;
    m_context->avinfo.audio.enableAec = yangfalse;
    m_context->avinfo.audio.audioCacheNum = 8;
    m_context->avinfo.audio.audioCacheSize = 8;
    m_context->avinfo.audio.audioPlayCacheNum = 8;

    m_context->avinfo.video.videoCacheNum = 10;
    m_context->avinfo.video.evideoCacheNum = 10;
    m_context->avinfo.video.videoPlayCacheNum = 10;

    m_context->avinfo.audio.audioEncoderType = Yang_AED_OPUS;
    m_context->avinfo.rtc.rtcLocalPort = 17000;
    m_context->avinfo.enc.enc_threads = 4;

    memcpy(&m_screenInfo, &m_context->avinfo.video, sizeof(YangVideoInfo));
    m_screenInfo.width = m_screenWidth;
    m_screenInfo.height = m_screenHeight;
    m_screenInfo.outWidth = m_screenWidth;
    m_screenInfo.outHeight = m_screenHeight;
}

void VideoPush::success()
{
    printf("推流成功！");
}

void VideoPush::failure(int32_t errcode)
{
    printf("push error(%d)!", errcode);
}