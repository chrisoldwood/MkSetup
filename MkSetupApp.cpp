/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MKSETUPAPP.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CMakeSetupApp class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// "The" application object.
CMakeSetupApp App;

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

#ifdef _DEBUG
const char* CMakeSetupApp::VERSION      = "v1.0 [Debug]";
#else
const char* CMakeSetupApp::VERSION      = "v1.0";
#endif
const char* CMakeSetupApp::INI_FILE_VER = "1.0";

/******************************************************************************
** Method:		Constructor
**
** Description:	Default constructor.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CMakeSetupApp::CMakeSetupApp()
	: CApp(m_AppWnd, m_AppCmds)
{

}

/******************************************************************************
** Method:		Destructor
**
** Description:	Cleanup.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CMakeSetupApp::~CMakeSetupApp()
{

}

/******************************************************************************
** Method:		OnOpen()
**
** Description:	Initialises the application.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CMakeSetupApp::OnOpen()
{
	// Set the app title.
	m_strTitle = "Make Setup";

	// Load settings.
	LoadConfig();
	
	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	m_AppWnd.Show(m_iCmdShow);

	// Update UI.
	m_AppCmds.UpdateUI();

	return true;
}

/******************************************************************************
** Method:		OnClose()
**
** Description:	Cleans up the application resources.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CMakeSetupApp::OnClose()
{
	// Save settings.
	SaveConfig();

	return true;
}

/******************************************************************************
** Method:		LoadConfig()
**
** Description:	Load the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CMakeSetupApp::LoadConfig()
{
	// Read the file version.
	CString strVer = m_oIniFile.ReadString("Version", "Version", INI_FILE_VER);
}

/******************************************************************************
** Method:		SaveConfig()
**
** Description:	Save the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CMakeSetupApp::SaveConfig()
{
	// Write the file version.
	m_oIniFile.WriteString("Version", "Version", INI_FILE_VER);
}
