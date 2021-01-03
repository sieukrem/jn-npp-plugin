#include "CallFrame.h"


#ifdef _M_X64
extern "C" size_t STDMETHODCALLTYPE nativecall(uint8_t* r, FARPROC proc, uint64_t stackSize);
#else
size_t STDMETHODCALLTYPE nativecall(uint8_t* r, FARPROC proc, size_t stackSize){
	
	/*
		copy params to stack
	*/

	char* buf;		// pointer to the buffer on stack
	void* v_esp;	// old stack pointer
	DWORD v_res;

	// reserve memory on stack
	__asm {
		mov v_esp, esp
		mov eax, stackSize
		sub esp, eax
		mov buf, esp
	}

	memcpy(buf, r, stackSize);

	/*
		make a call to the function
	*/

    __asm {
        call    proc
		mov v_res, eax
        mov eax, stackSize
        mov esp, v_esp
    }
 
	return v_res;

}
#endif

CallFrame::CallFrame(){
	m_Buffer.reserve(4 * sizeof(size_t));
}

CallFrame::~CallFrame(void) {
}

size_t CallFrame::Call(FARPROC fn)
{
	auto result = nativecall(
		m_Buffer.begin()._Ptr, 
		fn, 
		m_Buffer.size()
	);

	m_Buffer.resize(0);

	return result;
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
	
	return writePtr<size_t>(value);	
}

HRESULT STDMETHODCALLTYPE CallFrame::pushFloat(
	VARIANT value) {
	return write<float_t>(value);
}

HRESULT STDMETHODCALLTYPE CallFrame::pushDouble(
	VARIANT value) {
	return write<double_t>(value);
}