#include "CallBack.h"
#include "MyActiveSite.h"

CallBack::CallBack(unsigned int stacksize, IDispatchEx* cfg, unsigned int num):CComDispatch(){
	m_Stack = stacksize;
	m_Cfg = cfg;
	cfg->AddRef();
	m_Num = num;
	*CallBack::GetCallBack(num) = this;
}

DWORD CallBack::Call(void* ebp){
	void* params = ((DWORD*)ebp) + 2; // 1 for the return adress

	VARIANT result;

	VARIANT var[1];
	var[0].vt = VT_BSTR;
	var[0].bstrVal = SysAllocStringByteLen(((LPCSTR)params),m_Stack); 

	MyActiveSite::callMethod(TEXT("cmd"), m_Cfg, var, 1, &result);

	SysFreeString(var[0].bstrVal);

	VARTYPE type = VT_UI4;
	HRESULT hr = VariantChangeType(&result, &result, 0, type);
	
	DWORD dwResult = 0;
	dwResult = result.uintVal;

	return dwResult;
}

CallBack::~CallBack(){
	m_Cfg->Release();
	*CallBack::GetCallBack(m_Num) = NULL;
}

CallBack::CBP* CallBack::Proc(){
	// say compiler it should create 12 function stubs
	static CallBack::CBP* cbps[]={
		&CallBack::CallBackProc<0>,&CallBack::CallBackProc<1>,
		&CallBack::CallBackProc<2>,&CallBack::CallBackProc<3>,
		&CallBack::CallBackProc<4>,&CallBack::CallBackProc<5>,
		&CallBack::CallBackProc<6>,&CallBack::CallBackProc<7>,
		&CallBack::CallBackProc<8>,&CallBack::CallBackProc<9>,
		&CallBack::CallBackProc<10>,&CallBack::CallBackProc<11>,
	};

	return cbps[m_Num];
}
CallBack** CallBack::GetCallBack(unsigned int index){
	static CallBack* callBacks[12]={0};
	return &callBacks[index];
}


CallBack* CallBack::RegisterCallBack( IDispatchEx *cfg, unsigned int stacksize){
	for(int i=0; i<12; i++){
		if (*CallBack::GetCallBack(i)!=NULL)
			continue;

		return new CallBack(stacksize, cfg, i);
	}

	return NULL;
}
