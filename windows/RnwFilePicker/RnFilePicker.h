#pragma once

#include "JSValue.h"
#include "NativeModules.h"
#include <codegen/NativeRNWFilePickerSpec.g.h>

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>

using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;

using namespace winrt::Microsoft::ReactNative;

namespace RN = winrt::Microsoft::ReactNative;

namespace winrt::RnFilePicker
{

REACT_MODULE(RnFilePicker, L"RnwFilePicker")
struct RnFilePicker
{
    // See https://microsoft.github.io/react-native-windows/docs/native-modules for details on writing native modules

  REACT_INIT(Initialize)
    void Initialize(ReactContext const& reactContext) noexcept;
    /* {
        m_reactContext = reactContext;
    }*/
    
    REACT_METHOD(pickFile)
    void pickFile(std::vector<std::string> const& types, std::wstring destFolder, RN::ReactPromise<JSValueObject> &&promise) noexcept;
    /* {
      FileOpenPicker picker;
      picker.ViewMode(PickerViewMode::List);
      picker.SuggestedStartLocation(PickerLocationId::DocumentsLibrary);
      picker.FileTypeFilter().Append(L".txt"); // Filter for text files

      // Show the file picker and get the selected file
      StorageFile file = co_await picker.PickSingleFileAsync();

      if (file)
      {
        // Return the file path
        promise.Resolve(file.Path());
      }
      else
      {
        promise.Reject(L"no file");
      }
    }*/

    private:
      IAsyncOperation<hstring> ReadFileAsBase64Async(StorageFile file);
      IAsyncOperation<Windows::Storage::Pickers::FileOpenPicker> CreateFileOpenPicker(std::vector<std::string> const& types);
      IAsyncAction pickInternal(std::vector<std::string> const& types, std::wstring destFolder, RN::ReactPromise<JSValueObject> promise) noexcept;

    private:
        ReactContext m_reactContext{nullptr};
};

} // namespace winrt::RnwFilePicker
