//*****************************************************************************
//
//	Copyright 2015 Microsoft Corporation
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
#include "Reader.h"
#include "SampleProvider.h"

using namespace std;

namespace winrt::FFmpegInterop::implementation
{
	Reader::Reader(_In_ AVFormatContext* formatContext, _In_ const map<int, SampleProvider*>& streamMap) :
		m_formatContext(formatContext),
		m_streamIdMap(streamMap)
	{

	}

	void Reader::ReadPacket()
	{
		AVPacket_ptr packet{ av_packet_alloc() };
		THROW_IF_NULL_ALLOC(packet);

		// Read the next packet and push it into the appropriate sample provider.
		// Drop the packet if the stream is not being used.
		THROW_HR_IF_FFMPEG_FAILED(av_read_frame(m_formatContext, packet.get()));

		FFMPEG_INTEROP_TRACE("Read packet for Stream %d. PTS = %I64d, Duration = %I64d, Pos = %I64d",
			packet->stream_index, packet->pts, packet->duration, packet->pos);

		auto iter = m_streamIdMap.find(packet->stream_index);
		if (iter != m_streamIdMap.end())
		{
			iter->second->QueuePacket(move(packet));
		}
	}
}
