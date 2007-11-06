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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/SDIDoc.hpp>

// Forward declarations.
class CFileProps;

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
	// Helper methods.
	//
	CFileProps* FindFile(const CString& strFileName);

	// Template shorthands.
	typedef std::vector<CFileProps*> CFileList;

	//
	// Members.
	//
	bool		m_bModified;		// Document modified?

	CString		m_strTitle;			// Setup window title.
	CString		m_strProduct;		// Product name.
	CString		m_strAuthor;		// Product author name.
	CString		m_strDefRoot;		// Default root folder.
	CString		m_strDefFolder;		// Default folder.
	bool		m_bProgIcon;		// Create program icons?
	bool		m_bAllUsers;		// Install for all users?
	CString		m_strProgGroup;		// Default program group name.
	bool		m_bNewGroup;		// Create a new group?
	bool		m_bDeskIcon;		// Create a desktop icon?

	CFileList	m_aoFiles;			// List of files to install.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // SETUPDOC_HPP
