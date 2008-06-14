/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SETUPDOC.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CSetupDoc class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "SetupDoc.hpp"
#include <Legacy/STLUtils.hpp>
#include <WCL/IniFile.hpp>
#include <WCL/FileException.hpp>
#include "MkSetupApp.hpp"
#include <WCL/StrTok.hpp>
#include "FileProps.hpp"

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

static const tchar* FILE_VER_10  = TXT("1.0");
static const tchar* FILE_VER_11  = TXT("1.1");
static const tchar* FILE_VER_12  = TXT("1.2");
static const tchar* CUR_FILE_VER = FILE_VER_12;

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
	, m_strTitle(TXT("Product Setup"))
	, m_strProduct(TXT("Product"))
	, m_strAuthor(TXT("Author"))
	, m_strDefRoot(TXT("%ProgramFiles%"))
	, m_strDefFolder(TXT("Product Folder"))
	, m_bProgIcon(true)
	, m_bAllUsers(false)
	, m_strProgGroup(TXT("Product Group"))
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
			throw CFileException(CFileException::E_PATH_INVALID, m_Path, ERROR_PATH_NOT_FOUND);

		// Read the .ini file version.
		CString strVer = oScript.ReadString(TXT("Version"), TXT("Version"), TXT(""));

		if (strVer == TXT(""))
			throw CFileException(CFileException::E_FORMAT_INVALID, m_Path, ERROR_FILE_CORRUPT);

		if ( (strVer != FILE_VER_12) && (strVer != FILE_VER_11) && (strVer != FILE_VER_10) )
			throw CFileException(CFileException::E_VERSION_INVALID, m_Path, ERROR_FILE_CORRUPT);

		// Force write, if old format.
		if (strVer != CUR_FILE_VER)
		{
			App.NotifyMsg(TXT("The script was for Setup v%s and will be upgraded for Setup v%s."), strVer, CUR_FILE_VER);
			m_bModified = true;
		}

		// Load project settings.
		m_strTitle     = oScript.ReadString(TXT("Main"), TXT("Title"),     m_strTitle    );
		m_strProduct   = oScript.ReadString(TXT("Main"), TXT("Product"),   m_strProduct  );
		m_strAuthor    = oScript.ReadString(TXT("Main"), TXT("Author"),    m_strAuthor   );
		m_strDefRoot   = oScript.ReadString(TXT("Main"), TXT("DefRoot"),   m_strDefRoot  );
		m_strDefFolder = oScript.ReadString(TXT("Main"), TXT("DefFolder"), m_strDefFolder);
		m_bProgIcon    = oScript.ReadBool  (TXT("Main"), TXT("ProgIcon"),  m_bProgIcon   );
		m_bAllUsers    = oScript.ReadBool  (TXT("Main"), TXT("AllUsers"),  m_bAllUsers   );
		m_strProgGroup = oScript.ReadString(TXT("Main"), TXT("ProgGroup"), m_strProgGroup);
		m_bNewGroup    = oScript.ReadBool  (TXT("Main"), TXT("NewGroup"),  m_bNewGroup   );
		m_bDeskIcon    = oScript.ReadBool  (TXT("Main"), TXT("DeskIcon"),  m_bDeskIcon   );

		// Read file list.
		int nFiles = oScript.ReadInt(TXT("Files"), TXT("Count"), 0);

		for (int i = 0; i < nFiles; ++i)
		{
			CString strEntry, strValue;

			strEntry.Format(TXT("File[%d]"), i);

			// Read filename + shortcut name + description.
			strValue = oScript.ReadString(TXT("Files"), strEntry, TXT(""));

			CStrArray astrFields;

			// Split into file + name + description.
			CStrTok::Split(strValue, TXT(','), astrFields);

			// Must have at least a filename.
			if ( (astrFields.Size() < 1) || (astrFields[0] == TXT("")) )
				throw CFileException(CFileException::E_FORMAT_INVALID, m_Path, ERROR_FILE_CORRUPT);

			// Create file props object and add to collection.
			CFileProps* pFileProps = new CFileProps(astrFields[0]);

			pFileProps->m_bProgIcon   = false;
			pFileProps->m_bDeskIcon   = false;
			pFileProps->m_strIconName = TXT("");
			pFileProps->m_strIconDesc = TXT("");

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
		nFiles = oScript.ReadInt(TXT("Shortcuts"), TXT("Count"), 0);

		for (int i = 0; i < nFiles; ++i)
		{
			CString strEntry, strFileName;

			strEntry.Format(TXT("Shortcut[%d]"), i);

			// Read filename.
			strFileName = oScript.ReadString(TXT("Shortcuts"), strEntry, TXT(""));

			// Find file and object and set flag.
			CFileProps* pFileProps = FindFile(strFileName);

			ASSERT(pFileProps != NULL);

			if (pFileProps != NULL)
				pFileProps->m_bProgIcon = true;
		}

		// Read desktop icons list.
		nFiles = oScript.ReadInt(TXT("DeskIcons"), TXT("Count"), 0);

		for (int i = 0; i < nFiles; ++i)
		{
			CString strEntry, strFileName;

			strEntry.Format(TXT("DeskIcon[%d]"), i);

			// Read filename.
			strFileName = oScript.ReadString(TXT("DeskIcons"), strEntry, TXT(""));

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
		App.m_AppWnd.AlertMsg(TXT("%s"), e.What());
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
			throw CFileException(CFileException::E_READ_ONLY, m_Path, ERROR_ACCESS_DENIED);

		// Read the .ini file version.
		oScript.WriteString(TXT("Version"), TXT("Version"), CUR_FILE_VER);

		// Save project settings.
		oScript.WriteString(TXT("Main"), TXT("Title"),     m_strTitle    );
		oScript.WriteString(TXT("Main"), TXT("Product"),   m_strProduct  );
		oScript.WriteString(TXT("Main"), TXT("Author"),    m_strAuthor   );
		oScript.WriteString(TXT("Main"), TXT("DefRoot"),   m_strDefRoot  );
		oScript.WriteString(TXT("Main"), TXT("DefFolder"), m_strDefFolder);
		oScript.WriteBool  (TXT("Main"), TXT("ProgIcon"),  m_bProgIcon   );
		oScript.WriteBool  (TXT("Main"), TXT("AllUsers"),  m_bAllUsers   );
		oScript.WriteString(TXT("Main"), TXT("ProgGroup"), m_strProgGroup);
		oScript.WriteBool  (TXT("Main"), TXT("NewGroup"),  m_bNewGroup   );
		oScript.WriteBool  (TXT("Main"), TXT("DeskIcon"),  m_bDeskIcon   );

		// Write file list.
		oScript.DeleteSection(TXT("Files"));
		oScript.WriteInt(TXT("Files"), TXT("Count"), 0);

		int nCount = 0;

		for (uint i = 0; i < m_aoFiles.size(); ++i)
		{
			CFileProps* pFileProps = m_aoFiles[i];

			CString strEntry, strValue;

			strEntry.Format(TXT("File[%d]"), nCount);

			// Generate file entry value.
			strValue  = pFileProps->m_strFileName;
			strValue += TXT(",") + pFileProps->m_strFolder;
			strValue += TXT(",") + pFileProps->m_strIconName;
			strValue += TXT(",") + pFileProps->m_strIconDesc;

			// Write filename.
			oScript.WriteString(TXT("Files"), strEntry, strValue);

			++nCount;
		}

		oScript.WriteInt(TXT("Files"), TXT("Count"), nCount);

		// Write shortcuts list.
		oScript.DeleteSection(TXT("Shortcuts"));
		oScript.WriteInt(TXT("Shortcuts"), TXT("Count"), 0);

		nCount = 0;

		for (uint i = 0; i < m_aoFiles.size(); ++i)
		{
			CFileProps* pFileProps = m_aoFiles[i];

			if (pFileProps->m_bProgIcon)
			{
				CString strEntry;

				strEntry.Format(TXT("Shortcut[%d]"), nCount);

				// Write filename.
				oScript.WriteString(TXT("Shortcuts"), strEntry, pFileProps->m_strFileName);

				++nCount;
			}
		}

		oScript.WriteInt(TXT("Shortcuts"), TXT("Count"), nCount);

		// Write desktop icons list.
		oScript.DeleteSection(TXT("DeskIcons"));
		oScript.WriteInt(TXT("DeskIcons"), TXT("Count"), 0);

		nCount = 0;

		for (uint i = 0; i < m_aoFiles.size(); ++i)
		{
			CFileProps* pFileProps = m_aoFiles[i];

			if (pFileProps->m_bDeskIcon)
			{
				CString strEntry;

				strEntry.Format(TXT("DeskIcon[%d]"), nCount);

				// Write filename.
				oScript.WriteString(TXT("DeskIcons"), strEntry, pFileProps->m_strFileName);

				++nCount;
			}
		}

		oScript.WriteInt(TXT("DeskIcons"), TXT("Count"), nCount);

		// Reset status.
		m_bModified = false;
	}
	catch (CFileException& e)
	{
		// Notify user.
		App.m_AppWnd.AlertMsg(TXT("%s"), e.What());
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
