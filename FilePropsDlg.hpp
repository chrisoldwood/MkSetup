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
** .
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
	CCheckBox	m_ckProgIcon;
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

#endif // FILEPROPSDLG_HPP
