/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppCmds class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPCMDS_HPP
#define APPCMDS_HPP

/******************************************************************************
** 
** Application command controller.
**
*******************************************************************************
*/

class CAppCmds : public CSDICmds
{
public:
	//
	// Constructors/Destructor.
	//
	CAppCmds();
	~CAppCmds();

	//
	// Commands.
	//

	// File menu.
	void OnFileNew();
	void OnFileOpen();
	void OnFileSave();
	void OnFileSaveAs();
	void OnFileClose();
	void OnFileOpenMRU(uint iCmdID);
	void OnFileExit();

	// Edit menu.
	void OnEditAddFile();
	void OnEditAddFiles();
	void OnEditFileProps();
	void OnEditRemoveFile();
	void OnEditProjCfg();

	// Help menu.
	void OnHelpAbout();

	//
	// UI handlers.
	//

	// File menu.
	void OnUIFileSave();
	void OnUIFileSaveAs();
	void OnUIFileClose();
	void OnUIFileOpenMRU();

	// Edit menu.
	void OnUIEditAddFile();
	void OnUIEditAddFiles();
	void OnUIEditFileProps();
	void OnUIEditRemoveFile();
	void OnUIEditProjCfg();

protected:
	//
	// Template methods..
	//
	virtual void OnFileCreated(CSDIDoc& oDoc);
	virtual void OnFileOpened(CSDIDoc& oDoc);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPCMDS_HPP
