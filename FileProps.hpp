/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEPROPS.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFileProps class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FILEPROPS_HPP
#define FILEPROPS_HPP

/******************************************************************************
** 
** The data class used to store the properties for a file in the install set.
**
*******************************************************************************
*/

class CFileProps
{
public:
	//
	// Constructors/Destructor.
	//
	CFileProps(const CPath& strFileName);
	
	//
	// Members.
	//
	CPath	m_strFileName;		// File name.
	bool	m_bProgIcon;		// Create program icon?
	bool	m_bDeskIcon;		// Create desktop icon?
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CFileProps::CFileProps(const CPath& strFileName)
	: m_strFileName(strFileName)
	, m_bProgIcon(false)
	, m_bDeskIcon(false)
{
	CString strExt = m_strFileName.FileExt().ToLower();

	m_bProgIcon = ((strExt == ".exe") || (strExt == ".hlp"));
	m_bDeskIcon = (strExt == ".exe");
}

#endif // FILEPROPS_HPP
