#include "gui/limit_cycle_bifurcation_dialog.h"

#include "gui/app.h"

#include <string>

namespace dynsolver {
namespace gui {

limit_cycle_bifurcation_dialog::limit_cycle_bifurcation_dialog(const app& appl)
  : limit_cycle_bifurcation_dialog_base(nullptr), appl(appl) {
  initialParametersDataViewCtrl->AppendTextColumn("Parameter");
  initialParametersDataViewCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);

  limitCyclePG->
    Append( new wxPropertyCategory( wxT("Transversal A"), wxT("Transversal A") ) );
  transversalAHorizPGItem = limitCyclePG->
    Append( new wxFloatProperty( wxT("Horizontal"), wxT("Horizontal A") ) );
  transversalAVertPGItem = limitCyclePG->
    Append( new wxFloatProperty( wxT("Vertical"), wxT("Vertical A") ) );
  limitCyclePG->
    Append( new wxPropertyCategory( wxT("Transversal A"), wxT("Transversal A") ) );
  transversalBHorizPGItem = limitCyclePG->
    Append( new wxFloatProperty( wxT("Horizontal"), wxT("Horizontal B") ) );
  transversalBVertPGItem = limitCyclePG->
    Append( new wxFloatProperty( wxT("Vertical"), wxT("Vertical B") ) );
}

bool limit_cycle_bifurcation_dialog::show_dialog(limit_cycle_bifurcation_specs& newSpecs) {
  specs = newSpecs;
  set_ui();
  if(ShowModal() == wxID_OK) {
    newSpecs = specs;
    return true;
  } else {
    return false;
  }
}

void limit_cycle_bifurcation_dialog::set_ui() {
  incrementPGItem->SetValue(specs.inc);
  spanPGItem->SetValue(specs.span);
  searchRadiusPGItem->SetValue(specs.searchRadius);
  searchIncrementPGItem->SetValue(specs.searchInc);
  transversalAHorizPGItem->SetValue(specs.transversalA[0]);
  transversalAVertPGItem->SetValue(specs.transversalA[1]);
  transversalBHorizPGItem->SetValue(specs.transversalB[0]);
  transversalBVertPGItem->SetValue(specs.transversalB[1]);

  initialParametersDataViewCtrl->DeleteAllItems();
  for(int i = 0; i != appl.get_model().get_parameters(); ++i) {
    wxVector<wxVariant> data;
    data.push_back(appl.get_model().get_parameter_names()[i]);
    data.push_back(std::to_string(specs.init[i]));
    initialParametersDataViewCtrl->AppendItem(data);
  }
  int row = 0;
  /*for(std::unordered_map<periodic_solution_id, separatrix>::const_iterator iter =
	appl.get_model().get_separatrices().begin();
      iter != appl.get_model().get_separatrices().begin(); ++iter) {
    separatrix1ComboBox->Append(std::to_string(iter->first));
    separatrix2ComboBox->Append(std::to_string(iter->first));
    if(specs.separatrix1 == iter->first) {
      separatrix1ComboBox->SetSelection(row);
    }
    if(specs.separatrix2 == iter->first) {
      separatrix2ComboBox->SetSelection(row);
    }
    ++row;
    }*/
}

bool limit_cycle_bifurcation_dialog::validate_and_set() {
  specs.inc = incrementPGItem->GetValue().GetDouble();
  if(specs.inc <= 0) return false;
  specs.span = spanPGItem->GetValue().GetInteger();
  if(specs.span <= 0) return false;
  specs.searchRadius = searchRadiusPGItem->GetValue().GetDouble();
  if(specs.searchRadius <= 0) return false;
  specs.searchInc = searchIncrementPGItem->GetValue().GetDouble();
  if(specs.searchInc <= 0) return false;
  for(int i = 0; i != appl.get_model().get_parameters(); ++i) {
    try {
      specs.init[i]
	= std::stod(initialParametersDataViewCtrl->GetTextValue(i, 1).ToStdString());
    } catch (const std::invalid_argument& exc) {
      return false;
    } catch (const std::out_of_range& exc) {
      return false;
    }
  }
  std::string value(limitCycle1ComboBox->GetStringSelection().ToStdString());
  if(value == "") {
    return false;
  }
  specs.cycle1 = std::stoi(value);
  value = limitCycle2ComboBox->GetStringSelection().ToStdString();
  if(value == "") {
    return false;
  }
  specs.cycle2 = std::stoi(value);

  specs.transversalA[0] = transversalAHorizPGItem->GetValue().GetDouble();
  specs.transversalA[1] = transversalAVertPGItem->GetValue().GetDouble();
  specs.transversalB[0] = transversalBHorizPGItem->GetValue().GetDouble();
  specs.transversalB[1] = transversalBVertPGItem->GetValue().GetDouble();
  if(specs.transversalA.distance(specs.transversalB) <= 10e-5) return false;
  return true;
}

void limit_cycle_bifurcation_dialog::cancel_button_on_button_click(wxCommandEvent& evt) {
  EndModal(wxID_CANCEL);
}

void limit_cycle_bifurcation_dialog::ok_button_on_button_click(wxCommandEvent& evt) {
  if(validate_and_set()) {
    EndModal(wxID_OK);
  }
}

} // namespace gui
} // namespace dynsolver
