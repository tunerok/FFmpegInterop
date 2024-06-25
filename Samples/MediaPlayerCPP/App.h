﻿//*****************************************************************************
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

#pragma once

#include "App.xaml.g.h"

namespace winrt::MediaPlayerCPP::implementation
{
	class App : 
		public AppT<App>
	{
	public:
		App();

		void OnLaunched(const Windows::ApplicationModel::Activation::LaunchActivatedEventArgs& args);
		void OnFileActivated(const Windows::ApplicationModel::Activation::FileActivatedEventArgs& args);
		void OnSuspending(const IInspectable& sender, const Windows::ApplicationModel::SuspendingEventArgs& args);
		void OnNavigationFailed(const IInspectable& sender, const Windows::UI::Xaml::Navigation::NavigationFailedEventArgs& args);

		static void Log(const IInspectable& sender, const FFmpegInterop::LogEventArgs& args);

	private:
		void InitRootFrame();
	};
}
