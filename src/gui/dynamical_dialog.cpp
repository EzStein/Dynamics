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
    Append(new wxPropertyCategory( wxT("Axes Variables"), wxT("Axes Variables") ) );
  xAxisVariablePropertyGridItem = dynamical3dPropertyGrid->
    Append( new wxEnumProperty( wxT("X"), wxT("XV") ) );
  yAxisVariablePropertyGridItem = dynamical3dPropertyGrid->
    Append( new wxEnumProperty( wxT("Y"), wxT("YV") ) );
  zAxisVariablePropertyGridItem = dynamical3dPropertyGrid->
    Append( new wxEnumProperty( wxT("Z"), wxT("ZV") ) ); 
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

bool dynamical_dialog::show_dialog(const dynamical_specs& spec,
				  dynamical_specs* ret) {
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
  // 2d
  double horizontalAxisMin, horizontalAxisMax, verticalAxisMin, verticalAxisMax;
  horizontalAxisMin = dynamicalSpecification.viewport2d.get_position().x();
  verticalAxisMin = dynamicalSpecification.viewport2d.get_position().y();
  horizontalAxisMax = horizontalAxisMin
    + dynamicalSpecification.viewport2d.get_span().x();
  verticalAxisMax = verticalAxisMin
    + dynamicalSpecification.viewport2d.get_span().y();
  
  dynamical2dPropertyGrid->SetPropertyValue(horizontalAxisMinPropertyGridItem,
					 horizontalAxisMin);
  dynamical2dPropertyGrid->SetPropertyValue(horizontalAxisMaxPropertyGridItem,
					  horizontalAxisMax);
  
  dynamical2dPropertyGrid->SetPropertyValue(verticalAxisMinPropertyGridItem,
					 verticalAxisMin);
  dynamical2dPropertyGrid->SetPropertyValue(verticalAxisMaxPropertyGridItem,
					  verticalAxisMax);

  // 3d
  math::vector3d cameraPosition(dynamicalSpecification.viewport3d.get_position());
  math::vector3d cameraDirection(dynamicalSpecification.viewport3d.get_view_direction());
  math::vector3d upDirection(dynamicalSpecification.viewport3d.get_up_direction());
  double zNear = dynamicalSpecification.viewport3d.get_near_z();
  double zFar = dynamicalSpecification.viewport3d.get_far_z();
  dynamical3dPropertyGrid->SetPropertyValue(cameraPositionXPropertyGridItem,
					    cameraPosition.x());
  dynamical3dPropertyGrid->SetPropertyValue(cameraPositionYPropertyGridItem,
					    cameraPosition.y());
  dynamical3dPropertyGrid->SetPropertyValue(cameraPositionZPropertyGridItem,
					    cameraPosition.z());

  dynamical3dPropertyGrid->SetPropertyValue(cameraDirectionXPropertyGridItem,
					    cameraDirection.x());
  dynamical3dPropertyGrid->SetPropertyValue(cameraDirectionYPropertyGridItem,
					    cameraDirection.y());
  dynamical3dPropertyGrid->SetPropertyValue(cameraDirectionZPropertyGridItem,
					    cameraDirection.z());

  dynamical3dPropertyGrid->SetPropertyValue(upDirectionXPropertyGridItem,
					    upDirection.x());
  dynamical3dPropertyGrid->SetPropertyValue(upDirectionYPropertyGridItem,
					    upDirection.y());
  dynamical3dPropertyGrid->SetPropertyValue(upDirectionZPropertyGridItem,
					    upDirection.z());

  dynamical3dPropertyGrid->SetPropertyValue(zNearPropertyGridItem,
					    zNear);
  dynamical3dPropertyGrid->SetPropertyValue(zFarPropertyGridItem,
					    zFar);

  // Set choices.
  assert(dynamicalSpecification.dynamicalVars >= 2);
  wxPGChoices list;
  list.Add("t");
  for(unsigned int i = 1; i != dynamicalSpecification.dynamicalVars; ++i) {
    list.Add("x" + std::to_string(i));
  }
  horizontalAxisVariablePropertyGridItem->SetChoices(list);
  verticalAxisVariablePropertyGridItem->SetChoices(list);
  xAxisVariablePropertyGridItem->SetChoices(list);
  yAxisVariablePropertyGridItem->SetChoices(list);
  zAxisVariablePropertyGridItem->SetChoices(list);
  horizontalAxisVariablePropertyGridItem
    ->SetChoiceSelection(dynamicalSpecification.horzAxisVar);
  verticalAxisVariablePropertyGridItem
    ->SetChoiceSelection(dynamicalSpecification.vertAxisVar);
  xAxisVariablePropertyGridItem->
    SetChoiceSelection(dynamicalSpecification.xAxisVar);
  yAxisVariablePropertyGridItem->
    SetChoiceSelection(dynamicalSpecification.yAxisVar);
  zAxisVariablePropertyGridItem->
    SetChoiceSelection(dynamicalSpecification.zAxisVar);

  if(dynamicalSpecification.is3d) {
    viewportRadioBox->SetSelection(1);
  }
}

