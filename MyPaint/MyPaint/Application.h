#pragma once
#include <UIRibbon.h>
#include "RibbonIDs.h"
#include "resource.h"
//
//  CLASS: CApplication : IUIApplication
//
//  PURPOSE: Implements interface IUIApplication that defines methods
//           required to manage Framework events.
//
//  COMMENTS:
//
//    CApplication implements the IUIApplication interface which is required for any ribbon application.
//    IUIApplication contains callbacks made by the ribbon framework to the application
//    during various updates like command creation/destruction and view state changes.
//
class CApplication
	: public IUIApplication // Applications must implement IUIApplication.
{
public:

	// Static method to create an instance of the object.
	static HRESULT CreateInstance(CApplication **ppApplication);

	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID iid, void** ppv);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();

	// IUIApplication methods
	STDMETHOD(OnCreateUICommand)(UINT nCmdID,
		UI_COMMANDTYPE typeID,
		IUICommandHandler** ppCommandHandler);

	STDMETHOD(OnViewChanged)(UINT viewId,
		UI_VIEWTYPE typeId,
		IUnknown* pView,
		UI_VIEWVERB verb,
		INT uReasonCode);

	STDMETHOD(OnDestroyUICommand)(UINT32 commandId,
		UI_COMMANDTYPE typeID,
		IUICommandHandler* commandHandler);

	int GetCurrentShape();

	void SetCurrentShape(int newShape);

	int GetCurrentColorStyle();

	void SetCurrentColorStyle(int);

private:
	CApplication()
		: m_cRef(1)
		, iShape(ID_CMD_LINE)
		, isTransparent(ID_CMD_TRANSPARENT)
		, m_pCommandHandler(NULL)
	{
	}

	~CApplication()
	{
		if (m_pCommandHandler)
		{
			m_pCommandHandler->Release();
			m_pCommandHandler = NULL;
		}
	}

	LONG m_cRef;                            // Reference count.
	int iShape;
	int isTransparent;
	IUICommandHandler * m_pCommandHandler;  // Generic Command Handler
};
