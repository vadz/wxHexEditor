/***********************************(GPL)********************************
*   wxHexEditor is a hex edit tool for editing massive files in Linux   *
*   Copyright (C) 2010  Erdem U. Altinyurt                              *
*                                                                       *
*   This program is free software; you can redistribute it and/or       *
*   modify it under the terms of the GNU General Public License         *
*   as published by the Free Software Foundation; either version 2      *
*   of the License, or any later version.                               *
*                                                                       *
*   This program is distributed in the hope that it will be useful,     *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
*   GNU General Public License for more details.                        *
*                                                                       *
*   You should have received a copy of the GNU General Public License   *
*   along with this program;                                            *
*   if not, write to the Free Software	Foundation, Inc.,                *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA        *
*                                                                       *
*               home  : wxhexeditor.sourceforge.net                     *
*               email : death_knight at gamebox.net                     *
*************************************************************************/

#ifndef __HexEditorFrame__
#define __HexEditorFrame__

#include "HexEditorGui.h"
#include "HexEditor.h"
#include "HexEditorApp.h"
#include "HexDialogs.h"
#include "InfoPanel.h"
#include "../resources/wxhex.xpm"
#include <wx/filename.h>
#include <wx/dnd.h>
#include <wx/aboutdlg.h>
#include <wx/artprov.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibar.h>
#include <wx/aui/auibook.h>
#include <wx/config.h>
#include <wx/url.h>
#include <wx/textdlg.h>
class DnDFile;

class HexEditorFrame : public HexEditorGui {
	public:
		HexEditorFrame();
		HexEditorFrame(	wxWindow* parent, wxWindowID id = wxID_ANY );
		~HexEditorFrame();
		void TagHideAll();

	protected:
		void OnMenuEvent( wxCommandEvent& event );
		void OnUpdateUI( wxUpdateUIEvent& event );
		void OnViewMenu( wxCommandEvent& event );
		void OnOptionsFileMode( wxCommandEvent& event );
		void OnClose( wxCloseEvent& event );
		void OnQuit( wxCommandEvent& event );
		void OnAbout( wxCommandEvent& event );
		void OnActivate( wxActivateEvent& event );

		void ActionEnabler( void );
		void ActionDisabler( void );
		void OnNotebookTabSelection( wxAuiNotebookEvent& event );
		void OnNotebookTabClose( wxAuiNotebookEvent& event );

	private:
		void PrepareAUI( void );
		wxAuiNotebook *MyNotebook;
		wxAuiManager *MyAUI;
		DataInterpreter *MyInterpreter;
		InfoPanel *MyInfoPanel;
		wxAuiToolBar* Toolbar;
		friend class DnDFile;
	};

class DnDFile : public wxFileDropTarget{
	public:
		DnDFile( HexEditorFrame* myHexFramework) {
			HexFramework = myHexFramework;
			}
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
	private:
		wxAuiNotebook *m_pOwner;
		HexEditorFrame *HexFramework;
	};

class VersionChecker : public UpdateDialog_Gui {
	public:
		VersionChecker( wxString _url, wxString current_version, wxWindow *parent = NULL, wxWindowID id = 1  );
		void OnChkDisplay( wxCommandEvent& event );
	};

#endif