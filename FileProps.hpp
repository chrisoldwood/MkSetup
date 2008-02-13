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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/Path.hpp>

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
	CPath	m_strFolder;		// Destination folder.
	bool	m_bProgIcon;		// Create program icon?
	bool	m_bDeskIcon;		// Create desktop icon?
	CString	m_strIconName;		// Shortcut name.
	CString	m_strIconDesc;		// Shortcut description.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CFileProps::CFileProps(const CPath& strFileName)
	: m_strFileName(strFileName)
	, m_strFolder(TXT("%TargetDir%"))
	, m_bProgIcon(false)
	, m_bDeskIcon(false)
{
	CString strExt = m_strFileName.FileExt().ToLower();

	// Create icons for the application and helpfile.
	m_bProgIcon = ((strExt == TXT(".exe")) || (strExt == TXT(".hlp")));
	m_bDeskIcon = (strExt == TXT(".exe"));

	// Create default shortcut name + description.
	if (m_bProgIcon || m_bDeskIcon)
	{
		// Create default shortcut name.
		m_strIconName = strFileName.FileTitle();

		// Create default shortcut description.
		if (strExt == TXT(".exe"))
			m_strIconDesc = TXT("Launch ") + m_strIconName;

		if (strExt == TXT(".hlp"))
			m_strIconDesc = m_strIconName + TXT(" Manual");
	}
}

#endif // FILEPROPS_HPP
