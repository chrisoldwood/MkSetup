/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MKSETUPAPP.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CMakeSetupApp class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MKSETUPAPP_HPP
#define MKSETUPAPP_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/SDIApp.hpp>
#include "AppWnd.hpp"
#include "AppCmds.hpp"
#include "SetupDoc.hpp"
#include "SetupView.hpp"
#include <WCL/IniFile.hpp>

/******************************************************************************
** 
** The application class.
**
*******************************************************************************
*/

class CMakeSetupApp : public CSDIApp
{
public:
	//
	// Constructors/Destructor.
	//
	CMakeSetupApp();
	~CMakeSetupApp();

	//
	// Typed access to the doc and view.
	//
	CSetupDoc*  Doc();
	CSetupView* View();

	//
	// Members
	//
	CAppWnd		m_AppWnd;			// Main window.
	CAppCmds	m_AppCmds;			// Command handler.
	CIniFile	m_oIniFile;			// .INI File
	CRect		m_rcAppWnd;			// Last position of main window.
	CPath		m_strLastDir;		// Last folder used.

	//
	// Constants.
	//
	static const tchar* VERSION;

protected:
	//
	// Startup and Shutdown template methods.
	//
	virtual	bool OnOpen();
	virtual	bool OnClose();

	//
	// Internal methods.
	//
	void LoadConfig();
	void SaveConfig();

	//
	// Template methods for getting doc and view specifics.
	//
	virtual CSDIDoc*    CreateDoc() const;
	virtual CView*      CreateView(CDoc& rDoc) const;
	virtual const tchar* FileExts() const;
	virtual const tchar* DefFileExt() const;

	//
	// Constants.
	//
	static const tchar* INI_FILE_VER;
};

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// The application object.
extern CMakeSetupApp App;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CSetupDoc* CMakeSetupApp::Doc()
{
	return static_cast<CSetupDoc*>(m_pDoc);
}

inline CSetupView* CMakeSetupApp::View()
{
	return static_cast<CSetupView*>(m_pView);
}

inline CSDIDoc* CMakeSetupApp::CreateDoc() const
{
	return new CSetupDoc;
}

inline CView* CMakeSetupApp::CreateView(CDoc& rDoc) const
{
	return new CSetupView(static_cast<CSetupDoc&>(rDoc));
}

inline const tchar* CMakeSetupApp::FileExts() const
{
	static tchar szExts[] = {	TXT("Setup Files (*.ini)\0*.ini\0")
								TXT("All Files (*.*)\0*.*\0")
								TXT("\0\0")							};

	return szExts;
}

inline const tchar* CMakeSetupApp::DefFileExt() const
{
	static tchar szDefExt[] = { TXT("ini") };

	return szDefExt;
}

#endif //MKSETUPAPP_HPP
