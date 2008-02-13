/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SETUPVIEW.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CSetupView class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SETUPVIEW_HPP
#define SETUPVIEW_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/View.hpp>
#include <WCL/ListView.hpp>
#include "SetupDoc.hpp"

// Forward declarations.
class CFileProps;

/******************************************************************************
** 
** The documents' view class.
**
*******************************************************************************
*/

class CSetupView : public CView
{
public:
	//
	// Constructors/Destructor.
	//
	CSetupView(CSetupDoc& rDoc);
	~CSetupView();

	//
	// Methods.
	//
	bool        IsFileSelected() const;
	CFileProps* GetSelection() const;

	void RefreshFileList();
	void RefreshFile(const CFileProps* pFileProps);

private:	
	//
	// Members.
	//
	CListView	m_lvFiles;

	// Columns.
	enum
	{
		FILE_NAME,
		FILE_DIR,
		PROG_ICON,
		DESK_ICON,
		ICON_NAME,
		ICON_DESC,
	};

	// Child window ID.
	enum { IDC_FILES = 100 };

	//
	// Message processors (Overriden from the base class).
	//
	virtual void OnCreate(const CRect& rcClient);
	virtual void OnResize(int iFlag, const CSize& rNewSize);
	virtual void OnFocus();
	LRESULT OnListSelChange(NMHDR& oHdr);
	LRESULT OnListDoubleClick(NMHDR& oHdr);

	//
	// Internal methods.
	//
	CSetupDoc& Doc();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CSetupDoc& CSetupView::Doc()
{
	return static_cast<CSetupDoc&>(m_Doc);
}

inline bool CSetupView::IsFileSelected() const
{
	return m_lvFiles.IsSelection();
}

inline CFileProps* CSetupView::GetSelection() const
{
	ASSERT(IsFileSelected());

	return static_cast<CFileProps*>(m_lvFiles.ItemPtr(m_lvFiles.Selection()));
}

#endif // SETUPVIEW_HPP
