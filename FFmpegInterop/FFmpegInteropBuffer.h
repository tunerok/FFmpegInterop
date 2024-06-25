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

namespace winrt::FFmpegInterop::implementation
{
	class FFmpegInteropBuffer :
		public implements<
			FFmpegInteropBuffer,
			Windows::Storage::Streams::IBuffer,
			::Windows::Storage::Streams::IBufferByteAccess,
			IMarshal>
	{
	public:
		FFmpegInteropBuffer(_In_ AVBufferRef* bufRef);
		FFmpegInteropBuffer(_In_ AVBufferRef_ptr bufRef);
		FFmpegInteropBuffer(_In_ AVPacket_ptr packet) noexcept;
		FFmpegInteropBuffer(_In_ AVBlob_ptr buf, _In_ uint32_t bufSize) noexcept;
		FFmpegInteropBuffer(_In_ std::vector<uint8_t>&& buf) noexcept;

		// IBuffer
		IFACEMETHOD_(uint32_t, Capacity)() const noexcept;
		IFACEMETHOD_(uint32_t, Length)() const noexcept;
		void Length(_In_ uint32_t length);

		// IBufferByteAccess
		IFACEMETHOD(Buffer)(_Outptr_ uint8_t** buf) noexcept;

		// IMarshal
		IFACEMETHOD(GetUnmarshalClass)(
			_In_ REFIID riid,
			_In_opt_ void* pv,
			_In_ DWORD dwDestContext,
			_Reserved_ void* pvDestContext,
			_In_ DWORD mshlflags,
			_Out_ CLSID* pclsid) noexcept;
		IFACEMETHOD(GetMarshalSizeMax)(
			_In_ REFIID riid,
			_In_opt_ void* pv,
			_In_ DWORD dwDestContext,
			_Reserved_ void* pvDestContext,
			_In_ DWORD mshlflags,
			_Out_ DWORD* pcbSize) noexcept;
		IFACEMETHOD(MarshalInterface)(
			_In_ IStream* pStm,
			_In_ REFIID riid,
			_In_opt_ void* pv,
			_In_ DWORD dwDestContext,
			_Reserved_ void* pvDestContext,
			_In_ DWORD mshlflags) noexcept;
		IFACEMETHOD(UnmarshalInterface)(_In_ IStream* pStm, _In_ REFIID riid, _Outptr_ void** ppv) noexcept;
		IFACEMETHOD(ReleaseMarshalData)(_In_ IStream* pStm) noexcept;
		IFACEMETHOD(DisconnectObject)(_Reserved_ DWORD dwReserved) noexcept;

	private:
		void InitMarshaler();

		std::unique_ptr<uint8_t, std::move_only_function<void(uint8_t*)>> m_buf;
		uint32_t m_length{ 0 };
		com_ptr<IMarshal> m_marshaler;
		std::once_flag m_marshalerInitFlag;
	};
}
