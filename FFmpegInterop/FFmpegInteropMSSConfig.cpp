//*****************************************************************************
//
//	Copyright 2019 Microsoft Corporation
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
//
//	http ://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
//*****************************************************************************

#include "pch.h"
#include "FFmpegInteropMSSConfig.h"
#include "FFmpegInteropMSSConfig.g.cpp"

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::FFmpegInterop::implementation
{
    bool FFmpegInteropMSSConfig::IsMediaSourceAppService()
    {
        return m_isMediaSourceAppService;
    }

    void FFmpegInteropMSSConfig::IsMediaSourceAppService(_In_ bool isMediaSourceAppService)
    {
        m_isMediaSourceAppService = isMediaSourceAppService;
    }

    bool FFmpegInteropMSSConfig::ForceAudioDecode()
    {
        return m_forceAudioDecode;
    }

    void FFmpegInteropMSSConfig::ForceAudioDecode(_In_ bool forceAudioDecode)
    {
        m_forceAudioDecode = forceAudioDecode;
    }

    bool FFmpegInteropMSSConfig::ForceVideoDecode()
    {
        return m_forceVideoDecode;
    }

    void FFmpegInteropMSSConfig::ForceVideoDecode(_In_ bool forceVideoDecode)
    {
        m_forceVideoDecode = forceVideoDecode;
    }

    uint32_t FFmpegInteropMSSConfig::AllowedDecodeErrors()
    {
        return m_allowedDecodeErrors;
    }

    void FFmpegInteropMSSConfig::AllowedDecodeErrors(_In_ uint32_t allowedDecodeErrors)
    {
        m_allowedDecodeErrors = allowedDecodeErrors;
    }

    StringMap FFmpegInteropMSSConfig::FFmpegOptions()
    {
        return m_ffmpegOptions;
    }
}
