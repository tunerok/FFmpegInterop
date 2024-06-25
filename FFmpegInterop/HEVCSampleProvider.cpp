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
#include "HEVCSampleProvider.h"

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Media::MediaProperties;
using namespace winrt::Windows::Storage::Streams;
using namespace std;

namespace winrt::FFmpegInterop::implementation
{
	HEVCSampleProvider::HEVCSampleProvider(_In_ const AVFormatContext* formatContext, _In_ AVStream* stream, _In_ Reader& reader) :
		NALUSampleProvider(formatContext, stream, reader)
	{
		// Parse codec private data if present
		if (m_stream->codecpar->extradata != nullptr && m_stream->codecpar->extradata_size > 0)
		{
			// Check the HEVC bitstream flavor
			if (m_stream->codecpar->extradata_size > 3 && (m_stream->codecpar->extradata[0] || m_stream->codecpar->extradata[1] || m_stream->codecpar->extradata[2] > 1))
			{
				// hvcC config format
				FFMPEG_INTEROP_TRACE("Stream %d: HEVC codec private data", m_stream->index);

				m_isBitstreamAnnexB = false;

				HEVCConfigParser parser{ m_stream->codecpar->extradata, static_cast<uint32_t>(m_stream->codecpar->extradata_size) };
				m_naluLengthSize = parser.GetNaluLengthSize();
				tie(m_codecPrivateNaluData, m_codecPrivateNaluLengths) = parser.GetNaluData();
			}
			else
			{
				// Annex B format
				FFMPEG_INTEROP_TRACE("Stream %d: Annex B codec private data", m_stream->index);

				AnnexBParser parser{ m_stream->codecpar->extradata, static_cast<uint32_t>(m_stream->codecpar->extradata_size) };
				tie(m_codecPrivateNaluData, m_codecPrivateNaluLengths) = parser.GetNaluData();
			}
		}
	}

	HEVCConfigParser::HEVCConfigParser(_In_reads_(dataSize) const uint8_t* data, _In_ uint32_t dataSize) :
		m_data(data),
		m_dataSize(dataSize)
	{
		// Validate parameters
		WINRT_ASSERT(m_data != nullptr);
		THROW_HR_IF(MF_E_INVALID_FILE_FORMAT, m_dataSize < MIN_SIZE);
	}

	uint8_t HEVCConfigParser::GetNaluLengthSize() const noexcept
	{
		return m_data[21] & 0x03 + 1;
	}

	tuple<vector<uint8_t>, vector<uint32_t>> HEVCConfigParser::GetNaluData() const
	{
		vector<uint8_t> naluData;
		vector<uint32_t> naluLengths;

		// Scan the parameter sets
		uint32_t pos{ 22 };
		const uint8_t parameterSetCount{ m_data[pos++] };
		for (uint8_t i{ 0 }; i < parameterSetCount; i++)
		{
			// Get the NALU type and count
			THROW_HR_IF(MF_E_INVALID_FORMAT, pos + sizeof(uint8_t) + sizeof(uint16_t) >= m_dataSize);
			const uint8_t naluType{ static_cast<uint8_t>(m_data[pos++] & 0x3F) };
			const uint16_t naluCount{ _byteswap_ushort(*reinterpret_cast<const uint16_t*>(m_data + pos)) };
			pos += sizeof(uint16_t);

			// Save the NALU data if it's VPS, SPS, or PPS
			const bool copyNaluData{ naluType == NALU_TYPE_HEVC_VPS || naluType == NALU_TYPE_HEVC_SPS || naluType == NALU_TYPE_HEVC_PPS };
			if (copyNaluData)
			{
				// Reserve estimated space now to minimize reallocations
				naluLengths.reserve(naluLengths.size() + naluCount);
			}

			// Scan the NALUs
			for (uint16_t j{ 0 }; j < naluCount; j++)
			{
				// Get the NALU length
				uint32_t naluLength{ GetAVCNaluLength(m_data + pos, m_dataSize - pos, sizeof(uint16_t)) };
				pos += sizeof(uint16_t);

				if (copyNaluData)
				{
					// Write the NALU start code and VPS/SPS/PPS data to the buffer
					naluData.insert(naluData.end(), begin(NALU_START_CODE), end(NALU_START_CODE));
					naluData.insert(naluData.end(), m_data + pos, m_data + pos + naluLength);

					naluLengths.push_back(sizeof(NALU_START_CODE) + naluLength);
				}

				pos += naluLength;
			}
		}

		return { naluData, naluLengths };
	}
}
