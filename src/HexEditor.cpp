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

#include "HexEditor.h"

HexEditor::HexEditor(	wxWindow* parent,
							int id,
							wxStatusBar *statbar_,
							DataInterpreter *interpreter_,
							InfoPanel *infopanel_,
							wxFileName* myfilename_,
							const wxPoint& pos,
							const wxSize& size,
							long style ):
			HexEditorCtrl(parent, id, pos, size, wxTAB_TRAVERSAL)
			,statusbar(statbar_),
			interpreter(interpreter_),
			infopanel(infopanel_){
	printf("Rahman ve Rahim olan Allah'ın adıyla.\n"); // Praying to GOD neccessary
	myfile = NULL;
	if( myfilename_ != NULL ){
		if( !FileOpen( *myfilename_ ) ){
			}
		}
	offset_scroll->Enable( true );
	Dynamic_Connector();
	copy_mark = new copy_maker( );
	myDialogVector = new DialogVector;
	myDialogVector->goto_hex=0;
	myDialogVector->goto_branch=0;
	myDialogVector->goto_input=0;
	}

HexEditor::~HexEditor(){
		//FileClose();
		Dynamic_Disconnector();
	}

void HexEditor::Dynamic_Connector(){
    hex_ctrl ->Connect( wxEVT_KEY_DOWN,	wxKeyEventHandler(HexEditor::OnKeyboardInput),NULL, this);
    text_ctrl->Connect( wxEVT_KEY_DOWN,	wxKeyEventHandler(HexEditor::OnKeyboardInput),NULL, this);
//	hex_ctrl ->Connect( wxEVT_KEY_UP,	wxKeyEventHandler(HexEditor::OnKeyboardSelectionEnd),NULL, this);
//	text_ctrl->Connect( wxEVT_KEY_UP,	wxKeyEventHandler(HexEditor::OnKeyboardSelectionEnd),NULL, this);
    hex_ctrl ->Connect( wxEVT_CHAR,		wxKeyEventHandler(HexEditor::OnKeyboardChar),NULL, this);
    text_ctrl->Connect( wxEVT_CHAR,		wxKeyEventHandler(HexEditor::OnKeyboardChar),NULL, this);
	hex_ctrl ->Connect( wxEVT_LEFT_DOWN,	wxMouseEventHandler(HexEditor::OnMouseLeft),NULL, this);
	text_ctrl->Connect( wxEVT_LEFT_DOWN,	wxMouseEventHandler(HexEditor::OnMouseLeft),NULL, this);
	hex_ctrl ->Connect( wxEVT_LEFT_UP,	wxMouseEventHandler(HexEditor::OnMouseSelectionEnd),NULL, this);
	text_ctrl->Connect( wxEVT_LEFT_UP,	wxMouseEventHandler(HexEditor::OnMouseSelectionEnd),NULL, this);
	hex_ctrl ->Connect( wxEVT_RIGHT_DOWN,wxMouseEventHandler(HexEditor::OnMouseRight),NULL, this);
	text_ctrl->Connect( wxEVT_RIGHT_DOWN,wxMouseEventHandler(HexEditor::OnMouseRight),NULL, this);
	hex_ctrl ->Connect( wxEVT_MIDDLE_DOWN,wxMouseEventHandler(HexEditor::OnMouseTest),NULL, this);
	text_ctrl->Connect( wxEVT_MIDDLE_DOWN,wxMouseEventHandler(HexEditor::OnMouseTest),NULL, this);
	hex_ctrl ->Connect( wxEVT_MOTION,	wxMouseEventHandler(HexEditor::OnMouseMove),NULL, this);
	text_ctrl->Connect( wxEVT_MOTION,	wxMouseEventHandler(HexEditor::OnMouseMove),NULL, this);
	hex_ctrl ->Connect( wxEVT_MOUSEWHEEL,wxMouseEventHandler(HexEditor::OnMouseWhell),NULL, this);
	text_ctrl->Connect( wxEVT_MOUSEWHEEL,wxMouseEventHandler(HexEditor::OnMouseWhell),NULL, this);
	}

void HexEditor::Dynamic_Disconnector(){
    hex_ctrl ->Disconnect( wxEVT_KEY_DOWN,	wxKeyEventHandler(HexEditor::OnKeyboardInput),NULL, this);
	text_ctrl->Disconnect( wxEVT_KEY_DOWN,	wxKeyEventHandler(HexEditor::OnKeyboardInput),NULL, this);
//	hex_ctrl ->Disconnect( wxEVT_KEY_UP,	wxKeyEventHandler(HexEditor::OnKeyboardSelectionEnd),NULL, this);
//	text_ctrl->Disconnect( wxEVT_KEY_UP,	wxKeyEventHandler(HexEditor::OnKeyboardSelectionEnd),NULL, this);
    hex_ctrl ->Disconnect( wxEVT_CHAR,		wxKeyEventHandler(HexEditor::OnKeyboardChar),NULL, this);
    text_ctrl->Disconnect( wxEVT_CHAR,		wxKeyEventHandler(HexEditor::OnKeyboardChar),NULL, this);
	hex_ctrl ->Disconnect( wxEVT_LEFT_DOWN,	wxMouseEventHandler(HexEditor::OnMouseLeft),NULL, this);
	text_ctrl->Disconnect( wxEVT_LEFT_DOWN,	wxMouseEventHandler(HexEditor::OnMouseLeft),NULL, this);
	hex_ctrl ->Disconnect( wxEVT_LEFT_UP,	wxMouseEventHandler(HexEditor::OnMouseSelectionEnd),NULL, this);
	text_ctrl->Disconnect( wxEVT_LEFT_UP,	wxMouseEventHandler(HexEditor::OnMouseSelectionEnd),NULL, this);
	hex_ctrl ->Disconnect( wxEVT_RIGHT_DOWN,wxMouseEventHandler(HexEditor::OnMouseRight),NULL, this);
	text_ctrl->Disconnect( wxEVT_RIGHT_DOWN,wxMouseEventHandler(HexEditor::OnMouseRight),NULL, this);
	hex_ctrl ->Disconnect( wxEVT_MIDDLE_DOWN,wxMouseEventHandler(HexEditor::OnMouseTest),NULL, this);
	text_ctrl->Disconnect( wxEVT_MIDDLE_DOWN,wxMouseEventHandler(HexEditor::OnMouseTest),NULL, this);
	hex_ctrl ->Disconnect( wxEVT_MOTION,	wxMouseEventHandler(HexEditor::OnMouseMove),NULL, this);
	text_ctrl->Disconnect( wxEVT_MOTION,	wxMouseEventHandler(HexEditor::OnMouseMove),NULL, this);
	}

