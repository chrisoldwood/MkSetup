/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SETUPVIEW.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CSetupView class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CSetupView::CSetupView(CSetupDoc& rDoc)
	: CView(rDoc)
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

CSetupView::~CSetupView()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Create the files listview.
**
** Parameters:	rcClient	The client rectangle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSetupView::OnCreate(const CRect& /*rcClient*/)
{
	m_lvFiles.Create(*this, IDC_FILES, ClientRect());

	m_lvFiles.InsertColumn(0, "File",       100);
	m_lvFiles.InsertColumn(1, "Shortcut?",   75);
	m_lvFiles.InsertColumn(2, "Desk Icon?",  75);

	m_lvFiles.FullRowSelect();
}

/******************************************************************************
** Method:		OnResize()
**
** Description:	Resize the child window.
**
** Parameters:	iFlag		The resize method.
**				rNewSize	The new window dimensions.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSetupView::OnResize(int /*iFlag*/, const CSize& /*rNewSize*/)
{
	m_lvFiles.Move(ClientRect());
}
