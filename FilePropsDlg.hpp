/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEPROPSDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFilePropsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FILEPROPSDLG_HPP
#define FILEPROPSDLG_HPP

/******************************************************************************
** 
** The dialog used to edit the installtion properties for a file.
**
*******************************************************************************
*/

class CFilePropsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CFilePropsDlg();
	
	//
	// Members.
	//
	CFileProps*	m_pFileProps;
	
private:
	//
	// Controls.
	//
	CEditBox	m_ebFileName;
	CComboBox	m_cbFolder;
	CCheckBox	m_ckProgIcon;
	CCheckBox	m_ckDeskIcon;
	CEditBox	m_ebIconName;
	CEditBox	m_ebIconDesc;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	void OnFileIcon();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // FILEPROPSDLG_HPP
