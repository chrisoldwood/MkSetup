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
	// Cleanup file list.
	m_aoFiles.DeleteAll();
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

		// Read file list.
		int nFiles = oScript.ReadInt("Files", "Count", 0);

		for (int i = 0; i < nFiles; ++i)
		{
			CString strEntry, strFileName;

			strEntry.Format("File[%d]", i);

			// Read filename.
			strFileName = oScript.ReadString("Files", strEntry, "");

			if (strFileName == "")
				continue;

			// Create file props object and add to collection.
			CFileProps* pFileProps = new CFileProps(strFileName);

			pFileProps->m_bProgIcon = false;
			pFileProps->m_bDeskIcon = false;

			m_aoFiles.Add(pFileProps);
		}

		// Read shortcuts list.
		nFiles = oScript.ReadInt("Shortcuts", "Count", 0);

		for (i = 0; i < nFiles; ++i)
		{
			CString strEntry, strFileName;

			strEntry.Format("Shortcut[%d]", i);

			// Read filename.
			strFileName = oScript.ReadString("Shortcuts", strEntry, "");

			// Find file and object and set flag.
			CFileProps* pFileProps = FindFile(strFileName);

			ASSERT(pFileProps != NULL);

			if (pFileProps != NULL)
				pFileProps->m_bProgIcon = true;
		}

		// Read desktop icons list.
		nFiles = oScript.ReadInt("DeskIcons", "Count", 0);

		for (i = 0; i < nFiles; ++i)
		{
			CString strEntry, strFileName;

			strEntry.Format("DeskIcon[%d]", i);

			// Read filename.
			strFileName = oScript.ReadString("DeskIcons", strEntry, "");

			// Find file and object and set flag.
			CFileProps* pFileProps = FindFile(strFileName);

			ASSERT(pFileProps != NULL);

			if (pFileProps != NULL)
				pFileProps->m_bDeskIcon = true;
		}
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
	try
	{
		CIniFile oScript(m_Path);

		// File is read-only?
		if ( (m_Path.Exists()) && (m_Path.ReadOnly()) )
			throw CFileException(CFileException::E_READ_ONLY, m_Path);

		// Read the .ini file version.
		oScript.WriteString("Version", "Version", CUR_FILE_VER);

		// Save project settings.
		oScript.WriteString("Main", "Title",     m_strTitle    );
		oScript.WriteString("Main", "Product",   m_strProduct  );
		oScript.WriteString("Main", "DefFolder", m_strDefFolder);
		oScript.WriteBool  ("Main", "ProgIcon",  m_bProgIcon   );
		oScript.WriteBool  ("Main", "AllUsers",  m_bAllUsers   );
		oScript.WriteString("Main", "ProgGroup", m_strProgGroup);
		oScript.WriteBool  ("Main", "NewGroup",  m_bNewGroup   );
		oScript.WriteBool  ("Main", "DeskIcon",  m_bDeskIcon   );

		// Write file list.
		oScript.DeleteSection("Files");
		oScript.WriteInt("Files", "Count", 0);

		for (int i = 0, c = 0; i < m_aoFiles.Size(); ++i, ++c)
		{
			CString strEntry;

			strEntry.Format("File[%d]", c);

			// Write filename.
			oScript.WriteString("Files", strEntry, m_aoFiles[i]->m_strFileName);
		}

		oScript.WriteInt("Files", "Count", c);

		// Write shortcuts list.
		oScript.DeleteSection("Shortcuts");
		oScript.WriteInt("Shortcuts", "Count", 0);

		for (i = 0, c = 0; i < m_aoFiles.Size(); ++i)
		{
			CFileProps* pFileProps = m_aoFiles[i];

			if (pFileProps->m_bProgIcon)
			{
				CString strEntry;

				strEntry.Format("Shortcut[%d]", c);

				// Write filename.
				oScript.WriteString("Shortcuts", strEntry, pFileProps->m_strFileName);

				++c;
			}
		}

		oScript.WriteInt("Shortcuts", "Count", c);

		// Write desktop icons list.
		oScript.DeleteSection("DeskIcons");
		oScript.WriteInt("DeskIcons", "Count", 0);

		for (i = 0, c = 0; i < m_aoFiles.Size(); ++i)
		{
			CFileProps* pFileProps = m_aoFiles[i];

			if (pFileProps->m_bDeskIcon)
			{
				CString strEntry;

				strEntry.Format("DeskIcon[%d]", c);

				// Write filename.
				oScript.WriteString("DeskIcons", strEntry, pFileProps->m_strFileName);

				++c;
			}
		}

		oScript.WriteInt("DeskIcons", "Count", c);

		// Reset status.
		m_bModified = false;
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
** Method:		FindFile()
**
** Description:	Find a file in the list by its filename.
**
** Parameters:	strFileName		The file to find.
**
** Returns:		The file object or NULL.
**
*******************************************************************************
*/

CFileProps* CSetupDoc::FindFile(const CString& strFileName)
{
	// For all files...
	for (int i = 0; i < m_aoFiles.Size(); ++i)
	{
		CFileProps* pFileProps = m_aoFiles[i];

		if (pFileProps->m_strFileName == strFileName)
			return pFileProps;
	}

	return NULL;
}
