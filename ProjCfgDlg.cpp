/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROJCFGDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CProjCfgDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ProjCfgDlg.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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

CProjCfgDlg::CProjCfgDlg()
	: CDialog(IDD_PROJ_CFG)
	, m_pDoc(NULL)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_TITLE,		&m_ebTitle   )
		CTRL(IDC_PRODUCT,	&m_ebProduct )
		CTRL(IDC_AUTHOR,	&m_ebAuthor  )
		CTRL(IDC_FOLDER,	&m_ebFolder  )
		CTRL(IDC_PROG_ICON, &m_ckProgIcon)
		CTRL(IDC_ALL_USERS, &m_ckAllUsers)
		CTRL(IDC_NEW_GROUP, &m_ckNewGroup)
		CTRL(IDC_GROUP,		&m_ebGroup   )
		CTRL(IDC_DESK_ICON, &m_ckDeskIcon)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
	END_CTRLMSG_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CProjCfgDlg::OnInitDialog()
{
	ASSERT(m_pDoc != NULL);

	// Initialise controls.
	m_ebTitle.Text(m_pDoc->m_strTitle);
	m_ebProduct.Text(m_pDoc->m_strProduct);
	m_ebAuthor.Text(m_pDoc->m_strAuthor);
	m_ebFolder.Text(m_pDoc->m_strDefFolder);
	m_ckProgIcon.Check(m_pDoc->m_bProgIcon);
	m_ckAllUsers.Check(m_pDoc->m_bAllUsers);
	m_ckNewGroup.Check(m_pDoc->m_bNewGroup);
	m_ebGroup.Text(m_pDoc->m_strProgGroup);
	m_ckDeskIcon.Check(m_pDoc->m_bDeskIcon);
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	The OK button was pressed.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CProjCfgDlg::OnOk()
{
	// Get values from controls.
	m_pDoc->m_strTitle     = m_ebTitle.Text();
	m_pDoc->m_strProduct   = m_ebProduct.Text();
	m_pDoc->m_strAuthor    = m_ebAuthor.Text();
	m_pDoc->m_strDefFolder = m_ebFolder.Text();
	m_pDoc->m_bProgIcon    = m_ckProgIcon.IsChecked();
	m_pDoc->m_bAllUsers    = m_ckAllUsers.IsChecked();
	m_pDoc->m_bNewGroup    = m_ckNewGroup.IsChecked();
	m_pDoc->m_strProgGroup = m_ebGroup.Text();
	m_pDoc->m_bDeskIcon    = m_ckDeskIcon.IsChecked();

	// Mark doc dirty.
	m_pDoc->m_bModified = true;

	return true;
}
