#include <cstdint>
#include <yangutil/yangerrorcode.h>
#include <yangutil/yangtype.h>
#include <yangutil/yangavctype.h>
#include <yangrtc/YangPeerConnection.h>
#include <yangpush/YangPushFactory.h>
#include <yangpush/VideoPush.h>

#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char const *argv[])
{
    YangPushFactory mf;
    VideoPush videoPush;
    YangSysMessageHandle *sysmessage = mf.createPushMessageHandle(videoPush.m_hasAudio, false, videoPush.m_videoType, &videoPush.m_screenInfo, &videoPush.m_outInfo, videoPush.m_context, &videoPush, &videoPush);

    videoPush.m_message = sysmessage;
    sysmessage->start();

    std::this_thread::sleep_for(std::chrono::microseconds(200));
    videoPush.initPreview();
    while (true){
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    {
        /* code */
    }
    
    return 0;
}