bool dynamical_dialog::validate_and_set_specification() {
  double horizontalAxisMax = horizontalAxisMaxPropertyGridItem->GetValue().GetDouble();
  double horizontalAxisMin = horizontalAxisMinPropertyGridItem->GetValue().GetDouble();

  double verticalAxisMax = verticalAxisMaxPropertyGridItem->GetValue().GetDouble();
  double verticalAxisMin = verticalAxisMinPropertyGridItem->GetValue().GetDouble();
  if(horizontalAxisMax <= horizontalAxisMin ||  verticalAxisMax <= verticalAxisMin) {
    return false;
  }
  dynamicalSpecification.viewport2d
    .set_position(math::vector2d(horizontalAxisMin, verticalAxisMin));
  dynamicalSpecification.viewport2d
    .set_span(math::vector2d(horizontalAxisMax - horizontalAxisMin,
			     verticalAxisMax - verticalAxisMin));
  dynamicalSpecification.horzAxisVar
    = horizontalAxisVariablePropertyGridItem->GetChoiceSelection();
  dynamicalSpecification.vertAxisVar
    = verticalAxisVariablePropertyGridItem->GetChoiceSelection();

  math::vector3d cameraPosition(cameraPositionXPropertyGridItem->GetValue().GetDouble(),
			  cameraPositionYPropertyGridItem->GetValue().GetDouble(),
			  cameraPositionZPropertyGridItem->GetValue().GetDouble());
  math::vector3d cameraDirection(cameraDirectionXPropertyGridItem->GetValue().GetDouble(),
			  cameraDirectionYPropertyGridItem->GetValue().GetDouble(),
			  cameraDirectionZPropertyGridItem->GetValue().GetDouble());
  math::vector3d upDirection(upDirectionXPropertyGridItem->GetValue().GetDouble(),
		       upDirectionYPropertyGridItem->GetValue().GetDouble(),
		       upDirectionZPropertyGridItem->GetValue().GetDouble());

  // Verify that the upDriection and cameraDirection are not colinear.
  math::vector3d up = math::vector3d::cross(cameraDirection, upDirection);
  const double tolerance = 0.001;
  if(std::abs(up.x()) <= tolerance &&
     std::abs(up.y()) <= tolerance &&
     std::abs(up.z()) <= tolerance) {
    return false;
  }
  double zNear = zNearPropertyGridItem->GetValue().GetDouble();
  double zFar = zFarPropertyGridItem->GetValue().GetDouble();
  if(zNear <= 0 || zNear >= zFar) return false;

  dynamicalSpecification.viewport3d
    = math::window3d(cameraPosition, cameraDirection, upDirection, zNear, zFar,
		     dynamicalSpecification.viewport3d.get_width(),
		     dynamicalSpecification.viewport3d.get_height());

  dynamicalSpecification.xAxisVar
    = xAxisVariablePropertyGridItem->GetChoiceSelection();
  dynamicalSpecification.yAxisVar
    = yAxisVariablePropertyGridItem->GetChoiceSelection();
  dynamicalSpecification.zAxisVar
    = zAxisVariablePropertyGridItem->GetChoiceSelection();

  dynamicalSpecification.is3d = viewportRadioBox->GetSelection() == 0?false:true;
  return true;
}

void dynamical_dialog::viewport_radio_box_on_radio_box(wxCommandEvent&) {
  int selection = viewportRadioBox->GetSelection();
  if(selection == 0) {
    dynamical2dPropertyGrid->Show();
    dynamical3dPropertyGrid->Hide();
    Layout();
  } else if(selection == 1) {
    dynamical3dPropertyGrid->Show();
    dynamical2dPropertyGrid->Hide();
    Layout();
  } else {
    assert(false);
  }
}
} // namespace gui
} // namespace dynslover
