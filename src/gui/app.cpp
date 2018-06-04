#include "gui/app.h"

#include <cassert>
#include <vector>

namespace dynsolver {
namespace gui {

bool app::OnInit() {
  // Setup a basic example.
  modelData.set_parameters(2);
  std::vector<std::string> initialExpressions;
  initialExpressions.push_back("b*x^2 - a*y");
  initialExpressions.push_back("x*y - a - t");
  bool success = modelData.compile(initialExpressions);
  assert(success);

  window2d win(point2d(500, 500), point2d(10, 10), point2d(-5, 5));
  int parameterId = modelData.add_parameter_window(win,0,1);
  int dynamicalId = modelData.add_dynamical_window(win,1,2);
  parameterFrames.insert(
      std::make_pair(parameterId, new parameter_frame(*this, parameterId)));
  dynamicalFrames.insert(
      std::make_pair(dynamicalId, new dynamical_frame(*this, dynamicalId)));
  consoleFrame = new console_frame(*this);  
  consoleFrame->Show();
  dynamicalFrames.begin()->second->Show();
  parameterFrames.begin()->second->Show();
  return true;
}

int app::OnExit() {
  return 0;
}

const model& app::get_model() {
  return modelData;
}

} // namespace gui
} // namespace dynsolver
