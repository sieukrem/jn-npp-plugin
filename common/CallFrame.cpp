#include "CallFrame.h"


#ifdef _M_X64
extern "C" size_t STDMETHODCALLTYPE nativecall(uint64_t r, FARPROC proc, void* stack, uint64_t stackSize);
#endif

CallFrame::CallFrame():m_DoubleFloatRegister(&m_Registers[0]), m_IntegerRegister(&m_Registers[4]){
}
CallFrame::~CallFrame(void) {
}

size_t CallFrame::Call(FARPROC fn)
{
	if (m_Buffer.size() % sizeof(size_t) > 0)
		m_Buffer.resize(m_Buffer.size() + (sizeof(size_t) - m_Buffer.size() % sizeof(size_t))); // ensure alignment

	auto result = nativecall(
		(uint64_t)m_Registers, 
		fn, 
		m_Buffer.size() == 0? NULL: &m_Buffer[0],
		m_Buffer.size()
	);

	m_Position = 0;
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