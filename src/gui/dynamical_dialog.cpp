#include "gui/dynamical_dialog.h"

#include <cassert>

namespace dynsolver {
namespace gui {

dynamical_dialog::dynamical_dialog() :
  dynamical_dialog_base(nullptr) {
  dynamicalPropertyGrid->Append
    (new wxPropertyCategory( wxT("Horizontal Axis"), wxT("Horizontal Axis") ) ); 
  horizontalAxisVariablePropertyGridItem = dynamicalPropertyGrid->
    Append( new wxEnumProperty( wxT("Variable"), wxT("VariableH") ) ); 
  horizontalAxisMinPropertyGridItem = dynamicalPropertyGrid->
    Append( new wxFloatProperty( wxT("Minimum"), wxT("MinimumH") ) ); 
  horizontalAxisMaxPropertyGridItem = dynamicalPropertyGrid->
    Append( new wxFloatProperty( wxT("Maximum"), wxT("MaximumH") ) ); 
  dynamicalPropertyGrid->
    Append( new wxPropertyCategory( wxT("Vertical Axis"), wxT("Vertical Axis") ) ); 
  verticalAxisVariablePropertyGridItem = dynamicalPropertyGrid->
    Append( new wxEnumProperty( wxT("Variable"), wxT("VariableV") ) ); 
  verticalAxisMinPropertyGridItem = dynamicalPropertyGrid->
    Append( new wxFloatProperty( wxT("Minimum"), wxT("MinimumV") ) ); 
  verticalAxisMaxPropertyGridItem = dynamicalPropertyGrid->
    Append( new wxFloatProperty( wxT("Maximum"), wxT("MaximumV") ) ); 
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
  dynamicalPropertyGrid->SetPropertyValue(horizontalAxisMinPropertyGridItem,
					 dynamicalSpecification.horizontalAxisMin);
  dynamicalPropertyGrid->SetPropertyValue(horizontalAxisMaxPropertyGridItem,
					  dynamicalSpecification.horizontalAxisMax);
  
  dynamicalPropertyGrid->SetPropertyValue(verticalAxisMinPropertyGridItem,
					 dynamicalSpecification.verticalAxisMin);
  dynamicalPropertyGrid->SetPropertyValue(verticalAxisMaxPropertyGridItem,
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
    dynamicalPropertyGrid->
    GetPropertyValue(horizontalAxisMaxPropertyGridItem).GetDouble();
  dynamicalSpecification.horizontalAxisMin =
    dynamicalPropertyGrid->
    GetPropertyValue(horizontalAxisMinPropertyGridItem).GetDouble();

  dynamicalSpecification.verticalAxisMax =
    dynamicalPropertyGrid->
    GetPropertyValue(verticalAxisMaxPropertyGridItem).GetDouble();
  dynamicalSpecification.verticalAxisMin =
    dynamicalPropertyGrid->
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
} // namespace gui
} // namespace dynslover
