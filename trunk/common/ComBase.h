/*
This file is part of jN, a plugin for Notepad++
Copyright (C)2013 Eugen Kremer <eugen DOT kremer AT gmail DOT com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include <windows.h>
#include <OAIDl.h>

#include "Statics.h"

#pragma warning(disable : 4250)


template<typename T, bool Increment=false>
class LocRef{
public:
	T* m_Reference;
	LocRef(T* val){
		m_Reference = val;
		if(Increment && m_Reference)
			m_Reference->AddRef();
	}
	LocRef():m_Reference(NULL){
	
	}
	~LocRef(){
		if (m_Reference)
			m_Reference->Release();
	}

	operator void**(){
		// release existing reference
		if(m_Reference){
			m_Reference->Release();
			m_Reference = NULL;
		}

		return &(void*)m_Reference;
	}

	operator bool(){
		return m_Reference != NULL;
	}

	operator T*(){
		return m_Reference; 
	}
	T* operator-> () { return m_Reference; }
	T** operator& () { return &m_Reference; }
};
// class for single threaded Objects that uses c++ increment and decrement operators for reference 
// count purposes ..
// 
class CSingleThreaded
{
public:
	static STDMETHODIMP_(ULONG) Increment(long &reflong)  
	{
		reflong ++;
		return reflong; 
	}

	static STDMETHODIMP_(ULONG) Decrement(long &reflong)
	{
		reflong --; 
		return reflong; 
	}
};

// class for multi threaded Objects that uses Win32 API for reference 
// count purposes ..
// 
class CMultiThreaded
{
public:
	static STDMETHODIMP_(ULONG) Increment(long &reflong) 
	{ 
		::InterlockedIncrement(&reflong); 
		return reflong; 
	}

	static STDMETHODIMP_(ULONG) Decrement(long &reflong) 
	{
		::InterlockedDecrement(&reflong); 
		return reflong; 
	}
};

template <class Interface>
class QI1 : public Interface{
public:
	typedef Interface InterfaceType; 
protected:
	virtual HRESULT Query(REFIID riid, LPVOID *ppv){
		*ppv = NULL;

		if(IsEqualIID(riid, IID_IUnknown)){
			*ppv = static_cast<IUnknown*>(this);		
		}else if (IsEqualIID(riid, __uuidof(Interface))){
			*ppv = static_cast<Interface*>(this);
		}else 
			return E_NOINTERFACE;
 
		return S_OK;
	}
};

class Empty{

};

template <class I1, class I2> class QI2: public  I1, public  I2{
protected:
	HRESULT Query(REFIID riid, LPVOID *ppv){
		*ppv = NULL;
		
		if (I2::Query(riid, ppv)== S_OK){
		}else if ( I1::Query(riid, ppv)== S_OK){
		}else
			return E_NOINTERFACE;

		return S_OK;
	}
};



// this mandatory class manages the life of the COM object it uses a perticular threading model
// for reference count management..
//
template <class I, class ThreadModel = CSingleThreaded, class CInterface = QI1<I>>
class CComBase  :  public CInterface
{
protected:
	long	m_cRef;

public:
	CComBase() {
		m_cRef = 1;
	};

	virtual ~CComBase(){	
	}
	virtual HRESULT __stdcall QueryInterface(REFIID riid, LPVOID *ppv){
		*ppv = NULL;
		
		if (CInterface::Query(riid, ppv) == S_OK){

		}else
			return E_NOINTERFACE;

		
		AddRef();

		return S_OK;
	} 

	virtual STDMETHODIMP_(ULONG) AddRef() 
	{
		return ThreadModel::Increment(m_cRef); 
	}
	virtual STDMETHODIMP_(ULONG) Release() 
	{
		long Value = ThreadModel::Decrement(m_cRef); 
		if(!m_cRef)
			delete this;

		return Value;
	}
};


template <class DispInterface, class CImplementInt = QI1<DispInterface>>
class CComDispatch  :  public CComBase<DispInterface, CMultiThreaded, CImplementInt>
{
protected:
	LocRef<ITypeLib,false> m_TypeLib;
public: 
	CComDispatch(): m_TypeLib(Statics::instance().GetTypeLib()){ 
	}
	virtual ~CComDispatch(){
	}
	
	HRESULT __stdcall QueryInterface(REFIID riid, LPVOID *ppv){

		*ppv = NULL;

		if (CImplementInt::Query(riid, ppv)== S_OK){
		} else if (IsEqualIID(riid,IID_IDispatch))
			*ppv = static_cast<IDispatch*>(this);
		else
			return E_NOINTERFACE;
		
		AddRef();

		return S_OK;
	}

	// IDispatch standard

	
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
		/* [out] */ __RPC__out UINT *pctinfo){
		return E_NOTIMPL;
	}
    
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
		/* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo){
		return E_NOTIMPL;
	}
    
    virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
        /* [in] */ __RPC__in REFIID riid,
        /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
        /* [range][in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId){

			LocRef<ITypeInfo> typeInfo;
			m_TypeLib->GetTypeInfoOfGuid(__uuidof(DispInterface), &typeInfo);

			HRESULT res = DispGetIDsOfNames(typeInfo, rgszNames, cNames, rgDispId);
			return res;
	}
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr){

		LocRef<ITypeInfo> typeInfo;
		m_TypeLib->GetTypeInfoOfGuid(__uuidof(DispInterface), &typeInfo);

		HRESULT res = DispInvoke(
			static_cast<IDispatch*>(this),//this,
			typeInfo,
			dispIdMember, 
			wFlags, 
			pDispParams,
			pVarResult,
			pExcepInfo, 
			puArgErr); 

		return res;
	}
	
	template<class T>
	T* As(){
		return static_cast<T*>(this);
	}
};