bool HexEditor::FileOpen(wxFileName& myfilename ){
	if(myfile!=NULL){
		wxLogError(_("Critical Error. File pointer is not empty!"));
		return false;
		}
	else if(myfilename.IsFileReadable()){ //IsFileReadable
		if ( myfilename.GetSize( ) < 50*MB && myfilename.IsFileWritable() )
			myfile = new FileDifference( myfilename, FileDifference::ReadWrite );
		else
			myfile = new FileDifference( myfilename );

		if(myfile->IsOpened()){
			myscroll = new scrollthread(0,this);
//			copy_mark = new copy_maker();
			LoadTAGS( myfilename.GetFullPath().Append(wxT(".tags")) );
			LoadFromOffset(0, true);
			//offset_scroll->SetScrollbar(offset_scroll->GetThumbPosition(), 1, (FileLength() / ByteCapacity())+1, 1 );//Adjusting slider to page size
			SetLocalHexInsertionPoint(0);
			return true;
			}
		else{
			wxMessageBox(_("File cannot open."),_("Error"), wxOK|wxICON_ERROR);
			return false;
			}
		}
	else{
		wxMessageBox(_("File isn't Readable.(Permissions?)"),_("Error"), wxOK|wxICON_ERROR);
		return false;
		}
	}

bool HexEditor::FileSave( bool question ){
	if( myfile->IsChanged() ){
		int select;
		if ( myfile->GetAccessMode() == FileDifference::ReadOnly){
			wxMessageDialog msg( this, _( "File in Read Only mode. Cannot save file.\n"), _("File Save"), wxOK|wxICON_EXCLAMATION, wxDefaultPosition);
			msg.ShowModal();
			}
		if ( !question )
			select = wxID_YES;
		else{
			wxMessageDialog msg( this, _( "Do you want to save this file?\n"), _("File Save"), wxYES_NO|wxCANCEL|wxICON_QUESTION, wxDefaultPosition);
			select=msg.ShowModal();
			}
		switch( select ){
			case(wxID_YES):
				if( !myfile->Apply() ){
					wxMessageBox( _( "File cannot saved. Operation Cancelled\n"), _("File Save Error"), wxOK|wxICON_ERROR);
					return false;
					}
			case(wxID_NO):
				return true;
			case(wxID_CANCEL):
			default: return false;
			}
		}
	else{
		wxMessageDialog msg( this, _( "File is not changed. Nothing to save.\n"), _("File Save"), wxOK|wxICON_EXCLAMATION, wxDefaultPosition);
		msg.ShowModal();
		}
	return false;
	}

bool HexEditor::FileSave( wxString savefilename ){
// TODO (death#1#): Protection to save opened file/samefile
	wxFFile savefile( savefilename, _("w") );
	if(savefile.IsOpened()){
		myfile->Seek( 0, wxFromStart);
		char *buffer = new char[MB];
		while( savefile.Tell() < myfile->Length() ){
			savefile.Write( buffer, myfile->Read( buffer, MB ) );
	// TODO (death#3#): Progressbar here?
			}
		return true;
		}
	else
		return false;
	}

bool HexEditor::FileClose( void ){
	if( myfile != NULL ){
		if( myfile->IsChanged() ){
			wxMessageDialog msg( this, _( "Do you want to save file?\n"), _("File Has Changed!"), wxYES_NO|wxCANCEL|wxICON_QUESTION, wxDefaultPosition);
			int state = msg.ShowModal();
			switch(state){
				case(wxID_YES):
					if( !FileSave( false ) )
						return false;
					break;
				case(wxID_NO):
					break;
				case(wxID_CANCEL):
					return false;
				}
			}
		SaveTAGS( myfile->GetFileName() );
		//myscroll->GetMyThread()->Delete();
//		myscroll->GetMyThread()->Wait();
//		delete myscroll;
//		delete copy_mark;
		Clear( true );
		myfile->Close();
		delete myfile;
		myfile = NULL;
		}
	return true;
	}

bool HexEditor::DoUndo( void ){
	Goto( myfile->Undo() );
#if defined(_DEBUG_) && _DEBUG_ > 3
	std::cout << "Send UnReDo Event" << std::endl;
#endif
	wxUpdateUIEvent eventx;
	eventx.SetId( UNREDO_EVENT );
	GetEventHandler()->ProcessEvent( eventx );
	}

