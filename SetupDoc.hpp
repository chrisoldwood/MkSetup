/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SETUPDOC.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CSetupDoc class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SETUPDOC_HPP
#define SETUPDOC_HPP

/******************************************************************************
** 
** The application document class.
**
*******************************************************************************
*/

class CSetupDoc : public CSDIDoc
{
public:
	//
	// Constructors/Destructor.
	//
	CSetupDoc();
	~CSetupDoc();
	
	//
	// Persistance methods.
	//
	virtual bool Modified() const;

	virtual bool Load();
	virtual bool Save();

	//
	// Members.
	//
	bool	m_bModified;

	CString	m_strTitle;
	CString	m_strProduct;
	CString	m_strDefFolder;
	bool	m_bProgIcon;
	bool	m_bAllUsers;
	CString	m_strProgGroup;
	bool	m_bNewGroup;
	bool	m_bDeskIcon;

private:
	//
	// Constants.
	//
	static const char* CUR_FILE_VER;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // SETUPDOC_HPP
