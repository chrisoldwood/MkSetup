/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPTOOLBAR.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppToolBar class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

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

CAppToolBar::CAppToolBar()
{
	// Define the toolbar.
	DEFINE_TOOLBAR
		TBCTL(&m_NewBtn,	ID_FILE_NEW)
		TBCTL(&m_OpenBtn,	ID_FILE_OPEN)
		TBCTL(&m_SaveBtn,	ID_FILE_SAVE)
		TBGAP()
		TBCTL(&m_btnHelp,	ID_HELP_ABOUT)
	END_TOOLBAR
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

CAppToolBar::~CAppToolBar()
{
}