bool HexEditor::DoRedo( void ){
	Goto( myfile->Redo() );
#if defined(_DEBUG_) && _DEBUG_ > 3
	std::cout << "Send UnReDo Event" << std::endl;
#endif
	wxUpdateUIEvent eventx;
	eventx.SetId( UNREDO_EVENT );
	GetEventHandler()->ProcessEvent( eventx );
	}

void HexEditor::Goto( int64_t cursor_offset ){
	if( cursor_offset == -1 ){
		LoadFromOffset( page_offset, false, true );	//Refresh
		return;
		}
	if(page_offset <= cursor_offset &&
		page_offset+ByteCapacity() >= cursor_offset){	//cursor_offset is in visible area
		Reload();					//Reload data needed for undo - redo
		SetLocalHexInsertionPoint( (cursor_offset - page_offset)*2 );
		}
	else{// out of view
		page_offset = cursor_offset;
		page_offset -= static_cast<int64_t>( ByteCapacity() * 0.20 ); // load some bytes behind of request for make lines at top side
		page_offset -= page_offset % BytePerLine();	//to allign offset
		if(page_offset < 0)
			page_offset = 0;
		else if(page_offset > FileLength() )
			page_offset = FileLength() - ByteCapacity() + 2*BytePerLine();
		Reload();
		SetLocalHexInsertionPoint( (cursor_offset - page_offset)*2 );
		}
//	UpdateCursorLocation();
	UpdateOffsetScroll();
	}

void HexEditor::UpdateOffsetScroll( void ){
	if( offset_scroll->GetRange() != (myfile->Length() / ByteCapacity()) ||
		offset_scroll->GetThumbPosition() != page_offset / ByteCapacity() )
		offset_scroll->SetScrollbar(page_offset / ByteCapacity(), 1, (FileLength() / ByteCapacity())+1, 1 );//Adjusting slider to page size
		}
void HexEditor::OnOffsetScroll( wxScrollEvent& event ){
	std::cout << event.GetEventType() << std::endl;
	if((event.GetEventType() == wxEVT_SCROLL_CHANGED) or (event.GetEventType() == wxEVT_SCROLL_THUMBTRACK)){
		LoadFromOffset( static_cast<int64_t>(offset_scroll->GetThumbPosition()) * ByteCapacity() );
		UpdateCursorLocation();
	#if wxUSE_STATUSBAR
		if( statusbar != NULL )
			statusbar->SetStatusText(wxString::Format(_("Showing Page: %d"), page_offset/ByteCapacity() ), 0);
	#endif
		wxYieldIfNeeded();
		}
	}

void HexEditor::LoadFromOffset(int64_t position, bool cursor_reset, bool paint){
	#ifdef _DEBUG_
		std::cout << "LoadFromOffset() : " << position << std::endl;
	#endif
    myfile->Seek(position, wxFromStart);
	char *buffer = new char[ ByteCapacity() ];
	int readedbytes = myfile->Read(buffer, ByteCapacity());
	ReadFromBuffer( position, readedbytes, buffer, cursor_reset, paint );
	delete buffer;
	}

void HexEditor::Reload( void ){
    myfile->Seek(page_offset, wxFromStart);
	char *buffer = new char[ ByteCapacity() ];
	int readedbytes = myfile->Read(buffer, ByteCapacity());
	ReadFromBuffer( page_offset, readedbytes, buffer, false, true );
	delete buffer;
	}

void HexEditor::OnResize( wxSizeEvent &event){
	std::cout << "HexEditor::OnResize() Event Type:" << event.GetEventType() << std::endl ;
	#ifdef _DEBUG_
		std::cout << "ByteCapacity() Before:" << ByteCapacity() << std::endl;
	#endif
	HexEditorCtrl::OnResize( event );
	//event.Skip( true );
	if(myfile != NULL && 0 < ByteCapacity()){
		offset_scroll->SetScrollbar(page_offset / ByteCapacity(),
									1,
									1+(myfile->Length() / ByteCapacity()),
									1,true
									);
		Reload();
		}
    }

bool HexEditor::FileAddDiff( int64_t start_byte, const char* data, int64_t size, bool extension ){
	myfile->Add( start_byte, data, size, extension );
// TODO (death#1#): Disabling undo - redo buttons
//	if(myfile->IsAvailable_Undo())
//		Toolbar->EnableTool( wxID_UNDO, true);
//		mbar->EnableTool( wxID_UNDO, true);
	}

void HexEditor::OnKeyboardSelector(wxKeyEvent& event){
	if(! event.ShiftDown() ){
		if( select->IsState( select->SELECT_TRUE ) )
			select->SetState( select->SELECT_END );
		}
    else
		Selector();
	}

// TODO (death#1#): BUG: Remove Text Selection when UNDO (CTRL+SHIFT)
// TODO (death#1#): BUG: Hex-Text Selection at release shift action
// TODO (death#5#): File Name star'in * when file changed & saved
// TODO (death#1#): BUG: Remove Text Selection when UNDO (CTRL+SHIFT)
// TODO (death#1#): BUG: Hex-Text Selection at release shift action
// TODO (death#5#): File Name star'in * when file changed & saved

