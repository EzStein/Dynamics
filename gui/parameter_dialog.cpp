#include "gui/parameter_dialog.h"

#include "gui/app.h"

namespace dynsolver {
namespace gui {

parameter_dialog::parameter_dialog(const app& appl)
  : parameter_dialog_base(nullptr),
    appl(appl) {
  parameterPropertyGrid->Append
    (new wxPropertyCategory( wxT("Horizontal Axis"), wxT("Horizontal Axis") ) );
  horizAxisVarPGItem = parameterPropertyGrid->Append
    (new wxEnumProperty( wxT("Variable"), wxT("VariableH") ) );
  horizAxisMinPGItem = parameterPropertyGrid->Append
    (new wxFloatProperty( wxT("Min"), wxT("MinH") ) );
  horizAxisMaxPGItem = parameterPropertyGrid->Append
    (new wxFloatProperty( wxT("Max"), wxT("MaxH") ) );

  parameterPropertyGrid->Append
    (new wxPropertyCategory( wxT("Vertical Axis"), wxT("Vertical Axis") ) );
  vertAxisVarPGItem = parameterPropertyGrid->Append
    (new wxEnumProperty( wxT("Variable"), wxT("VariableV") ) );
  vertAxisMinPGItem = parameterPropertyGrid->Append
    (new wxFloatProperty( wxT("Min"), wxT("MinV") ) );
  vertAxisMaxPGItem = parameterPropertyGrid->Append
    (new wxFloatProperty( wxT("Max"), wxT("MaxV") ) );
}

bool parameter_dialog::show_dialog(parameter_specs& inSpecs) {
  specs = inSpecs;
  set_ui();
  if(ShowModal() == wxID_OK) {
    inSpecs = specs;
    return true;
  }
  return false;
}

void parameter_dialog::cancel_button_on_button_click(wxCommandEvent&) {
  EndModal(wxID_CANCEL);
}

void parameter_dialog::ok_button_on_button_click(wxCommandEvent&) {
  if(validate_and_set()) {
    EndModal(wxID_OK);
  }
}

void parameter_dialog::set_ui() {
  
  math::vector2d min(specs.viewport.get_position());
  math::vector2d span(specs.viewport.get_span());
  math::vector2d max(span + min);
  horizAxisMinPGItem->SetValue(wxVariant(min.x()));
  horizAxisMaxPGItem->SetValue(wxVariant(max.x()));
  vertAxisMinPGItem->SetValue(wxVariant(min.y()));
  vertAxisMaxPGItem->SetValue(wxVariant(max.y()));

  varChoices.Clear();
  for(int i = 0; i != appl.get_model().get_parameters(); ++i) {
    varChoices.Add(appl.get_model().get_parameter_names()[i]);
  }
  horizAxisVarPGItem->SetChoices(varChoices);
  horizAxisVarPGItem->SetChoiceSelection(varChoices.Index(specs.horizAxisVar));
  vertAxisVarPGItem->SetChoices(varChoices);
  vertAxisVarPGItem->SetChoiceSelection(varChoices.Index(specs.vertAxisVar));
}

bool parameter_dialog::validate_and_set() {
  math::vector2d min, max;
  max.x() = horizAxisMaxPGItem->GetValue().GetDouble();
  min.x() = horizAxisMinPGItem->GetValue().GetDouble();

  max.y() = vertAxisMaxPGItem->GetValue().GetDouble();
  min.y() = vertAxisMinPGItem->GetValue().GetDouble();
  if(max.x() <= min.x() || max.y() <= min.y()) {
    return false;
  }
  specs.viewport.set_position(min);
  specs.viewport.set_span(max - min);
  specs.horizAxisVar = varChoices.GetLabel(horizAxisVarPGItem->GetChoiceSelection());
  specs.vertAxisVar = varChoices.GetLabel(vertAxisVarPGItem->GetChoiceSelection());
  return true;
}

} // namespace gui
} // namespace dynsolver
