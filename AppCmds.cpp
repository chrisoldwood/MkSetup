/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppCmds class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "AboutDlg.hpp"
#include "ProjCfgDlg.hpp"
#include "FilePropsDlg.hpp"

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

// Range of MRU commands.
const int ID_MRU_FIRST = ID_FILE_MRU_1;
const int ID_MRU_LAST  = ID_FILE_MRU_4;

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static char g_szExts[] = {	"All Files (*.*)\0*.*\0"
							"\0\0"						};

static char g_szDefExt[] = { "" };

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

CAppCmds::CAppCmds()
{
	// Define the command table.
	DEFINE_CMD_TABLE
		// File menu.
		CMD_ENTRY(ID_FILE_NEW,					OnFileNew,			NULL,					 0)
		CMD_ENTRY(ID_FILE_OPEN,					OnFileOpen,			NULL,					 1)
		CMD_ENTRY(ID_FILE_SAVE,					OnFileSave,			OnUIFileSave,			 2)
		CMD_ENTRY(ID_FILE_SAVEAS,				OnFileSaveAs,		OnUIFileSaveAs,			-1)
		CMD_ENTRY(ID_FILE_CLOSE,				OnFileClose,		OnUIFileClose,			 1)
		CMD_RANGE(ID_MRU_FIRST,	ID_MRU_LAST,	OnFileOpenMRU,		OnUIFileOpenMRU,		-1)
		CMD_ENTRY(ID_FILE_EXIT,					OnFileExit,			NULL,					-1)
		// File menu.
		CMD_ENTRY(ID_EDIT_ADD,					OnEditAddFile,		OnUIEditAddFile,		-1)
		CMD_ENTRY(ID_EDIT_ADD_MANY,				OnEditAddFiles,		OnUIEditAddFiles,		-1)
		CMD_ENTRY(ID_EDIT_PROPS,				OnEditFileProps,	OnUIEditFileProps,		-1)
		CMD_ENTRY(ID_EDIT_REMOVE,				OnEditRemoveFile,	OnUIEditRemoveFile,		-1)
		CMD_ENTRY(ID_EDIT_PROJ_CFG,				OnEditProjCfg,		OnUIEditProjCfg,		-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_ABOUT,				OnHelpAbout,		NULL,					10)
	END_CMD_TABLE
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

CAppCmds::~CAppCmds()
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

void CAppCmds::OnFileNew()
{
	NewFile();
}

/******************************************************************************
** Method:		OnFileCreated()
**
** Description:	New file created.
**
** Parameters:	oDoc	The doc affected.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileCreated(CSDIDoc& oDoc)
{
	OnFileOpened(oDoc);
}

/******************************************************************************
** Method:		OnFileOpen()
**
** Description:	Open a new file, saving the current one first if necessary.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileOpen()
{
	OpenFile();
}

/******************************************************************************
** Method:		OnFileOpened()
**
** Description:	File opened.
**
** Parameters:	oDoc	The doc affected.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileOpened(CSDIDoc&)
{
	// Refresh view.
	App.View()->RefreshFileList();

	UpdateUI();
}

/******************************************************************************
** Method:		OnFileSave()
**
** Description:	Save the current file, asking the user for a file name if
**				it has not being set yet.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileSave()
{
	SaveFile();
}

/******************************************************************************
** Method:		OnFileSaveAs()
**
** Description:	Save the file to a new file name.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileSaveAs()
{
	SaveFileAs();
}

/******************************************************************************
** Method:		OnFileClose()
**
** Description:	Close the current file, saving it if necessary.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileClose()
{
	CloseFile();
}

/******************************************************************************
** Method:		OnFileOpenMRU()
**
** Description:	Open a MRU file from the MRU list.
**
** Parameters:	iCmdID		The ID of the command.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileOpenMRU(uint iCmdID)
{
	OpenMRUFile(iCmdID - ID_MRU_FIRST);
}

/******************************************************************************
** Method:		OnFileExit()
**
** Description:	Terminate the app.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileExit()
{
	App.m_AppWnd.Close();
}

/******************************************************************************
** Method:		OnEditAddFile()
**
** Description:	Add a file to the install set.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnEditAddFile()
{
	ASSERT(App.Doc() != NULL);

	// Get doc and view.
	CSetupDoc*  pDoc  = App.Doc();
	CSetupView* pView = App.View();

	CPath strFile;

	// Show file select dialog.
	if (!strFile.Select(App.m_AppWnd, CPath::SelectFile, g_szExts, g_szDefExt, App.m_strLastDir))
		return;

	// Remember folder.
	App.m_strLastDir = strFile.Directory();

	// Create file props object.
	CFileProps* pFileProps = new CFileProps(strFile.FileName());

	// Add to document.
	pDoc->m_aoFiles.Add(pFileProps);
	pDoc->m_bModified = true;

	// Refresh view.
	pView->RefreshFileList();

	UpdateUI();
}

/******************************************************************************
** Method:		OnEditAddFiles()
**
** Description:	Add many files to the install set.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnEditAddFiles()
{
	ASSERT(App.Doc() != NULL);

	// Get doc and view.
	CSetupDoc*  pDoc  = App.Doc();
	CSetupView* pView = App.View();

	CStrArray astrFiles;

	// Show file select dialog.
	if (!CPath::SelectFiles(App.m_AppWnd, g_szExts, g_szDefExt, App.m_strLastDir, astrFiles))
		return;

	// Nothing selected?
	if (astrFiles.Size() == 0)
		return;

	// For all files...
	for (int i = 0; i < astrFiles.Size(); ++i)
	{
		CPath strFile = astrFiles[i];

		// Create file props object.
		CFileProps* pFileProps = new CFileProps(strFile.FileName());

		// Add to document.
		pDoc->m_aoFiles.Add(pFileProps);

		// Remember folder.
		App.m_strLastDir = strFile.Directory();
	}

	pDoc->m_bModified = true;

	// Refresh view.
	pView->RefreshFileList();

	UpdateUI();
}

/******************************************************************************
** Method:		OnEditFileProps()
**
** Description:	Edit the properties for the selected file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnEditFileProps()
{
	ASSERT(App.Doc() != NULL);

	// Get doc and view.
	CSetupDoc*  pDoc  = App.Doc();
	CSetupView* pView = App.View();

	// Get selected file.
	CFileProps* pFileProps = pView->GetSelection();

	ASSERT(pFileProps != NULL);

	CFilePropsDlg Dlg;

	Dlg.m_pFileProps = pFileProps;

	// Show project settings dialog.
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		pDoc->m_bModified = true;

		// Refresh view.
		pView->RefreshFile(pFileProps);

		UpdateUI();
	}
}

/******************************************************************************
** Method:		OnEditRemoveFile()
**
** Description:	Remove the selected file from the install set.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnEditRemoveFile()
{
	ASSERT(App.Doc() != NULL);

	// Get doc and view.
	CSetupDoc*  pDoc  = App.Doc();
	CSetupView* pView = App.View();

	// Get selected file.
	CFileProps* pFileProps = pView->GetSelection();

	ASSERT(pFileProps != NULL);

	// Remove from collection.
	pDoc->m_aoFiles.Delete(pDoc->m_aoFiles.Find(pFileProps));
	pDoc->m_bModified = true;

	// Refresh view.
	pView->RefreshFileList();

	UpdateUI();
}

/******************************************************************************
** Method:		OnEditProjCfg()
**
** Description:	Show the dialog to edit the main project settings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnEditProjCfg()
{
	ASSERT(App.Doc() != NULL);

	CProjCfgDlg Dlg;

	Dlg.m_pDoc = App.Doc();

	// Show project settings dialog.
	Dlg.RunModal(App.m_rMainWnd);

	UpdateUI();
}

/******************************************************************************
** Method:		OnHelpAbout()
**
** Description:	Show the about dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnHelpAbout()
{
	CAboutDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnUI...()
**
** Description:	UI update handlers.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnUIFileSave()
{
	bool bDocOpen  = (App.m_pDoc != NULL);
	bool bModified = (bDocOpen && App.m_pDoc->Modified());

	App.m_AppWnd.m_Menu.EnableCmd(ID_FILE_SAVE, (bDocOpen && bModified));
	App.m_AppWnd.m_ToolBar.m_SaveBtn.Enable(bDocOpen && bModified);
}

void CAppCmds::OnUIFileSaveAs()
{
	bool bDocOpen = (App.m_pDoc != NULL);

	App.m_AppWnd.m_Menu.EnableCmd(ID_FILE_SAVEAS, bDocOpen);
}

void CAppCmds::OnUIFileClose()
{
	bool bDocOpen = (App.m_pDoc != NULL);

	App.m_AppWnd.m_Menu.EnableCmd(ID_FILE_CLOSE, bDocOpen);
}

void CAppCmds::OnUIFileOpenMRU()
{
	App.m_MRUList.UpdateMenu(App.m_AppWnd.m_Menu, ID_MRU_FIRST);
}

void CAppCmds::OnUIEditAddFile()
{
	bool bDocOpen = (App.m_pDoc != NULL);

	App.m_AppWnd.m_Menu.EnableCmd(ID_EDIT_ADD, bDocOpen);
}

void CAppCmds::OnUIEditAddFiles()
{
	bool bDocOpen = (App.m_pDoc != NULL);

	App.m_AppWnd.m_Menu.EnableCmd(ID_EDIT_ADD_MANY, bDocOpen);
}

void CAppCmds::OnUIEditFileProps()
{
	bool bDocOpen = (App.m_pDoc  != NULL);
	bool bIsSel   = (App.m_pView != NULL) && (App.View()->IsFileSelected());

	App.m_AppWnd.m_Menu.EnableCmd(ID_EDIT_PROPS, bDocOpen && bIsSel);
}

void CAppCmds::OnUIEditRemoveFile()
{
	bool bDocOpen = (App.m_pDoc  != NULL);
	bool bIsSel   = (App.m_pView != NULL) && (App.View()->IsFileSelected());

	App.m_AppWnd.m_Menu.EnableCmd(ID_EDIT_REMOVE, bDocOpen && bIsSel);
}

void CAppCmds::OnUIEditProjCfg()
{
	bool bDocOpen = (App.m_pDoc != NULL);

	App.m_AppWnd.m_Menu.EnableCmd(ID_EDIT_PROJ_CFG, bDocOpen);
}