void HexEditor::OnKeyboardInput( wxKeyEvent& event ){
	if(myfile != NULL){
		wxHexCtrl *myctrl = static_cast<wxHexCtrl*>(event.GetEventObject());
		//Keyboard Selection Code
		if(	event.GetKeyCode()==WXK_UP || event.GetKeyCode()==WXK_NUMPAD_UP ||
			event.GetKeyCode()==WXK_DOWN || event.GetKeyCode()==WXK_NUMPAD_DOWN ||
			event.GetKeyCode()==WXK_LEFT || event.GetKeyCode()==WXK_NUMPAD_LEFT ||
			event.GetKeyCode()==WXK_RIGHT || event.GetKeyCode()==WXK_NUMPAD_RIGHT ||
			event.GetKeyCode()==WXK_HOME || event.GetKeyCode()==WXK_NUMPAD_HOME ||
			event.GetKeyCode()==WXK_END || event.GetKeyCode()==WXK_NUMPAD_END ||
			event.GetKeyCode()==WXK_PAGEUP || event.GetKeyCode()==WXK_NUMPAD_PAGEUP ||
			event.GetKeyCode()==WXK_PAGEDOWN || event.GetKeyCode()==WXK_NUMPAD_PAGEDOWN
			)
			OnKeyboardSelector(event);	//Selection Starter call

	#ifdef _DEBUG_
		std::cout << "Received KeyCode : " << std::hex << event.GetKeyCode() << std::endl;
	#endif

		switch (event.GetKeyCode()){
			case (WXK_UP):case (WXK_NUMPAD_UP):
				if (ActiveLine() == 1){	//If cursor at first line
					if( page_offset == 0 )
						wxBell();					// there is no line over up!
					else{							// Illusion code
						LoadFromOffset( page_offset - BytePerLine() );	//offset decreasing one line & update text with new location, makes screen slide illusion
						UpdateCursorLocation();
						}
					}
					else
						SetLocalHexInsertionPoint( GetLocalHexInsertionPoint() - HexPerLine() );
					break;

				case (WXK_DOWN):case (WXK_NUMPAD_DOWN):
					if ( ActiveLine() == LineCount() ){			//If cursor at bottom of screen
						if(page_offset + ByteCapacity() < myfile->Length() ){//detects if another line is present or not
							int temp = GetLocalHexInsertionPoint();	//preserving cursor location
							LoadFromOffset( page_offset + BytePerLine() );	//offset increasing one line and update text with new location, makes screen slide illusion
							SetLocalHexInsertionPoint(temp);			//restoring cursor location
							}
						else{
							wxBell();											//there is no line to slide bell
							}
						}
					else if( ActiveLine() * HexPerLine() > GetLastPosition() )
						wxBell();	//If cursor at last line but not at bottom of screen
					else
						SetLocalHexInsertionPoint( GetLocalHexInsertionPoint() + HexPerLine() );
					break;
// TODO (death#1#): bug in left & text ctrl
				case (WXK_LEFT):case (WXK_NUMPAD_LEFT):
					if( GetLocalHexInsertionPoint() == 0){
						if(page_offset == 0)
							wxBell();
						else{
							LoadFromOffset( page_offset - BytePerLine() );
							SetLocalHexInsertionPoint( HexPerLine() - 1 );
							}
						}
					else
						SetLocalHexInsertionPoint( GetLocalHexInsertionPoint() - ((myctrl == hex_ctrl) ? 1 : 2) );
					break;
				case (WXK_RIGHT):case (WXK_NUMPAD_RIGHT):
					if( myctrl->GetInsertionPoint() >= myctrl->GetLastPosition() ){
						if(page_offset + ByteCapacity() < myfile->Length() ){	//Checks if its EOF or not
							LoadFromOffset( page_offset + BytePerLine() );
							SetLocalHexInsertionPoint( (LineCount() - 1) * HexPerLine() );
							}
						else
							wxBell();
						}
					else
						SetLocalHexInsertionPoint( GetLocalHexInsertionPoint() +  ((myctrl == hex_ctrl)? 1 : 2) );
					break;

				case (WXK_HOME):case(WXK_NUMPAD_HOME):{
					SetLocalHexInsertionPoint( GetLocalHexInsertionPoint() - GetLocalHexInsertionPoint() % HexPerLine() );
					break;
					}
				case (WXK_END):case(WXK_NUMPAD_END):{
					SetLocalHexInsertionPoint( GetLocalHexInsertionPoint() + HexPerLine() - (GetLocalHexInsertionPoint() % HexPerLine()) - 1  );
					break;
					}
				case (WXK_PAGEUP):case (WXK_NUMPAD_PAGEUP):
						if(page_offset - ByteCapacity() > 0){
//							int temp = GetHexInsertionPoint();
							LoadFromOffset(page_offset -  ByteCapacity());
							UpdateCursorLocation();
//							SetLocalHexInsertionPoint(temp);
							}
						else{
							int temp = GetLocalHexInsertionPoint() % HexPerLine();
							LoadFromOffset( 0 );
							SetLocalHexInsertionPoint(temp);
							wxBell();
							}
					break;
// TODO (death#5#): Add last byte problem. Also text ctrl has +1 issue
				case (WXK_PAGEDOWN):case (WXK_NUMPAD_PAGEDOWN):
						if(page_offset + ByteCapacity()*2 < myfile->Length()){ //*2 for cosmetic
							int temp = GetLocalHexInsertionPoint();
							LoadFromOffset( page_offset +  ByteCapacity() );
							SetLocalHexInsertionPoint( temp );
							}
						else{
							int temp = ( GetLocalHexInsertionPoint() %  HexPerLine() ) + ( LineCount()-1 ) * HexPerLine();
							page_offset = myfile->Length() - ByteCapacity();
							page_offset += BytePerLine() - page_offset % BytePerLine(); //cosmetic
							Reload();
							SetLocalHexInsertionPoint(temp);
							wxBell();
							}
					break;

				case (WXK_DELETE):case (WXK_NUMPAD_DELETE):
					/*if( myctrl->GetInsertionPoint() != myctrl->GetLastPosition() ){
						int hex_loc = myctrl->GetInsertionPoint();
						HexCharReplace(hex_loc,'0');
						myctrl->SetInsertionPoint(hex_loc);
						}
					else
					*/
						wxBell();
					break;

				case (WXK_BACK):
					/*
					if( myctrl->GetInsertionPoint()!=0 ){
						HexCharReplace(myctrl->GetInsertionPoint()-1,'0');
						myctrl->SetInsertionPoint(myctrl->GetInsertionPoint()-1);
						}
					else
						if(page_offset == 0)
							wxBell();
						else{	//Has to be a line over up
		// TODO (death#3#): if BytePerLine() changes, current offset gona be mad, like taking minus argument because it thinks a compleete line over up... spend caution about it.
							page_offset -= myctrl->BytePerLine();
							LoadFromOffset( page_offset );
							HexCharReplace(myctrl->HexPerLine()-1,'0');
							myctrl->SetInsertionPoint(myctrl->HexPerLine()-1);
							}
					*/
					break;

				default:{
						if( event.ControlDown() )
							switch( event.GetKeyCode() ){
								case( 0x5a ):		// CTRL+Z = UNDO
									if(event.ShiftDown())
										DoRedo();	// UNDO with shift = REDO
									else
										DoUndo();
									break;
								case( 0x59 ):		// CTRL+Y = REDO
									DoRedo();
									break;
								case( 0x53 ):{	// CTRL+S = SAVE
									FileSave();
									// TODO (death#1#): File Name star'in * when file changed & saved
									}
									break;
								case( 0x41 ):		// CTRL+A = ALL
									Select(0, FileLength());
									break;
								case( 0x58 ):		// CTRL+X = CUT
									wxBell();
									break;
								case( 0x43 ):		// CTRL+C = COPY
									CopySelection();
									break;
								case( 0x56 ):		// CTRL+V = PASTE
									PasteFromClipboard();
									break;
								case( 0x46 ):		// CTRL+F = FIND
									//finddlg();
									break;
								case( 0x4f ):		// CTRL+O = OPEN
									wxBell();
									break;
								default:
									event.Skip();// ->OnKeyboardChar( event );
									break;
								}
						else
							event.Skip();// ->OnKeyboardChar( event );
					}


				}//switch end
			UpdateOffsetScroll();
			//OnKeyboardSelector(event);
			PaintSelection( );
		}
	}

