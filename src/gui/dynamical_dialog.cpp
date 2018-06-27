#include "gui/dynamical_dialog.h"

#include <cassert>

namespace dynsolver {
namespace gui {

dynamical_dialog::dynamical_dialog() :
  dynamical_dialog_base(nullptr) {
  dynamical2dPropertyGrid->Append
    (new wxPropertyCategory( wxT("Horizontal Axis"), wxT("Horizontal Axis") ) ); 
  horizontalAxisVariablePropertyGridItem = dynamical2dPropertyGrid->
    Append( new wxEnumProperty( wxT("Variable"), wxT("VariableH") ) ); 
  horizontalAxisMinPropertyGridItem = dynamical2dPropertyGrid->
    Append( new wxFloatProperty( wxT("Minimum"), wxT("MinimumH") ) ); 
  horizontalAxisMaxPropertyGridItem = dynamical2dPropertyGrid->
    Append( new wxFloatProperty( wxT("Maximum"), wxT("MaximumH") ) ); 
  dynamical2dPropertyGrid->
    Append( new wxPropertyCategory( wxT("Vertical Axis"), wxT("Vertical Axis") ) ); 
  verticalAxisVariablePropertyGridItem = dynamical2dPropertyGrid->
    Append( new wxEnumProperty( wxT("Variable"), wxT("VariableV") ) ); 
  verticalAxisMinPropertyGridItem = dynamical2dPropertyGrid->
    Append( new wxFloatProperty( wxT("Minimum"), wxT("MinimumV") ) ); 
  verticalAxisMaxPropertyGridItem = dynamical2dPropertyGrid->
    Append( new wxFloatProperty( wxT("Maximum"), wxT("MaximumV") ) );

  dynamical3dPropertyGrid->
    Append(new wxPropertyCategory( wxT("Camera Position"), wxT("Camera Position") ) ); 
  cameraPositionXPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("X"), wxT("XCP") ) ); 
  cameraPositionYPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("Y"), wxT("YCP") ) ); 
  cameraPositionZPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("Z"), wxT("ZCP") ) ); 
  dynamical3dPropertyGrid->
    Append( new wxPropertyCategory( wxT("Camera Direction"), wxT("Camera Direction") ) ); 
  cameraDirectionXPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("X"), wxT("XCD") ) ); 
  cameraDirectionYPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("Y"), wxT("YCD") ) ); 
  cameraDirectionZPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("Z"), wxT("ZCD") ) ); 
  dynamical3dPropertyGrid->
    Append( new wxPropertyCategory( wxT("Up Direction"), wxT("Up Direction") ) ); 
  upDirectionXPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("X"), wxT("XUD") ) ); 
  upDirectionYPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("Y"), wxT("YUD") ) ); 
  upDirectionZPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("Z"), wxT("ZUD") ) ); 
  dynamical3dPropertyGrid->
    Append( new wxPropertyCategory( wxT("Clip Distance"), wxT("Clip Distance") ) ); 
  zNearPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("Near"), wxT("Near") ) );
  zFarPropertyGridItem =
    dynamical3dPropertyGrid->Append( new wxFloatProperty( wxT("Far"), wxT("Far") ) ); 
}

bool dynamical_dialog::show_dialog(const dynamical_window_specification& spec,
				  dynamical_window_specification* ret) {
  dynamicalSpecification = spec;
  set_ui();
  if(ShowModal() == wxID_OK) {
    *ret = dynamicalSpecification;
    return true;
  } else {
    return false;
  }
}

void dynamical_dialog::cancel_button_on_button_click(wxCommandEvent& evt) {
  EndModal(wxID_CANCEL);
}

void dynamical_dialog::ok_button_on_button_click(wxCommandEvent& evt) {
  if(validate_and_set_specification()) {
    EndModal(wxID_OK);
  }
}

void dynamical_dialog::set_ui() {
  dynamical2dPropertyGrid->SetPropertyValue(horizontalAxisMinPropertyGridItem,
					 dynamicalSpecification.horizontalAxisMin);
  dynamical2dPropertyGrid->SetPropertyValue(horizontalAxisMaxPropertyGridItem,
					  dynamicalSpecification.horizontalAxisMax);
  
  dynamical2dPropertyGrid->SetPropertyValue(verticalAxisMinPropertyGridItem,
					 dynamicalSpecification.verticalAxisMin);
  dynamical2dPropertyGrid->SetPropertyValue(verticalAxisMaxPropertyGridItem,
					  dynamicalSpecification.verticalAxisMax);
  assert(dynamicalSpecification.dynamicalVariables >= 2);
  wxPGChoices list;
  list.Add("t");
  for(unsigned int i = 1; i != dynamicalSpecification.dynamicalVariables; ++i) {
    list.Add("x" + std::to_string(i));
  }
  horizontalAxisVariablePropertyGridItem->SetChoices(list);
  verticalAxisVariablePropertyGridItem->SetChoices(list);
  horizontalAxisVariablePropertyGridItem
    ->SetChoiceSelection(dynamicalSpecification.horizontalAxisVariable);
  verticalAxisVariablePropertyGridItem
    ->SetChoiceSelection(dynamicalSpecification.verticalAxisVariable);
}

bool dynamical_dialog::validate_and_set_specification() {
  dynamicalSpecification.horizontalAxisMax =
    dynamical2dPropertyGrid->
    GetPropertyValue(horizontalAxisMaxPropertyGridItem).GetDouble();
  dynamicalSpecification.horizontalAxisMin =
    dynamical2dPropertyGrid->
    GetPropertyValue(horizontalAxisMinPropertyGridItem).GetDouble();

  dynamicalSpecification.verticalAxisMax =
    dynamical2dPropertyGrid->
    GetPropertyValue(verticalAxisMaxPropertyGridItem).GetDouble();
  dynamicalSpecification.verticalAxisMin =
    dynamical2dPropertyGrid->
    GetPropertyValue(verticalAxisMinPropertyGridItem).GetDouble();
  if(dynamicalSpecification.horizontalAxisMax
     <= dynamicalSpecification.horizontalAxisMin ||
     dynamicalSpecification.verticalAxisMax
     <= dynamicalSpecification.verticalAxisMin) {
    return false;
  }
  dynamicalSpecification.horizontalAxisVariable
    = horizontalAxisVariablePropertyGridItem->GetChoiceSelection();
  dynamicalSpecification.verticalAxisVariable
    = verticalAxisVariablePropertyGridItem->GetChoiceSelection();
  return true;
}

void dynamical_dialog::viewport_radio_box_on_radio_box(wxCommandEvent&) {
  
}
} // namespace gui
} // namespace dynslover
