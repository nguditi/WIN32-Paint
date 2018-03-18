// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include <UIRibbon.h>
#include "CommandHandler.h"
#include "RibbonIDs.h"
#include "RibbonFramework.h"
#include "Propvarutil.h"
#pragma comment(lib,"Propsys.lib")
#include "stdlib.h"
#include <UIRibbonPropertyHelpers.h>
#include "PropertySet.h"

int CCommandHandler::LineIds[4] =
{
	IDB_LINE1, IDB_LINE3, IDB_LINE5, IDB_LINE8
};
int CCommandHandler::StyleIds[4] =
{
	IDB_SOLID, IDB_DOT, IDB_DASH, IDB_DASHDOT
};

// Static method to create an instance of the object.
HRESULT CCommandHandler::CreateInstance(IUICommandHandler **ppCommandHandler)
{
	if (!ppCommandHandler)
	{
		return E_POINTER;
	}

	*ppCommandHandler = NULL;

	HRESULT hr = S_OK;

	CCommandHandler* pCommandHandler = new CCommandHandler();

	if (pCommandHandler != NULL)
	{
		*ppCommandHandler = static_cast<IUICommandHandler *>(pCommandHandler);
	}
	else
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}

// IUnknown method implementations.
STDMETHODIMP_(ULONG) CCommandHandler::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CCommandHandler::Release()
{
	LONG cRef = InterlockedDecrement(&m_cRef);
	if (cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP CCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(IUICommandHandler))
	{
		*ppv = static_cast<IUICommandHandler*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}


//
//  FUNCTION: UpdateProperty()
//
//  PURPOSE: Called by the Ribbon framework when a command property (PKEY) needs to be updated.
//
//  COMMENTS:
//
//    This function is used to provide new command property values, such as labels, icons, or
//    tooltip information, when requested by the Ribbon framework.  
//    
//    In this SimpleRibbon sample, the method is not implemented.  
//
STDMETHODIMP CCommandHandler::UpdateProperty(
	UINT nCmdID,
	REFPROPERTYKEY key,
	const PROPVARIANT* ppropvarCurrentValue,
	PROPVARIANT* ppropvarNewValue)
{
	HRESULT hr = E_FAIL;
	if (UI_PKEY_Categories == key)
	{
		hr = S_FALSE;
	}
	else if (key == UI_PKEY_BooleanValue)
	{
		if (nCmdID >= ID_CMD_LINE && nCmdID <= ID_CMD_Elipse)
		{
			hr = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, g_pApplication->GetCurrentShape() == nCmdID, ppropvarNewValue);
		}
		else if (nCmdID == ID_CMD_COLORSOLID || nCmdID == ID_CMD_TRANSPARENT)
		{
			hr = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, g_pApplication->GetCurrentColorStyle() == nCmdID, ppropvarNewValue);
		}
		else if (nCmdID == IDC_CMD_CONTEXTMAP1)
		{
			hr = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, IDC_CMD_CONTEXTMAP1 == IDC_CMD_CONTEXTMAP1, ppropvarNewValue);
		}
	}
	else if (UI_PKEY_ItemsSource == key)
	{
		IUICollection* pCollection;
		hr = ppropvarCurrentValue->punkVal->QueryInterface(IID_PPV_ARGS(&pCollection));
		for (int i = 0; i < 4; i++)
		{
			CPropertySet* pItem;
			hr = CPropertySet::CreateInstance(&pItem);
			if (FAILED(hr))
			{
				pCollection->Release();
				return hr;
			}
			// Create an IUIImage from a resource id.
			IUIImage* pImg;
			if (nCmdID == ID_CMD_SIZE)
				hr = CreateUIImageFromBitmapResource(MAKEINTRESOURCE(LineIds[i]), &pImg);
			else
				hr = CreateUIImageFromBitmapResource(MAKEINTRESOURCE(StyleIds[i]), &pImg);

			if (FAILED(hr))
			{
				pCollection->Release();
				pItem->Release();
				return hr;
			}
			pItem->InitializeItemProperties(pImg, L"", UI_COLLECTION_INVALIDINDEX);
			pCollection->Add(pItem);

			pItem->Release();
			pImg->Release();
		}
		pCollection->Release();
		hr = S_OK;
	}
	else if (UI_PKEY_SelectedItem == key)
	{
		if (nCmdID == ID_CMD_SIZE)
			hr = ::UIInitPropertyFromUInt32(UI_PKEY_SelectedItem, 1, ppropvarNewValue);
		else
			hr = ::UIInitPropertyFromUInt32(UI_PKEY_SelectedItem, 0, ppropvarNewValue);
	}
	return hr;
}

//
//  FUNCTION: Execute()
//
//  PURPOSE: Called by the Ribbon framework when a command is executed by the user.  For example, when
//           a button is pressed.
//
STDMETHODIMP CCommandHandler::Execute(
	UINT nCmdID,
	UI_EXECUTIONVERB verb,
	const PROPERTYKEY* key,
	const PROPVARIANT* ppropvarValue,
	IUISimplePropertySet* pCommandExecutionProperties)
{
	HWND hWnd = GetForegroundWindow();
	HRESULT hr = S_OK;
	if (key != NULL && *key == UI_PKEY_BooleanValue)
	{
		if (nCmdID >= ID_CMD_LINE && nCmdID <= ID_CMD_Elipse)
		{
			g_pApplication->SetCurrentShape(nCmdID);
		}
		else if (nCmdID == ID_CMD_COLORSOLID || nCmdID == ID_CMD_TRANSPARENT)
		{
			g_pApplication->SetCurrentColorStyle(nCmdID);
		}
		hr = g_pFramework->InvalidateUICommand(UI_ALL_COMMANDS, UI_INVALIDATIONS_VALUE, NULL);
	}

	SendMessage(hWnd, WM_COMMAND, nCmdID, 0);

	if (UI_EXECUTIONVERB_EXECUTE == verb)
	{
		if (nullptr != key && UI_PKEY_SelectedItem == (*key))
		{
			// Update the size
			unsigned int selectedIndex;
			hr = ::UIPropertyToUInt32(*key, *ppropvarValue, &selectedIndex);
			if (SUCCEEDED(hr))
			{
				if (nCmdID == ID_CMD_SIZE)
				{
					SendMessage(hWnd, WM_COMMAND, IDB_LINE1 + selectedIndex, 0);
					return hr;
				}
				if (nCmdID == ID_CMD_STYLE)
				{
					SendMessage(hWnd, WM_COMMAND, IDB_SOLID + selectedIndex, 0);
					return hr;
				}
			}

		}
	}
	return hr;
}

// Factory method to create IUIImages from resource identifiers.
HRESULT CCommandHandler::CreateUIImageFromBitmapResource(LPCTSTR pszResource, __out IUIImage **ppimg)
{
	HRESULT hr = E_FAIL;
	*ppimg = NULL;
	if (NULL == m_pifbFactory)
	{
		hr = CoCreateInstance(CLSID_UIRibbonImageFromBitmapFactory,NULL,CLSCTX_ALL,IID_PPV_ARGS(&this->m_pifbFactory));
		if (FAILED(hr))
		{
			return hr;
		}
	}
	HBITMAP hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL),pszResource,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
	if (hbm)
	{
		// Use the factory implemented by the framework to produce an IUIImage.
		hr = this->m_pifbFactory->CreateImage(hbm, UI_OWNERSHIP_TRANSFER, ppimg);
		if (FAILED(hr))
		{
			DeleteObject(hbm);
		}
	}
	return hr;
}
