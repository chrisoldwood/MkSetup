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
	: CSDIApp(m_AppWnd, m_AppCmds)
	, m_strLastDir(CPath::CurrentDir())
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

	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

	// Set the .INI file path.
	m_oIniFile.m_strPath  = CPath::ApplicationDir();
	m_oIniFile.m_strPath += "MkSetup.ini";

	// Load app settings.
	LoadConfig();
	
	// Load the MRUList.
	m_MRUList.Load(m_oIniFile);

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Move to last position.
	if ( (m_iCmdShow == SW_SHOWNORMAL) && (m_rcAppWnd.Empty() == false) )
		m_AppWnd.Move(m_rcAppWnd);

	// Show it.
	m_AppWnd.Show(m_iCmdShow);

	// Update UI.
	m_AppCmds.UpdateUI();

	return CSDIApp::OnOpen();
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
	CSDIApp::OnClose();

	// Save settings.
	SaveConfig();

	// Save MRU list.
	m_MRUList.Save(m_oIniFile);

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

	// Read UI settings.
	m_rcAppWnd   = m_oIniFile.ReadRect  ("Main", "Window",  CRect(0,0,0,0));
	m_strLastDir = m_oIniFile.ReadString("Main", "LastDir", m_strLastDir);
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

	// Write UI settings.
	m_oIniFile.WriteRect  ("Main", "Window",  m_rcAppWnd);
	m_oIniFile.WriteString("Main", "LastDir", m_strLastDir);
}
