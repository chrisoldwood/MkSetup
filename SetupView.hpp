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
	// Members.
	//
	CListView	m_lvFiles;

protected:
	// Child window ID.
	enum { IDC_FILES = 100 };

	//
	// Message processors (Overriden from the base class).
	//
	virtual void OnCreate(const CRect& rcClient);
	virtual void OnResize(int iFlag, const CSize& rNewSize);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // SETUPVIEW_HPP
