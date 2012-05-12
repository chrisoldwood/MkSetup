/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROJCFGDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CProjCfgDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ProjCfgDlg.hpp"
#include "SetupDoc.hpp"

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
		CTRL(IDC_ROOT,		&m_cbRoot    )
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

	// Initialise simple controls.
	m_ebTitle.Text(m_pDoc->m_strTitle);
	m_ebProduct.Text(m_pDoc->m_strProduct);
	m_ebAuthor.Text(m_pDoc->m_strAuthor);
	m_ebFolder.Text(m_pDoc->m_strDefFolder);
	m_ckProgIcon.Check(m_pDoc->m_bProgIcon);
	m_ckAllUsers.Check(m_pDoc->m_bAllUsers);
	m_ckNewGroup.Check(m_pDoc->m_bNewGroup);
	m_ebGroup.Text(m_pDoc->m_strProgGroup);
	m_ckDeskIcon.Check(m_pDoc->m_bDeskIcon);

	// Load root folder combo with defaults.
	m_cbRoot.Add(TXT("%ProgramFiles%"));
	m_cbRoot.Add(TXT("%SystemRoot%"));
	m_cbRoot.Add(TXT("%Temp%"));

	// Select destination folder, adding it if a custom one.
	size_t nFolder = m_cbRoot.FindExact(m_pDoc->m_strDefRoot);

	if (nFolder == CB_ERR)
		nFolder = m_cbRoot.Add(m_pDoc->m_strDefRoot);

	m_cbRoot.CurSel(nFolder);

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
	m_pDoc->m_strDefRoot   = m_cbRoot.Text();
	m_pDoc->m_strDefFolder = m_ebFolder.Text();
	m_pDoc->m_bProgIcon    = m_ckProgIcon.IsChecked();
	m_pDoc->m_bAllUsers    = m_ckAllUsers.IsChecked();
	m_pDoc->m_bNewGroup    = m_ckNewGroup.IsChecked();
	m_pDoc->m_strProgGroup = m_ebGroup.Text();
	m_pDoc->m_bDeskIcon    = m_ckDeskIcon.IsChecked();

	// Validate changes.
	if (m_ebProduct.TextLength() == 0)
	{
		AlertMsg(TXT("Please provide the product name."));
		m_ebProduct.Focus();
		return false;
	}

	if (m_ebTitle.TextLength() == 0)
	{
		AlertMsg(TXT("Please provide the Setup window title."));
		m_ebTitle.Focus();
		return false;
	}

	if (m_cbRoot.TextLength() == 0)
	{
		AlertMsg(TXT("Please provide the root installation folder."));
		m_cbRoot.Focus();
		return false;
	}

	if (m_ebFolder.TextLength() == 0)
	{
		AlertMsg(TXT("Please provide the program folder name."));
		m_ebFolder.Focus();
		return false;
	}

	if (m_ebGroup.TextLength() == 0)
	{
		AlertMsg(TXT("Please provide the program group name."));
		m_ebGroup.Focus();
		return false;
	}

	// Mark doc dirty.
	m_pDoc->m_bModified = true;

	return true;
}
