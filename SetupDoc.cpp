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

static const char* FILE_VER_10  = "1.0";
static const char* FILE_VER_11  = "1.1";
static const char* FILE_VER_12  = "1.2";
static const char* CUR_FILE_VER = FILE_VER_12;

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
	, m_strAuthor("Author")
	, m_strDefRoot("%ProgramFiles%")
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
	DeleteAll(m_aoFiles);
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

		if ( (strVer != FILE_VER_12) && (strVer != FILE_VER_11) && (strVer != FILE_VER_10) )
			throw CFileException(CFileException::E_VERSION_INVALID, m_Path);

		// Force write, if old format.
		if (strVer != CUR_FILE_VER)
		{
			App.NotifyMsg("The script was for Setup v%s and will be upgraded for Setup v%s.", strVer, CUR_FILE_VER);
			m_bModified = true;
		}

		// Load project settings.
		m_strTitle     = oScript.ReadString("Main", "Title",     m_strTitle    );
		m_strProduct   = oScript.ReadString("Main", "Product",   m_strProduct  );
		m_strAuthor    = oScript.ReadString("Main", "Author",    m_strAuthor   );
		m_strDefRoot   = oScript.ReadString("Main", "DefRoot",   m_strDefRoot  );
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
			CString strEntry, strValue;

			strEntry.Format("File[%d]", i);

			// Read filename + shortcut name + description.
			strValue = oScript.ReadString("Files", strEntry, "");

			CStrArray astrFields;

			// Split into file + name + description.
			CStrTok::Split(strValue, ',', astrFields);

			// Must have at least a filename.
			if ( (astrFields.Size() < 1) || (astrFields[0] == "") )
				throw CFileException(CFileException::E_FORMAT_INVALID, m_Path);

			// Create file props object and add to collection.
			CFileProps* pFileProps = new CFileProps(astrFields[0]);

			pFileProps->m_bProgIcon   = false;
			pFileProps->m_bDeskIcon   = false;
			pFileProps->m_strIconName = "";
			pFileProps->m_strIconDesc = "";

			// v1.2 file.
			if (strVer == FILE_VER_12)
			{
				ASSERT(astrFields.Size() == 4);

				pFileProps->m_strFolder   = astrFields[1];
				pFileProps->m_strIconName = astrFields[2];
				pFileProps->m_strIconDesc = astrFields[3];
			}
			// v1.1 file.
			else if (strVer == FILE_VER_11)
			{
				ASSERT((astrFields.Size() == 2) || (astrFields.Size() == 3));

				pFileProps->m_strIconName = astrFields[1];

				if (astrFields.Size() == 3)
					pFileProps->m_strIconDesc = astrFields[2];
			}
			// v1.0 file.
			else if (strVer == FILE_VER_10)
			{
				ASSERT(astrFields.Size() == 1);
			}

			m_aoFiles.push_back(pFileProps);
		}

		// Read shortcuts list.
		nFiles = oScript.ReadInt("Shortcuts", "Count", 0);

		for (int i = 0; i < nFiles; ++i)
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

		for (int i = 0; i < nFiles; ++i)
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
		oScript.WriteString("Main", "Author",    m_strAuthor   );
		oScript.WriteString("Main", "DefRoot",   m_strDefRoot  );
		oScript.WriteString("Main", "DefFolder", m_strDefFolder);
		oScript.WriteBool  ("Main", "ProgIcon",  m_bProgIcon   );
		oScript.WriteBool  ("Main", "AllUsers",  m_bAllUsers   );
		oScript.WriteString("Main", "ProgGroup", m_strProgGroup);
		oScript.WriteBool  ("Main", "NewGroup",  m_bNewGroup   );
		oScript.WriteBool  ("Main", "DeskIcon",  m_bDeskIcon   );

		// Write file list.
		oScript.DeleteSection("Files");
		oScript.WriteInt("Files", "Count", 0);

		int nCount = 0;

		for (uint i = 0; i < m_aoFiles.size(); ++i)
		{
			CFileProps* pFileProps = m_aoFiles[i];

			CString strEntry, strValue;

			strEntry.Format("File[%d]", nCount);

			// Generate file entry value.
			strValue  = pFileProps->m_strFileName;
			strValue += "," + pFileProps->m_strFolder;
			strValue += "," + pFileProps->m_strIconName;
			strValue += "," + pFileProps->m_strIconDesc;

			// Write filename.
			oScript.WriteString("Files", strEntry, strValue);

			++nCount;
		}

		oScript.WriteInt("Files", "Count", nCount);

		// Write shortcuts list.
		oScript.DeleteSection("Shortcuts");
		oScript.WriteInt("Shortcuts", "Count", 0);

		nCount = 0;

		for (uint i = 0; i < m_aoFiles.size(); ++i)
		{
			CFileProps* pFileProps = m_aoFiles[i];

			if (pFileProps->m_bProgIcon)
			{
				CString strEntry;

				strEntry.Format("Shortcut[%d]", nCount);

				// Write filename.
				oScript.WriteString("Shortcuts", strEntry, pFileProps->m_strFileName);

				++nCount;
			}
		}

		oScript.WriteInt("Shortcuts", "Count", nCount);

		// Write desktop icons list.
		oScript.DeleteSection("DeskIcons");
		oScript.WriteInt("DeskIcons", "Count", 0);

		nCount = 0;

		for (uint i = 0; i < m_aoFiles.size(); ++i)
		{
			CFileProps* pFileProps = m_aoFiles[i];

			if (pFileProps->m_bDeskIcon)
			{
				CString strEntry;

				strEntry.Format("DeskIcon[%d]", nCount);

				// Write filename.
				oScript.WriteString("DeskIcons", strEntry, pFileProps->m_strFileName);

				++nCount;
			}
		}

		oScript.WriteInt("DeskIcons", "Count", nCount);

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
	for (uint i = 0; i < m_aoFiles.size(); ++i)
	{
		CFileProps* pFileProps = m_aoFiles[i];

		if (pFileProps->m_strFileName == strFileName)
			return pFileProps;
	}

	return NULL;
}
