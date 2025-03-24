
/*
 * This file is auto-generated from a NativeModule spec file in js.
 *
 * This is a C++ Spec class that should be used with MakeTurboModuleProvider to register native modules
 * in a way that also verifies at compile time that the native module matches the interface required
 * by the TurboModule JS spec.
 */
#pragma once
// clang-format off

#include <NativeModules.h>
#include <tuple>

namespace RnwFilePicker {


struct RNWFilePickerSpec : winrt::Microsoft::ReactNative::TurboModuleSpec {
  static constexpr auto methods = std::tuple{
      Method<void(std::vector<std::string>, std::string, Promise<::React::JSValue>) noexcept>{0, L"pickFile"},
  };

  template <class TModule>
  static constexpr void ValidateModule() noexcept {
    constexpr auto methodCheckResults = CheckMethods<TModule, RNWFilePickerSpec>();

    REACT_SHOW_METHOD_SPEC_ERRORS(
          0,
          "pickFile",
          "    REACT_METHOD(pickFile) void pickFile(std::vector<std::string> const & types, std::string destPath, ::React::ReactPromise<::React::JSValue> &&result) noexcept { /* implementation */ }\n"
          "    REACT_METHOD(pickFile) static void pickFile(std::vector<std::string> const & types, std::string destPath, ::React::ReactPromise<::React::JSValue> &&result) noexcept { /* implementation */ }\n");
  }
};

} // namespace RnwFilePicker
