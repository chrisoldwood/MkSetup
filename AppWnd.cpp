/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPWND.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppWnd class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppWnd.hpp"
#include "MkSetupApp.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppWnd::CAppWnd(WCL::IMsgThread& thread, WCL::ICmdController& controller)
	: CSDIFrame(IDR_APPICON)
	, m_ToolBar(thread, controller)
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppWnd::~CAppWnd()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Creates the toolbar and status bars.
**
** Parameters:	rcClient	The client rectangle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnCreate(const CRect& rcClient)
{
	//
	// Create and attach the components.
	//
	m_Accel.LoadRsc(IDR_APPACCEL);
	Accel(&m_Accel);

	m_Menu.LoadRsc(IDR_APPMENU);
	Menu(&m_Menu);

	m_ToolBar.Create(*this, IDC_TOOL_BAR, rcClient);
	ToolBar(&m_ToolBar);

	m_StatusBar.Create(*this, IDC_STATUS_BAR, rcClient);
	StatusBar(&m_StatusBar);

	// Call base class.
	CSDIFrame::OnCreate(rcClient);
}

/******************************************************************************
** Method:		OnQueryClose()
**
** Description:	Check if the app can close.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppWnd::OnQueryClose()
{
	// Save windows final position.
	App.m_rcAppWnd = Placement();

	// Close the app if the file was closed.
	return App.m_AppCmds.CloseFile();
}

/******************************************************************************
** Method:		OnActivate()
**
** Description:	Set the focus to the view, if it exists.
**
** Parameters:	bActivating		Being activated?
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnActivate(bool bActivating)
{
	if ( (bActivating) && (m_pView != nullptr) )
		m_pView->Focus();
}
