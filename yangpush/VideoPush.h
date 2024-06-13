
#ifndef VIDEOPUSH_H
#define VIDEOPUSH_H

#include <yangutil/yangavinfotype.h>
#include <yangutil/sys/YangSysMessageI.h>
#include <yangutil/sys/YangSysMessageHandle.h>
#include <yangpush/YangPushFactory.h>
#define Yang_SendVideo_ 0
class VideoPush : public YangSysMessageI, public YangSysMessageHandleI
{
public:
    VideoPush();
    ~VideoPush();
    YangContext *m_context;
    int m_videoType;
    bool m_hasAudio;
    YangVideoInfo m_screenInfo;
    YangVideoInfo m_outInfo;
    YangSysMessageHandle *m_message;

public:
    void initPreview();
    void success();
    void failure(int32_t errcode);
    void receiveSysMessage(YangSysMessage *psm, int32_t phandleRet);

private:
    int m_screenWidth;
    int m_screenHeight;
    int32_t m_isStartpush;
    string m_url = "http://175.178.181.116:1985/rtc/v1/whip/?app=live&stream=livestream";
    int32_t m_screenInternal;
    YangPushFactory m_pushfactory;

private:
    void init();
    void closeAll();
};
#endif