void HexEditor::OnKeyboardChar( wxKeyEvent& event ){
	if(myfile != NULL){
		wxChar chr = event.GetKeyCode();
		/*
		void *myctrl = FindFocus();		//avoid reinterpret_cast for speed
		if( myctrl == text_ctrl )
			myctrl = text_ctrl;
		else if ( myctrl == hex_ctrl )
			myctrl = hex_ctrl;
		else
			{
			wxBell();
			return;
			}
		*/

		if( event.GetEventObject() == hex_ctrl )
			if( isxdigit(chr) ){
				(chr>='a'&&chr<='f')?(chr-=('a'-'A')):(chr=chr);		// Upper() for Char
				HexCharReplace( GetLocalHexInsertionPoint(), chr);		// write to screen

				char rdchr = hex_ctrl->ReadByte( GetLocalHexInsertionPoint()/2 );	// read from screen
				FileAddDiff( CursorOffset(), &rdchr ,1);				// add node to file

				if( hex_ctrl->GetInsertionPoint() >= hex_ctrl->GetLastPosition() ){
					if( CursorOffset() + ByteCapacity() <= myfile->Length() ){	//Checks if its EOF or not
						LoadFromOffset( page_offset + BytePerLine() );
						hex_ctrl->LastLine();
						}
					else
						wxBell();
					}
                else
                	hex_ctrl->NextChar();
				}
			else
				wxBell();
		else if( event.GetEventObject() == text_ctrl ){
			if( wxString((wxChar)event.GetKeyCode()).IsAscii() &&
				event.GetKeyCode()!=WXK_BACK &&
				event.GetKeyCode()!=WXK_DELETE &&
				!event.ControlDown()
				){

/*				if( text_ctrl->GetInsertionPoint() >= text_ctrl->GetLastPosition()){
					if(current_offset+hex_ctrl->ByteCapacity() <= myTempFile->Length() ){	//Checks if its EOF or not
						current_offset += hex_ctrl->BytePerLine();
						LoadFromOffset(current_offset);
						text_ctrl->LastLine();	// HexPerLine = BytePerLine()*2
						}
					}
*/
				int GLIP = GetLocalInsertionPoint();	//this required because TextCharReplace() calls HexCtrl->Replace that alters entering point
				TextCharReplace(GLIP, event.GetKeyCode());
				char rdchr = hex_ctrl->ReadByte( GLIP );	// read from screen
				FileAddDiff( GLIP + page_offset, &rdchr ,1);						// add node to file

				if( text_ctrl->GetInsertionPoint() >= text_ctrl->GetLastPosition() ){
					if( page_offset + ByteCapacity() <= myfile->Length() ){	//Checks if its EOF or not
						LoadFromOffset(page_offset + BytePerLine());
						text_ctrl->LastLine();
						}
					else
						wxBell();
					}
                else
                	text_ctrl->NextChar();
				}
			else
				wxBell();
			}
		}
#if defined(_DEBUG_) && _DEBUG_ > 3
	std::cout << "Send UnReDo Event" << std::endl;
#endif
	wxUpdateUIEvent eventx;
	eventx.SetId( UNREDO_EVENT );
	GetEventHandler()->ProcessEvent( eventx );
	}

