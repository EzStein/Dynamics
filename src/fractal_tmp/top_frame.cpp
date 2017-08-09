#include <iostream>
#include "top_frame.h"
#include "app_traits.h"
#include "render_panel.h"

top_frame::top_frame(const app_state * state) : wxFrame(NULL, wxID_ANY, APP_TRAITS_NAME), state(state) {
  wxWindowID id;

  wxMenu  *menuFile = new wxMenu();

  id = wxWindow::NewControlId();
  wxMenuItem *menuItemOpen = new wxMenuItem(NULL, id, "Open...");
  Bind(wxEVT_MENU, &top_frame::on_open_menu_click, this, id);
  menuFile->Append(menuItemOpen);

  id = wxWindow::NewControlId();
  wxMenuItem *menuItemClose = new wxMenuItem(NULL, id, "Close");
  Bind(wxEVT_MENU, &top_frame::on_close_menu_click, this, id);
  menuFile->Append(menuItemClose);

  id = wxWindow::NewControlId();
  wxMenuItem *menuItemSave = new wxMenuItem(NULL, id, "Save");
  Bind(wxEVT_MENU, &top_frame::on_save_menu_click, this, id);
  menuFile->Append(menuItemSave);

  id = wxWindow::NewControlId();
  wxMenuItem *menuItemSaveAs = new wxMenuItem(NULL, id, "Save As...");
  Bind(wxEVT_MENU, &top_frame::on_save_as_menu_click, this, id);
  menuFile->Append(menuItemSaveAs);

  id = wxWindow::NewControlId();
  wxMenuItem *menuItemQuit = new wxMenuItem(NULL, id, "Quit");
  Bind(wxEVT_MENU, &top_frame::on_quit_menu_click, this, id);
  menuFile->Append(menuItemQuit);

  wxMenu *menuEdit = new wxMenu();
  id = wxWindow::NewControlId();
  wxMenuItem *menuItemPreferences = new wxMenuItem(NULL, id, "Preferences");
  Bind(wxEVT_MENU, &top_frame::on_preferences_menu_click, this, id);
  menuEdit->Append(menuItemPreferences);

  wxMenu *menuInfo = new wxMenu();
  id = wxWindow::NewControlId();
  wxMenuItem *menuItemAbout = new wxMenuItem(NULL, id, "About");
  Bind(wxEVT_MENU, &top_frame::on_about_menu_click, this, id);
  menuInfo->Append(menuItemAbout);

  id = wxWindow::NewControlId();
  wxMenuItem *menuItemHelp = new wxMenuItem(NULL, id, "Help");
  Bind(wxEVT_MENU, &top_frame::on_help_menu_click, this, id);
  menuInfo->Append(menuItemHelp);

  wxMenuBar * menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "File");
  menuBar->Append(menuEdit, "Edit");
  menuBar->Append(menuInfo, "Info");

  SetMenuBar(menuBar);

  Bind(wxEVT_CLOSE_WINDOW, &top_frame::on_close, this);
  renderPanel = new render_panel(this, state);
  wxBoxSizer * sizerV = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer * sizerH = new wxBoxSizer(wxHORIZONTAL);
  sizerV->Add(renderPanel, 1, wxSHAPED | wxALL, 10);
  sizerV->Add(sizerH, 0, wxGROW | wxALL, 10);
  sizerH->Add(new wxButton(this, wxID_ANY, "OK"), 0, wxLEFT | wxRIGHT, 10);
  sizerH->Add(new wxButton(this, wxID_ANY, "CANCEL"), 1, wxLEFT | wxRIGHT, 10);
  SetSizerAndFit(sizerV);
}

void top_frame::on_close(wxCloseEvent& event) {
  event.Skip();
}

void top_frame::on_help_menu_click(wxCommandEvent&) {
  wxMessageDialog dialog(NULL, "You clicked the help menu item!", "Help", wxOK);
  dialog.ShowModal();
}
void top_frame::on_open_menu_click(wxCommandEvent&) {
  wxMessageDialog dialog(NULL, "You clicked the Open menu item!", "Open", wxOK);
  dialog.ShowModal();
}
void top_frame::on_close_menu_click(wxCommandEvent&) {
  wxMessageDialog dialog(NULL, "You clicked the Close menu item!", "Close", wxOK);
  dialog.ShowModal();
}
void top_frame::on_save_menu_click(wxCommandEvent&) {
  wxMessageDialog dialog(NULL, "You clicked the Save menu item!", "Save", wxOK);
  dialog.ShowModal();
}
void top_frame::on_save_as_menu_click(wxCommandEvent&) {
  wxMessageDialog dialog(NULL, "You clicked the Save As menu item!", "Save As", wxOK);
  dialog.ShowModal();
}
void top_frame::on_about_menu_click(wxCommandEvent&) {
  wxMessageDialog dialog(NULL, "You clicked the About menu item!",  "About", wxOK);
  dialog.ShowModal();
}
void top_frame::on_preferences_menu_click(wxCommandEvent&) {
  wxMessageDialog dialog(NULL, "You clicked the Preferences menu item!", "Preferences", wxOK);
  dialog.ShowModal();
}
void top_frame::on_quit_menu_click(wxCommandEvent&) {
  wxMessageDialog dialog(NULL, "You clicked the Quit menu item!", "Quit", wxOK);
  dialog.ShowModal();
}
