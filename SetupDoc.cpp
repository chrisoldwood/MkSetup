/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SETUPDOC.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CSetupDoc class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

const char* CSetupDoc::CUR_FILE_VER = "1.0";

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

CSetupDoc::CSetupDoc()
	: m_bModified(false)
	, m_strTitle("Product Setup")
	, m_strProduct("Product")
	, m_strDefFolder("Product Folder")
	, m_bProgIcon(true)
	, m_bAllUsers(false)
	, m_strProgGroup("Product Group")
	, m_bNewGroup(true)
	, m_bDeskIcon(false)
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

CSetupDoc::~CSetupDoc()
{
}

/******************************************************************************
** Methods:		Modified()
**
** Description:	Query if the document has been modified.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CSetupDoc::Modified() const
{
	return m_bModified;
}

/******************************************************************************
** Method:		Load()
**
** Description:	Load the setup script.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CSetupDoc::Load()
{
	try
	{
		CIniFile oScript(m_Path);

		// File does not exist?
		if (!m_Path.Exists())
			throw CFileException(CFileException::E_PATH_INVALID, m_Path);

		// Read the .ini file version.
		CString strVer = oScript.ReadString("Version", "Version", "");

		if (strVer == "")
			throw CFileException(CFileException::E_FORMAT_INVALID, m_Path);

		if (strVer != CUR_FILE_VER)
			throw CFileException(CFileException::E_VERSION_INVALID, m_Path);

		// Load project settings.
		m_strTitle     = oScript.ReadString("Main", "Title",     m_strTitle    );
		m_strProduct   = oScript.ReadString("Main", "Product",   m_strProduct  );
		m_strDefFolder = oScript.ReadString("Main", "DefFolder", m_strDefFolder);
		m_bProgIcon    = oScript.ReadBool  ("Main", "ProgIcon",  m_bProgIcon   );
		m_bAllUsers    = oScript.ReadBool  ("Main", "AllUsers",  m_bAllUsers   );
		m_strProgGroup = oScript.ReadString("Main", "ProgGroup", m_strProgGroup);
		m_bNewGroup    = oScript.ReadBool  ("Main", "NewGroup",  m_bNewGroup   );
		m_bDeskIcon    = oScript.ReadBool  ("Main", "DeskIcon",  m_bDeskIcon   );
	}
	catch (CFileException& e)
	{
		// Notify user.
		App.m_AppWnd.AlertMsg(e.ErrorText());
		return false;
	}

	return true;
}

/******************************************************************************
** Method:		Save()
**
** Description:	Save the setup script.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CSetupDoc::Save()
{
	// Reset status.
	m_bModified = false;

	return true;
}
