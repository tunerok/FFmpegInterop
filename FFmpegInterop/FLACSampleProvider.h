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

#include "SampleProvider.h"

namespace winrt::FFmpegInterop::implementation
{
	class FLACSampleProvider :
		public SampleProvider
	{
	public:
		FLACSampleProvider(_In_ const AVFormatContext* formatContext, _In_ AVStream* stream, _In_ Reader& reader);

	protected:
		std::tuple<Windows::Storage::Streams::IBuffer, int64_t, int64_t, std::vector<std::pair<GUID, Windows::Foundation::IInspectable>>, std::vector<std::pair<GUID, Windows::Foundation::IInspectable>>> GetSampleData() override;
	
		static constexpr uint32_t FLAC_STREAMINFO_SIZE{ 34 };
		static constexpr std::array<uint8_t, 4> FLAC_MARKER{ 'f', 'L', 'a', 'C' };
		static constexpr std::array<uint8_t, 4> FLAC_STREAMINFO_HEADER{ 0x80, 0x00, 0x00, 0x22 };

		_Field_size_(FLAC_STREAMINFO_SIZE) const uint8_t* m_flacStreamInfo{ nullptr };
	};
}
