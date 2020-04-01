#include "CallFrame.h"
extern "C"
size_t STDMETHODCALLTYPE nativecall(uint64_t p1,uint64_t p2,uint64_t p3,uint64_t p4, FARPROC proc);

CallFrame::CallFrame() {
	auto res = nativecall(0, (uint64_t) TEXT("Editor"), (uint64_t)TEXT("Editor"), MB_OK, (FARPROC)MessageBoxW);
}
CallFrame::~CallFrame(void) {
}

HRESULT STDMETHODCALLTYPE CallFrame::pushByte(VARIANT value) {
	return write<uint8_t>(value);
}

HRESULT STDMETHODCALLTYPE CallFrame::pushWORD(
	VARIANT value) {
	return write<uint16_t>(value);
}

HRESULT STDMETHODCALLTYPE CallFrame::pushDWORD(
	VARIANT value) {
	return write<uint32_t>(value);
}

HRESULT STDMETHODCALLTYPE CallFrame::pushLWORD(
	VARIANT value) {
	return write<uint64_t>(value);
}

HRESULT STDMETHODCALLTYPE CallFrame::pushPtr(
	VARIANT value) {
	
	return writePtr<void*>(value);	
}

HRESULT STDMETHODCALLTYPE CallFrame::pushFloat(
	VARIANT value) {
	return write<float_t>(value);
}

HRESULT STDMETHODCALLTYPE CallFrame::pushDouble(
	VARIANT value) {
	return write<double_t>(value);
}