void HexEditor::SetLocalHexInsertionPoint( int hex_location ){
	HexEditorCtrl::SetLocalHexInsertionPoint( hex_location );
	UpdateCursorLocation();
	}

void HexEditor::OnMouseLeft(wxMouseEvent& event){
	HexEditorCtrl::OnMouseLeft( event );
	#if wxUSE_STATUSBAR
    if ( statusbar ){
		statusbar->SetStatusText(_("Selected Block: N/A"), 3);
		statusbar->SetStatusText(_("Block Size: N/A") ,4);
		}
	#endif // wxUSE_STATUSBAR
	UpdateCursorLocation();
	std::cout << "CaptureMouse()\n" ;
	if(FindFocus() == hex_ctrl )
		hex_ctrl->CaptureMouse();
	else if( FindFocus() == text_ctrl )
		text_ctrl->CaptureMouse();
	MouseCapture = true;
	}

void HexEditor::OnMouseRight( wxMouseEvent& event ){
	if(event.GetEventObject() == hex_ctrl)
		LastRightClickAt = page_offset + hex_ctrl->PixelCoordToInternalPosition( event.GetPosition() )/2;
	else if(event.GetEventObject() == text_ctrl)
		LastRightClickAt = page_offset + text_ctrl->PixelCoordToInternalPosition( event.GetPosition() );
	else
		std::cout << "Right click captured without ctrl!\n";
	ShowContextMenu( event );
	event.Skip(false);
	}


void HexEditor::ShowContextMenu( const wxMouseEvent& event ){
	wxMenu menu;
	unsigned TagPosition=0;
	if( event.GetEventObject() == hex_ctrl )
		TagPosition = page_offset + (hex_ctrl->PixelCoordToInternalPosition( event.GetPosition() ) / 2);
	else if( event.GetEventObject() == text_ctrl )
		TagPosition = page_offset + text_ctrl->PixelCoordToInternalPosition( event.GetPosition() );

	menu.Append(idTagEdit, _T("Tag Edit"));
	menu.Append(idTagSelection, _T("New Tag"));
	menu.Append(wxID_COPY, _T("Copy"));
	menu.Append(wxID_CUT, _T("Cut"));
	menu.Append(wxID_PASTE, _T("Paste"));

	menu.Enable( idTagEdit, false );
	for( unsigned i = 0 ; i < MainTagArray.Count() ; i++ ){
		TagElement *TAG = MainTagArray.Item(i);
		if( TAG->isCover(TagPosition) ){
			menu.Enable( idTagEdit, true );
			break;
			}
		}
	menu.Enable( idTagSelection, select->IsState( select->SELECT_END) );
	menu.Enable( wxID_CUT, false );
	menu.Enable( wxID_COPY, select->IsState( select->SELECT_END) );
	//menu.AppendSeparator();
	wxPoint pos = event.GetPosition();
	wxWindow *scr = static_cast<wxWindow*>( event.GetEventObject() );
	pos += scr->GetPosition();
    PopupMenu(&menu, pos);
    // test for destroying items in popup menus
#if 0 // doesn't work in wxGTK!
// TODO (death#1#): inspect here
    PopupMenu( &menu, event.GetX(), event.GetY() );
#endif // 0
	}

void HexEditor::OnMouseWhell( wxMouseEvent& event ){
#ifdef _DEBUG_
	std::cout << "MouseWhell Rotation = " << event.GetWheelRotation() << "\t Delta = " << event.GetWheelDelta()
			<< "\tLinesPerAction: " << event.GetLinesPerAction() << std::endl;

#endif
	if(event.GetWheelRotation() > 0){		// Going to UP
		page_offset -= BytePerLine()*event.GetLinesPerAction(); 	//offset decreasing
		if( page_offset <= 0 ){
			page_offset = 0;
			wxBell();					// there is no line over up!
			}
		Reload();	//update text with new location, makes screen slide illusion
		if( page_offset != 0 ){
			if( ActiveLine() + event.GetLinesPerAction() <= LineCount() )	//cursor at bottom
				SetLocalHexInsertionPoint( GetLocalHexInsertionPoint() + HexPerLine() * event.GetLinesPerAction() );	//restoring cursor location
			else
				SetLocalHexInsertionPoint( GetLocalHexInsertionPoint()%HexPerLine() + HexPerLine()*(LineCount()-1) );
			}
		}
	else if(event.GetWheelRotation() < 0 ){	// Going to BOTTOM
		if(page_offset + ByteCapacity() < myfile->Length() ){//detects if another line is present or not
			LoadFromOffset( page_offset + BytePerLine() * event.GetLinesPerAction());	//offset increasing line as mouse whell rolls
			if( ActiveLine() > event.GetLinesPerAction() )	//cursor at top GetLinesPerAction
				SetLocalHexInsertionPoint( GetLocalHexInsertionPoint() - HexPerLine() * event.GetLinesPerAction() );	//restoring cursor location
			else
				SetLocalHexInsertionPoint( GetLocalHexInsertionPoint()%HexPerLine() );
			}
		else{
			wxBell();							//there is no line to slide bell
			}
		}
	UpdateOffsetScroll();
	}

