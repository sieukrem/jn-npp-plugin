#include "CallFrame.h"


CallFrame::CallFrame() {
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
	
#ifdef _M_X64
	return write<uint64_t>(value);
#else
	return write<uint32_t>(value);
#endif
	
}