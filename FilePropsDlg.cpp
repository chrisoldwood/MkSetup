/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEPROPSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFilePropsDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "FilePropsDlg.hpp"

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

CFilePropsDlg::CFilePropsDlg()
	: CDialog(IDD_FILE_PROPS)
	, m_pFileProps(NULL)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FILE_NAME, &m_ebFileName)
		CTRL(IDC_PROG_ICON, &m_ckProgIcon)
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

void CFilePropsDlg::OnInitDialog()
{
	ASSERT(m_pFileProps != NULL);

	// Initialise controls.
	m_ebFileName.Text(m_pFileProps->m_strFileName);
	m_ckProgIcon.Check(m_pFileProps->m_bProgIcon);
	m_ckDeskIcon.Check(m_pFileProps->m_bDeskIcon);
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

bool CFilePropsDlg::OnOk()
{
	// Save changes.
	m_pFileProps->m_strFileName = m_ebFileName.Text();
	m_pFileProps->m_bProgIcon   = m_ckProgIcon.IsChecked();
	m_pFileProps->m_bDeskIcon   = m_ckDeskIcon.IsChecked();

	return true;
}
