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

#pragma once
#include "FFmpegInteropMSSConfig.g.h"

namespace winrt::FFmpegInterop::implementation
{
    class FFmpegInteropMSSConfig :
        public FFmpegInteropMSSConfigT<FFmpegInteropMSSConfig>
    {
    public:
        FFmpegInteropMSSConfig() = default;

        bool IsMediaSourceAppService();
        void IsMediaSourceAppService(_In_ bool isMediaSourceAppService);
        bool ForceAudioDecode();
        void ForceAudioDecode(_In_ bool forceAudioDecode);
        bool ForceVideoDecode();
        void ForceVideoDecode(_In_ bool forceVideoDecode);
        uint32_t AllowedDecodeErrors();
        void AllowedDecodeErrors(_In_ uint32_t allowedDecodeErrors);
        Windows::Foundation::Collections::StringMap FFmpegOptions();

        static constexpr uint32_t kAllowedDecodeErrorsDefault{ 10 };

    private:
        bool m_isMediaSourceAppService{ false };
        bool m_forceAudioDecode{ false };
        bool m_forceVideoDecode{ false };
        uint32_t m_allowedDecodeErrors{ kAllowedDecodeErrorsDefault };
        Windows::Foundation::Collections::StringMap m_ffmpegOptions;
    };
}

namespace winrt::FFmpegInterop::factory_implementation
{
    struct FFmpegInteropMSSConfig :
        public FFmpegInteropMSSConfigT<FFmpegInteropMSSConfig, implementation::FFmpegInteropMSSConfig>
    {

    };
}
