#include "CallFrame.h"



CallFrame::CallFrame(){
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
	
	return writePtr<uint64_t>(value);	
}

HRESULT STDMETHODCALLTYPE CallFrame::pushFloat(
	VARIANT value) {
	return write<float_t>(value);
}

HRESULT STDMETHODCALLTYPE CallFrame::pushDouble(
	VARIANT value) {
	return write<double_t>(value);
}