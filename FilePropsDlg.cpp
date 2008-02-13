/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEPROPSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFilePropsDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "FilePropsDlg.hpp"
#include "FileProps.hpp"

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
		CTRL(IDC_FOLDER,    &m_cbFolder)
		CTRL(IDC_PROG_ICON, &m_ckProgIcon)
		CTRL(IDC_DESK_ICON, &m_ckDeskIcon)
		CTRL(IDC_ICON_NAME, &m_ebIconName)
		CTRL(IDC_ICON_DESC, &m_ebIconDesc)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_PROG_ICON, BN_CLICKED, &CFilePropsDlg::OnFileIcon)
		CMD_CTRLMSG(IDC_DESK_ICON, BN_CLICKED, &CFilePropsDlg::OnFileIcon)
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
	m_ebIconName.Text(m_pFileProps->m_strIconName);
	m_ebIconDesc.Text(m_pFileProps->m_strIconDesc);

	// Load destination folder combo with defaults.
	m_cbFolder.Add(TXT("%TargetDir%"));
	m_cbFolder.Add(TXT("%ProgramFiles%"));
	m_cbFolder.Add(TXT("%SystemRoot%"));
	m_cbFolder.Add(TXT("%Temp%"));

	// Select destination folder, adding it if a custom one.
	int nFolder = m_cbFolder.FindExact(m_pFileProps->m_strFolder);

	if (nFolder == CB_ERR)
		nFolder = m_cbFolder.Add(m_pFileProps->m_strFolder);

	m_cbFolder.CurSel(nFolder);

	// Initialise other UI.
	OnFileIcon();
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
	m_pFileProps->m_strFolder   = m_cbFolder.Text();
	m_pFileProps->m_bProgIcon   = m_ckProgIcon.IsChecked();
	m_pFileProps->m_bDeskIcon   = m_ckDeskIcon.IsChecked();
	m_pFileProps->m_strIconName = m_ebIconName.Text();
	m_pFileProps->m_strIconDesc = m_ebIconDesc.Text();

	// Validate changes.
	if (m_cbFolder.TextLength() == 0)
	{
		AlertMsg(TXT("Please provide the install folder."));
		m_cbFolder.Focus();
		return false;
	}

	if (m_pFileProps->m_bProgIcon || m_pFileProps->m_bDeskIcon)
	{
		// No shortcut name?
		if (m_pFileProps->m_strIconName == TXT(""))
		{
			AlertMsg(TXT("Please provide the shortcut name."));
			m_ebIconName.Focus();
			return false;
		}

		// No shortcut description?
		if (m_pFileProps->m_strIconDesc == TXT(""))
		{
			AlertMsg(TXT("Please provide the shortcut description."));
			m_ebIconDesc.Focus();
			return false;
		}
	}

	return true;
}

/******************************************************************************
** Method:		OnFileIcon()
**
** Description:	One of the icon checkboxes (program/desktop) was changed.
**				Enable/disable the shortcut edit boxes.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CFilePropsDlg::OnFileIcon()
{
	bool bProgIcon = m_ckProgIcon.IsChecked();
	bool bDeskIcon = m_ckDeskIcon.IsChecked();

	m_ebIconName.Enable(bProgIcon || bDeskIcon);
	m_ebIconDesc.Enable(bProgIcon || bDeskIcon);
}
