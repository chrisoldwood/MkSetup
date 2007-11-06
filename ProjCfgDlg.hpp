/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROJCFGDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CProjCfgDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PROJCFGDLG_HPP
#define PROJCFGDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

// Forward declarations.
class CSetupDoc;

/******************************************************************************
** 
** The dialog used to edit the main project settings.
**
*******************************************************************************
*/

class CProjCfgDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CProjCfgDlg();
	
	//
	// Members.
	//
	CSetupDoc*	m_pDoc;

protected:
	//
	// Controls.
	//
	CEditBox	m_ebTitle;
	CEditBox	m_ebProduct;
	CEditBox	m_ebAuthor;
	CComboBox	m_cbRoot;
	CEditBox	m_ebFolder;
	CCheckBox	m_ckProgIcon;
	CCheckBox	m_ckAllUsers;
	CCheckBox	m_ckNewGroup;
	CEditBox	m_ebGroup;
	CCheckBox	m_ckDeskIcon;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // PROJCFGDLG_HPP
