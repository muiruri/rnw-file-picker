#include "pch.h"
//#include "microsoft.ui.xaml.window.h"
#include <winrt/Windows.Foundation.Collections.h> 
#include "RnFilePicker.h"
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Storage.FileProperties.h>
#include <winrt/Windows.Security.Cryptography.h>
#include <windows.h>
#include <unknwn.h>
#include <iostream>
#include <string>

struct __declspec(uuid("3E68D4BD-7135-4D10-8018-9FB6D9F33FA1")) IInitializeWithWindow : ::IUnknown
{
  virtual HRESULT __stdcall Initialize(HWND hwnd) = 0;
};

namespace winrt::RnFilePicker {
  using namespace React;
  using namespace xaml;
  using namespace Windows::Foundation;
  using namespace Windows::Storage;
  using namespace Windows::Storage::Pickers;
  using namespace Windows::Storage::Streams;
  using namespace Windows::Storage::FileProperties;
  using namespace Windows::Security::Cryptography;
  

  void RnFilePicker::Initialize(ReactContext const& aReactContext) noexcept
  {
    m_reactContext = aReactContext;
  }

  IAsyncOperation<hstring> RnFilePicker::ReadFileAsBase64Async(StorageFile file) {
    if (file) { // Open the file for reading
      IRandomAccessStreamWithContentType stream = co_await file.OpenReadAsync();
      // Read the file content into a buffer
      DataReader reader(stream);
      co_await reader.LoadAsync(stream.Size());
      IBuffer buffer = reader.ReadBuffer(stream.Size());
      // Convert the buffer to a base64-encoded string
      hstring base64String = CryptographicBuffer::EncodeToBase64String(buffer);
      co_return base64String;
    } else {
      co_return L"";
    }
  }

  IAsyncOperation<FileOpenPicker> RnFilePicker::CreateFileOpenPicker(std::vector<std::string> const& types)
  {
    FileOpenPicker picker;

    const auto hwnd = ReactCoreInjection::GetTopLevelWindowId(m_reactContext.Properties().Handle());
    //auto initializeWithWindow = picker.as<::IInitializeWithWindow>();
    // winrt::check_hresult(initializeWithWindow->Initialize(hwnd));
    picker.as<::IInitializeWithWindow>()->Initialize(reinterpret_cast<HWND>(hwnd));

    for (const auto& mimeType : types)
    {
      const auto mimeTypeStr(to_hstring(mimeType));
      picker.FileTypeFilter().Append(mimeTypeStr);
    }
    // picker.FileTypeFilter().Append(L".txt");
    picker.SuggestedStartLocation(PickerLocationId::Desktop);
    picker.ViewMode(PickerViewMode::List);

    co_return picker;
  }

  IAsyncAction RnFilePicker::pickInternal(std::vector<std::string> const& types, std::wstring destFolder, ReactPromise<JSValueObject> promise) noexcept
  {
    auto picker(co_await CreateFileOpenPicker(types));

    StorageFile file = co_await picker.PickSingleFileAsync();
    JSValueObject response;
    if (file)
    {
      // hstring base64Content = co_await ReadFileAsBase64Async(file);
      // std::wcout << L"Output " << base64Content.c_str() << std::endl;
      BasicProperties props = co_await file.GetBasicPropertiesAsync();
      
      response["uri"] = to_string(file.Path());
      response["Name"] = to_string(file.Name());
      const uint64_t size = props.Size();
      response["size"] = size;
      response["mimeType"] = to_string(file.ContentType());
      // response["base64"] = to_string(base64Content);
      destFolder = ApplicationData::Current().LocalFolder().Path();
      if (!destFolder.empty()) {
        StorageFolder copyToFolder{ co_await StorageFolder::GetFolderFromPathAsync(destFolder) };
        co_await file.CopyAsync(copyToFolder, file.Name(), NameCollisionOption::ReplaceExisting);
        response["destPath"] = to_string(destFolder);
      }
      // Return the file path
      promise.Resolve(response);
    }
    else
    {
      promise.Reject("{error: null}");
    }
  }

  void RnFilePicker::pickFile(std::vector<std::string> const& types, std::wstring destFolder, ReactPromise<JSValueObject> &&promise) noexcept {
    m_reactContext.UIDispatcher().Post([this, types, destFolder, promise]() mutable {
      pickInternal(types, destFolder, promise).Completed();
    });
  }
}
