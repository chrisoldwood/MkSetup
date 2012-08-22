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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/SDICmds.hpp>

// Forward declarations.
class CAppWnd;

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
	CAppCmds(CAppWnd& appWnd);
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

	// Range of MRU commands.
	static const int ID_MRU_FIRST = ID_FILE_MRU_1;
	static const int ID_MRU_LAST  = ID_FILE_MRU_9;
	static const int NUM_MRU_CMDS = ID_MRU_LAST-ID_MRU_FIRST+1;

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