void HexEditor::OnMouseMove( wxMouseEvent& event ){
#if defined(_DEBUG_) && _DEBUG_ > 7
	std::cout << "HexEditor::OnMouseMove Coordinate X:Y = " << event.GetX()	<< " " << event.GetY()
			<< "\tLeft mouse button:" << event.LeftIsDown() << std::endl;
#endif
	if( event.LeftIsDown() ){
		int spd=0;
		if( event.GetY() < 0 and page_offset != 0){
			spd = static_cast<int>(0 - pow(2, abs(event.GetY()) / 25));
			(spd < -1024) ? (spd = -1024):(spd=spd);
			}
		else if(event.GetY()> hex_ctrl->GetSize().GetHeight() and page_offset + ByteCapacity() < myfile->Length()){
			int pointer_diff = event.GetY() - hex_ctrl->GetSize().GetHeight();
			spd = static_cast<int>(pow(2, pointer_diff / 25));
			(spd > 1024) ? (spd = 1024):(spd=spd);
			}
#if defined( __WXMAC__ ) || defined ( __WXMSW__ )
		ScrollNoThread( spd );			//MAC has problem with GuiMutex so useing safe scroll funtion
	#if defined(_DEBUG_) && _DEBUG_ > 4
		std::cout << "Scroll (Non-Thread) Speed = " << spd << std::endl;
	#endif
#else
		myscroll->UpdateSpeed(spd);
	#if defined(_DEBUG_) && _DEBUG_ > 4
		std::cout << "Scroll (Thread) Speed = " << spd << std::endl;
	#endif
#endif
		HexEditorCtrl::OnMouseMove( event );//Also makes selection in it
		UpdateCursorLocation();
		}
	else
		event.Skip(); //need for tag views
	}

void HexEditor::ScrollNoThread( int speed ){
	while( (!wxTheApp->Pending() and speed != 0 )
			and ( ((speed > 0) and (page_offset + ByteCapacity() < FileLength()))
				or ( (speed < 0) and (page_offset > 0) ))
			){
#if defined(_DEBUG_) && _DEBUG_ > 2
		std::cout << "Loop Scroll speed  :" << speed << std::endl;
		std::cout << "Loop Pending Event :" << wxTheApp->Pending() << std::endl;
#endif
		page_offset += BytePerLine() * speed;
		if( page_offset < 0 )
			page_offset = 0;
		else if( page_offset + ByteCapacity() >= FileLength() ){
			page_offset = FileLength() - ByteCapacity();
			page_offset += BytePerLine() - (page_offset % BytePerLine()) ; //cosmetic
			}
		LoadFromOffset( page_offset, false, false );
//		SetLocalHexInsertionPoint(cursor);
		Selector();
		PaintSelection();
		UpdateCursorLocation( true );
		if( offset_scroll->GetThumbPosition() != page_offset / ByteCapacity() )
			offset_scroll->SetThumbPosition( page_offset / ByteCapacity() );

		if( page_offset == 0 || page_offset + ByteCapacity() >= FileLength() )
			break;
		}
	}

void HexEditor::OnMouseSelectionEnd( wxMouseEvent& event ){
	HexEditorCtrl::OnMouseSelectionEnd( event );
	myscroll->UpdateSpeed( 0 );
	std::cout << "ReleaseMouse()\n" ; ReleaseMouse();
	MouseCapture = false;
	}

void HexEditor::UpdateCursorLocation( bool force ){
	static wxMutex update;

//	static int64_t lastPoint=-1;				//? Speed up Van goh
//	if( !force )
//		if( lastPoint == CursorOffset() )
//			return;
//	lastPoint = CursorOffset();
#if defined(_DEBUG_) && _DEBUG_ > 5
	std::cout << "mutex Update Locking..." << std::endl;
#endif
	if( update.TryLock()==wxMUTEX_NO_ERROR ){
		if( GetLocalHexInsertionPoint()/2+page_offset > FileLength() ){
			SetLocalHexInsertionPoint( (FileLength() - page_offset)*2 - 1 );
			}

		if( interpreter != NULL ){
			myfile->Seek( GetLocalHexInsertionPoint()/2+page_offset, wxFromStart );
//			char *bfr = new char [8];
//			int size=myfile->Read( bfr, 8);
//			interpreter->Set( bfr, size);
//			delete bfr;
			wxMemoryBuffer bfr;
			int size = myfile->Read( reinterpret_cast<char*>(bfr.GetWriteBuf( 8 )), 8);
			bfr.UngetWriteBuf( size );
			interpreter->Set( bfr );
			}
	#if wxUSE_STATUSBAR
		if( statusbar != NULL ){
			statusbar->SetStatusText(wxString::Format(_("Showing Page: %d"), page_offset/ByteCapacity() ), 0);
			if( offset_ctrl->hex_offset )
				statusbar->SetStatusText(wxString::Format(_("Cursor Offset: 0x%llX"), CursorOffset() ), 1);
			else
				statusbar->SetStatusText(wxString::Format(_("Cursor Offset: %lld"), CursorOffset() ), 1);
			uint8_t ch;
			myfile->Seek( CursorOffset() );
			myfile->Read( reinterpret_cast<char*>(&ch), 1);
			statusbar->SetStatusText(wxString::Format(_("Cursor Value: %u"), ch), 2);

			int start = select->StartOffset;
			int end = select->EndOffset;
			if(start > end ){
				int temp = start;
				start = end;
				end = temp;
				}
			if( select->IsState( select->SELECT_FALSE ) ){
				statusbar->SetStatusText(_("Selected Block: N/A"), 3);
				statusbar->SetStatusText(_("Block Size: N/A") ,4);
				}
			else{
				statusbar->SetStatusText(wxString::Format(_("Selected Block: %d -> %d"),start,end), 3);
				statusbar->SetStatusText(wxString::Format(_("Block Size: %d"), abs(start-end)+1), 4);
				}
			}
	#endif // wxUSE_STATUSBAR
	#if defined(_DEBUG_) && _DEBUG_ > 5
		std::cout << "mutex Update UnLocking..." << std::endl;
	#endif
		update.Unlock();
		}
	else
		std::cout << "mutex update cannot lock..." << std::endl;

	}

