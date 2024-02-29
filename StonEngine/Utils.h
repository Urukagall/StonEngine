#include <stdexcept>
#include <string>
#include <comdef.h>
#include <locale>
#include <codecvt>

inline void ThrowIfFailed(HRESULT hr, const wchar_t* file, int line)
{
    if (FAILED(hr))
    {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();

        std::wstring errorMessage = std::to_wstring(line) + L" in " + file + L"\nError: " + errMsg;
        throw std::runtime_error(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(errorMessage.c_str()));
    }
}

#define ThrowIfFailed(hr) ThrowIfFailed(hr, __FILEW__, __LINE__)