void HexEditor::OnMouseTest( wxMouseEvent& event ){
	myfile->ShowDebugState();
	SaveTAGS( myfile->GetFileName() );

			std::cout << "Send UpdateUI Event" << std::endl;
			wxUpdateUIEvent eventx;
//			event.SetEventObject( this );
			eventx.SetId( SELECT_EVENT );//idFileRO
			GetEventHandler()->ProcessEvent( eventx );
	}

void HexEditor::FindDialog( void ){
	class FindDialog *myfind = new FindDialog::FindDialog( this, myfile );
	myfind->ShowModal();
	myfind->Destroy();
	}

void HexEditor::ReplaceDialog( void ){
	class ReplaceDialog *myfind = new ReplaceDialog::ReplaceDialog( this, myfile );
	myfind->ShowModal();
	myfind->Destroy();
	}

void HexEditor::GotoDialog( void ){
	uint64_t newoffset;
	class GotoDialog *mygoto = new GotoDialog::GotoDialog( this, newoffset, CursorOffset(), FileLength(), myDialogVector );
	if( mygoto->ShowModal() == wxID_OK ){
		Goto( newoffset );
		}
	}

bool HexEditor::CopySelection( ){
	if( not select->IsState( select->SELECT_FALSE )){
		uint64_t start = select->StartOffset;
		uint64_t size = select->GetSize();
		uint64_t RAM_limit = 10*MB;
		if(size < RAM_limit){								//copy to clipboard if < 10 MB
			myfile->Seek( start, wxFromStart );
			void* buff=NULL;
			buff = copy_mark->m_buffer.GetWriteBuf( size );
			if( buff != NULL ){
				myfile->Read( static_cast< char*>( buff ), size );
				copy_mark->m_buffer.UngetWriteBuf( size );
				wxString CopyString;
				if( focus == HEX_CTRL ){
					for( unsigned i=0 ; i<size ; i++ )
						CopyString << wxString::Format(wxT("%02X "),static_cast<unsigned char>(copy_mark->m_buffer[i]));
					CopyString.Trim();	//remove last ' '
					}
				else if( focus == TEXT_CTRL )
					{
					copy_mark->m_buffer.AppendByte('\0');
					CopyString << wxString::FromAscii( static_cast<const char*>(copy_mark->m_buffer.GetData()) );
					}
				return copy_mark->SetClipboardData( CopyString );
				}
			else{
				wxMessageBox(_( "You have no RAM to copy this data.\nOperation cancelled!"), _("Copy To Clipboard Error"), wxOK|wxICON_ERROR);
				return false;
				}
			}
		else{
			wxMessageBox(_( "You are tried to copy data more than 10 MB.\n"\
							"Copying above 10 MB to clipboard is not allowed.\n"\
							"Only internal copy buffer used!"),
							_("Info"), wxOK|wxICON_INFORMATION);

			void* buff=NULL;
			buff = copy_mark->m_buffer.GetWriteBuf( size );
			if( buff != NULL ){
				myfile->Seek( start , wxFromStart );
				myfile->Read( static_cast<char*>(buff), size);
				copy_mark->m_buffer.UngetWriteBuf( size );
				return true;
				}
			else{
// TODO (death#1#): If there is no ram, use HDD temp file
				return false;
				}
			}
		}
	else{
		wxBell();
		return false;
		}
	}

bool HexEditor::PasteFromClipboard( void ){
	if( focus==HEX_CTRL ){
		wxString str = copy_mark->GetClipboardData();
		if( ! str.IsEmpty() ){
			wxMemoryBuffer mymem = wxHexCtrl::HexToBin( str );
			FileAddDiff( CursorOffset(), static_cast<char*>(mymem.GetData()), mymem.GetDataLen() );
			select->SetState( select->SELECT_FALSE );
			Goto( CursorOffset() + str.Len() );
			}
		}
	else if ( focus==TEXT_CTRL ){
		wxString str = copy_mark->GetClipboardData();
		if( ! str.IsEmpty() ){
			char *ch = new char [str.Len()];
			for( unsigned i=0;i<str.Len();i++ )
				ch[i] = str[i];
			FileAddDiff( CursorOffset(), ch, str.Len() );
			select->SetState( select->SELECT_FALSE );
			Goto( CursorOffset() + str.Len() );
			}
		}
	else
		wxMessageBox(_( "There is no focus!"), _("Paste Error"), wxOK|wxICON_ERROR);

	#if defined(_DEBUG_) && _DEBUG_ > 3
		std::cout << "Send UnReDo Event" << std::endl;
	#endif
	wxUpdateUIEvent eventx;
	eventx.SetId( UNREDO_EVENT );
	GetEventHandler()->ProcessEvent( eventx );
	}