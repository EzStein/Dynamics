#include "gui/model.h"

#include <vector>
#include <cassert>

#include <glad/glad.h>
#include <wx/glcanvas.h>

#include "parser/syntax_exception.h"
#include "compiler/expression_parser.h"
#include "math/util.h"
#include "math/vector.h"
#include "constants.h"
#include "gl/shader.h"
#include "util/util.h"
#include "../app.h"

namespace dynsolver {
namespace gui {

point2d::point2d() : math::vector(2) { }
point2d::point2d(double x, double y) : math::vector(2) {
  (*this)[0] = x;
  (*this)[1] = y;
}

double& point2d::x() {
  return (*this)[0];
}

const double& point2d::x() const {
  return (*this)[0];
}

double& point2d::y() {
  return (*this)[1];
}

const double& point2d::y() const {
  return (*this)[1];
}

solution_specification::solution_specification() : initialValue(1) { }

singular_point_specification::singular_point_specification() : initialValue(1) { }

window2d::window2d(point2d size, point2d span, point2d position) :
    size(size), span(span), position(position) { }

point2d window2d::pixel_coordinate_of(point2d real) const {
  point2d pixel;
  pixel.x() = (real.x() - position.x()) * size.x() / span.x();
  pixel.y() = (real.y() - position.y()) * size.y() / span.y();
  return pixel;
}

point2d window2d::real_coordinate_of(point2d pixel) const {
  point2d real;
  real.x() = pixel.x() * span.x() / size.x() + position.x();
  real.y() = pixel.y() * span.y() / size.y() + position.y();
  return real;
}

const point2d& window2d::get_size() const {
  return size;
}

const point2d& window2d::get_span() const {
  return span;
}

void window2d::set_size(const point2d& newSize) {
  size = newSize;
}

void window2d::set_span(const point2d& newSpan) {
  span = newSpan;
}

const point2d& window2d::get_position() const{
  return position;
}

bool window2d::contains_pixel(point2d pixel) const {
  return (pixel.x() >= 0 && pixel.x() < size.x()
          && pixel.y() >= 0 && pixel.y() < size.y());
}

bool window2d::contains_real(point2d real) const {
  return contains_pixel(pixel_coordinate_of(real));
}

void window2d::move_pixel(point2d pixel) {
  position.x() += pixel.x() * span.x() / size.x();
  position.y() += pixel.y() * span.y() / size.y();
}

void window2d::move_real(point2d real) {
  position.x() += real.x();
  position.y() += real.y();
}

void window2d::scale_pixel(point2d scale, point2d pixel) {
  scale_real(scale, real_coordinate_of(pixel));
}

void window2d::scale_real(point2d scale, point2d real) {
  span.x() *= scale.x();
  span.y() *= scale.y();
  position.x() = (position.x() - real.x()) * scale.x() + real.x();
  position.y() = (position.y() - real.y()) * scale.y() + real.y();
}

point2d window2d::get_center() const {
  point2d center;
  center.x() = position.x() + span.x()/2;
  center.y() = position.y() + span.y()/2;
  return center;
}

math::square_matrix window2d::real_to_ndc() {
  point2d center(get_center());
  math::square_matrix mat(4, 0.0);
  mat[0][0] = 2.0/span.x();
  mat[0][3] = -center.x()*mat[0][0];

  mat[1][1] = 2.0/span.y();
  mat[1][3] = -center.y()*mat[1][1];

  mat[2][2] = 1;
  mat[3][3] = 1;
  return mat;
}

math::square_matrix window2d::pixel_to_ndc() {
  point2d center(get_center());
  math::square_matrix mat(4, 0.0);
  mat[0][0] = 2.0/size.x();
  mat[0][3] = -1;

  mat[1][1] = 2.0/size.y();
  mat[1][3] = -1;

  mat[2][2] = 1;
  mat[3][3] = 1;
  return mat;
}

model::dynamical_window::dynamical_window(model& model,
					  const window2d& window, int horizontalAxisVariable,
					  int verticalAxisVariable) :
  modelData(model), window(window),
  horizontalAxisVariable(horizontalAxisVariable),
  verticalAxisVariable(verticalAxisVariable),
  axesVbo(nullptr, 8 * sizeof(float), GL_DYNAMIC_DRAW),
  axesVboVertexCount(4),
  ticksPerLabel(5),
  tickFontSize(12),
  minimumPixelTickDistance(15),
  maximumPixelTickDistance(minimumPixelTickDistance * ticksPerLabel * 2),
  realTickDistanceX(0.5),
  realTickDistanceY(0.5) {
  for(std::unordered_map<solution_id, solution>::const_iterator iter = modelData.solutions.begin();
      iter != modelData.solutions.end(); ++iter) {
    // Foreach solution
    std::vector<float> data;
    unsigned int points = iter->second.data.size();
    unsigned int size = points * 2 * sizeof(float);
    for(std::list<math::vector>::const_iterator point = iter->second.data.begin();
	point != iter->second.data.end(); ++point) {
      data.push_back((*point)[horizontalAxisVariable]);
      data.push_back((*point)[verticalAxisVariable]);
    }
    gl::buffer vbo(reinterpret_cast<unsigned char*>(data.data()), size, GL_DYNAMIC_DRAW);
    solution_render_data val{vbo, points};
    solutionRenderData.insert(std::make_pair(iter->first, val));
  }
}

void model::dynamical_window::set_specification
(const dynamical_window_specification& spec) {
  window = window2d(window.get_size(),
		    point2d(spec.horizontalAxisMax - spec.horizontalAxisMin,
			  spec.verticalAxisMax - spec.verticalAxisMin),
		    point2d(spec.horizontalAxisMin, spec.verticalAxisMin));
  
  if(horizontalAxisVariable != spec.horizontalAxisVariable ||
     verticalAxisVariable != spec.verticalAxisVariable) {
    horizontalAxisVariable = spec.horizontalAxisVariable;
    verticalAxisVariable = spec.verticalAxisVariable;
    
    // Update VBO's
    for(std::unordered_map<solution_id, solution>::const_iterator iter
	  = modelData.solutions.begin(); iter != modelData.solutions.end(); ++iter) {
      update_vbo(iter->first);
    }
  }

}

int model::dynamical_window::get_horizontal_axis_variable() const {
  return horizontalAxisVariable;
}

int model::dynamical_window::get_vertical_axis_variable() const {
  return verticalAxisVariable;
}
void model::dynamical_window::set_window(const window2d& val) {
  window = val;
}

const window2d& model::dynamical_window::get_window() const {
  return window;
}

void model::dynamical_window::clear() {
  solutionRenderData.clear();
}

void model::dynamical_window::generate_vbo(solution_id id,
					   const std::list<math::vector>& solution) {
  std::vector<float> data;
  size_t points = solution.size();
  size_t size = points * 2 * sizeof(float);
  for(std::list<math::vector>::const_iterator point = solution.begin();
      point != solution.end(); ++point) {
    data.push_back((*point)[horizontalAxisVariable]);
    data.push_back((*point)[verticalAxisVariable]);
  }
  gl::buffer vbo(reinterpret_cast<unsigned char*>(data.data()), size, GL_DYNAMIC_DRAW);
  solution_render_data val{vbo, points};
  solutionRenderData.insert(std::make_pair(id, val));
}

void model::dynamical_window::update_vbo(solution_id id) {
  std::vector<float> data;
  std::list<math::vector> solution = modelData.solutions.at(id).data;
  size_t points = solution.size();
  size_t size = points * 2 * sizeof(float);
  for(std::list<math::vector>::const_iterator point = solution.begin();
      point != solution.end(); ++point) {
    data.push_back((*point)[horizontalAxisVariable]);
    data.push_back((*point)[verticalAxisVariable]);
  }
  solutionRenderData.at(id).vertices = points;
  solutionRenderData.at(id).vbo.set_data(reinterpret_cast<unsigned char *>(data.data()), size);
}

void model::dynamical_window::paint() {
  glViewport(0, 0, window.get_size().x(), window.get_size().y());
  glUseProgram(modelData.program2d.get_handle());
  glBindVertexArray(modelData.vao2d.get_handle());
  math::square_matrix mat(window.real_to_ndc());
  float transformationMatrix[16];
  
  for(int i = 0; i != 4; ++i) {
    for(int j = 0; j != 4; ++j) {
      transformationMatrix[4*i+j] =
	mat[i][j];
    }
  }

  // Set color to Red
  glUniform4f(modelData.k2dColorUniformLocation, 1.0f, 0.0f, 0.0f, 1.0f);
  glUniformMatrix4fv(modelData.k2dTransformationUniformLocation,
		     1, true, transformationMatrix);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  for(std::unordered_map<solution_id, solution_render_data>::const_iterator iter
	= solutionRenderData.begin();
      iter != solutionRenderData.end(); ++iter) {
    glBindVertexBuffer(model::k2dVertexBinding,
		       iter->second.vbo.get_handle(), 0, 2 * sizeof(float));
    glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
  }

  // Compute the distance between each tick.
  int numberOfTicksX = window.get_span().x()/realTickDistanceX;
  int pixelTickDistanceX = window.get_size().x()/static_cast<double>(numberOfTicksX);
  if(pixelTickDistanceX < minimumPixelTickDistance) {
    realTickDistanceX *= ticksPerLabel;
  } else if(pixelTickDistanceX > maximumPixelTickDistance) {
    realTickDistanceX /= ticksPerLabel;
  }
  int numberOfTicksY = window.get_span().y()/realTickDistanceY;
  int pixelTickDistanceY = window.get_size().y()/static_cast<double>(numberOfTicksY);
  if(pixelTickDistanceY < minimumPixelTickDistance) {
    realTickDistanceY *= ticksPerLabel;
  } else if(pixelTickDistanceY > maximumPixelTickDistance) {
    realTickDistanceY /= ticksPerLabel;
  }

  // The x and y values in pixels for where the x and y axes should cross.
  point2d axesPixel(window.pixel_coordinate_of(point2d(0.0, 0.0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > window.get_size().x())
    axesPixel.x() = window.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > window.get_size().y())
    axesPixel.y() = window.get_size().y();
  
  // Set up tick data in pixel coordinates.
  std::vector<float> axesVboData;
  double xStart = realTickDistanceX *
    static_cast<int>(window.get_position().x() / realTickDistanceX);
  double yStart = realTickDistanceY *
    static_cast<int>(window.get_position().y() / realTickDistanceY);
  for(double x = xStart;
      x < window.get_position().x() + window.get_span().x(); x += realTickDistanceX) {
    float realCoordinate = window.pixel_coordinate_of(point2d(x, 0)).x();
    // First vertex
    axesVboData.push_back(realCoordinate); // X
    axesVboData.push_back(axesPixel.y() - 5); // Y

    // Second vertex
    axesVboData.push_back(realCoordinate); // X
    axesVboData.push_back(axesPixel.y() + 5); // Y
  }
  for(double y = yStart; y < window.get_position().y() + window.get_span().y();
      y += realTickDistanceY) {
    float realCoordinate = window.pixel_coordinate_of(point2d(0, y)).y();
    // First vertex
    axesVboData.push_back(axesPixel.x() - 5); // X
    axesVboData.push_back(realCoordinate); // Y
    
    // Second vertex
    axesVboData.push_back(axesPixel.x() + 5); // X
    axesVboData.push_back(realCoordinate); // Y
  }

  // Add in the actual axes
  axesVboData.push_back(0);
  axesVboData.push_back(axesPixel.y());
  
  axesVboData.push_back(window.get_size().x());
  axesVboData.push_back(axesPixel.y());

  axesVboData.push_back(axesPixel.x());
  axesVboData.push_back(0);
  
  axesVboData.push_back(axesPixel.x());
  axesVboData.push_back(window.get_size().y());
  
  axesVbo.set_data(reinterpret_cast<unsigned char*>(axesVboData.data()),
		   axesVboData.size()*sizeof(float));

  // Generate pixel to ndc transform
  mat = window.pixel_to_ndc();
  for(int i = 0; i != 4; ++i) {
    for(int j = 0; j != 4; ++j) {
      transformationMatrix[4*i+j] =
	mat[i][j];
    }
  }
  glUniformMatrix4fv(modelData.k2dTransformationUniformLocation, 1, true, transformationMatrix);

  // Set color to Black
  glUniform4f(modelData.k2dColorUniformLocation, 0.0f, 0.0f, 0.0f, 1.0f);
  glBindVertexBuffer(model::k2dVertexBinding, axesVbo.get_handle(),
		     0, 2 * sizeof(float));
  glDrawArrays(GL_LINES, 0, axesVboData.size() / 2);
  
  const double tolerance = 0.001;
  // Draw Axes text
  point2d axesReal(window.real_coordinate_of(axesPixel));
  xStart = realTickDistanceX * ticksPerLabel *
    static_cast<int>(window.get_position().x() / (realTickDistanceX * ticksPerLabel));
  yStart = realTickDistanceY * ticksPerLabel *
    static_cast<int>(window.get_position().y() / (realTickDistanceY * ticksPerLabel));
  for(double x = xStart; x > window.get_position().x();
      x -= ticksPerLabel * realTickDistanceX) {
    if(std::abs(x) < tolerance) continue;
    point2d pixel(window.pixel_coordinate_of(point2d(x, axesReal.y())));
    std::string text = util::double_to_string(x, 2);
    modelData.textRenderer.render_text(text, pixel.x() - text.size() * 3, pixel.y() + 10,
				       modelData.font, tickFontSize);
  }
  for(double x = xStart; x < window.get_position().x() + window.get_span().x();
      x += ticksPerLabel * realTickDistanceX) {
    if(std::abs(x) < tolerance) continue;
    point2d pixel(window.pixel_coordinate_of(point2d(x, axesReal.y())));
    std::string text = util::double_to_string(x, 2);
    modelData.textRenderer.render_text(text, pixel.x() - text.size() * 3,
				       pixel.y() + 10, modelData.font, tickFontSize);
  }

  for(double y = yStart; y > window.get_position().y();
      y -= ticksPerLabel * realTickDistanceY) {
    if(std::abs(y) < tolerance) continue;
    point2d pixel(window.pixel_coordinate_of(point2d(axesReal.x(),y)));
    std::string text = util::double_to_string(y, 2);
    modelData.textRenderer.render_text(text, pixel.x() + 10, pixel.y(),
				       modelData.font, tickFontSize);
  }
  for(double y = yStart; y < window.get_position().y() + window.get_span().y();
      y += ticksPerLabel * realTickDistanceY) {
    if(std::abs(y) < tolerance) continue;
    point2d pixel(window.pixel_coordinate_of(point2d(axesReal.x(),y)));
    std::string text = util::double_to_string(y, 2);
    modelData.textRenderer.render_text(text, pixel.x() + 10,
				       pixel.y(), modelData.font, tickFontSize);
  }
}
  
void model::dynamical_window::resize(double width, double height) {
  double changeWidth = (width - window.get_size().x())
    * window.get_span().x() / window.get_size().x();
  double changeHeight = (height - window.get_size().y())
    * window.get_span().y() / window.get_size().y();
  window.set_size(point2d(width, height));
  window.set_span(point2d(changeWidth + window.get_span().x(),
			  changeHeight + window.get_span().y()));
}

bool model::dynamical_window::on_vertical_axis(double x, double y) const {
  point2d axesPixel(window.pixel_coordinate_of(point2d(0.0, 0.0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > window.get_size().x())
    axesPixel.x() = window.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > window.get_size().y())
    axesPixel.y() = window.get_size().y();
  double axis = axesPixel.x();
  return axis - 5 < x && x < axis + 5;
}
bool model::dynamical_window::on_horizontal_axis(double x, double y) const {
  point2d axesPixel(window.pixel_coordinate_of(point2d(0.0, 0.0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > window.get_size().x())
    axesPixel.x() = window.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > window.get_size().y())
    axesPixel.y() = window.get_size().y();
  double axis = axesPixel.y();
  return axis - 5 < y && y < axis + 5;
}

const std::string model::k2dVertexShaderFilePath
(::dynsolver::app::generate_resource_path(std::vector<std::string>{"gl"}, "2d_renderer.vert"));
const GLuint model::k2dPositionAttribute(0);
const GLuint model::k2dVertexBinding(0);
const std::string model::k2dTransformationUniform("transformation");

const std::string model::k2dFragmentShaderFilePath
(::dynsolver::app::generate_resource_path(std::vector<std::string>{"gl"}, "2d_renderer.frag"));
const std::string model::k2dColorUniform("inColor");

std::vector<gl::shader> model::build_shaders() {
  std::vector<gl::shader> shaders;
  shaders.push_back(gl::shader(util::read_file(model::k2dVertexShaderFilePath),
			       GL_VERTEX_SHADER));
  shaders.push_back(gl::shader(util::read_file(model::k2dFragmentShaderFilePath),
			       GL_FRAGMENT_SHADER));
  return shaders;
}

model::model() : parameterVariables(0),
		 parameterPosition(1, 0.0),
		 uniqueDynamicalId(0),
                 defaultWindow(window2d(point2d(500, 500),
                                        point2d(10, 10),
                                        point2d(-5, -5))),

		 font(constants::kDefaultFontFilePath),
		 dynamicalDimension(0),
		 compiled(false),
		 program2d(build_shaders()),
		 k2dTransformationUniformLocation
		 (glGetUniformLocation(program2d.get_handle(),
				       k2dTransformationUniform.c_str())),
		 k2dColorUniformLocation
		 (glGetUniformLocation(program2d.get_handle(),
				       k2dColorUniform.c_str())) {
  glUseProgram(program2d.get_handle());
  glBindVertexArray(vao2d.get_handle());
  glEnableVertexAttribArray(k2dPositionAttribute);
  glVertexAttribFormat(k2dPositionAttribute,
		       2, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(k2dPositionAttribute,
			k2dVertexBinding);


}


void model::add_solution(const solution_specification& spec) {
  assert(spec.initialValue.size() == get_dynamical_variables());
  assert(spec.initialValue[0] > spec.tMin && spec.initialValue[0] < spec.tMax && spec.increment > 0);
  assert(compiled);
  
  std::list<math::vector> points;
  std::vector<compiler::function<double, const double*>> systemFunctions;

  for(std::vector<expression>::const_iterator iter = system.begin();
      iter != system.end(); ++iter) {
    systemFunctions.push_back(iter->function);
  }
  
  math::vector tmp(spec.initialValue);
  // Fill in forwards
  while(tmp[0] <= spec.tMax) {
    points.push_back(tmp);
    math::euler(tmp, systemFunctions, parameterPosition, spec.increment);
  }
  
  tmp = spec.initialValue;
  // Fill in backwards
  while(tmp[0] >= spec.tMin) {
    points.push_front(tmp);
    math::euler(tmp, systemFunctions, parameterPosition, -spec.increment);
  }
  
  int solutionId = ++uniqueSolutionId;
  solutions.insert(std::make_pair(solutionId,
                                  solution{spec, points}));
  // We now need to update the VBO's of each window.
  for(std::unordered_map<dynamical_window_id, dynamical_window>::iterator iter
	= dynamicalWindows.begin(); iter != dynamicalWindows.end(); ++iter) {
    iter->second.generate_vbo(solutionId, solutions.at(solutionId).data);
  }
}
                    
bool model::compile(std::vector<std::string> newExpressions) {
  assert(newExpressions.size() >= 1);

  int newDynamicalDimension = newExpressions.size();
  
  // Set up symbol table.
  std::list<symbol> symbolTable;
  symbolTable.push_back(symbol("t",0,0));
  for(int i = 0; i != newDynamicalDimension; ++i) {
    std::string str("x" + std::to_string(i + 1));
    symbolTable.push_back(symbol(str,i+1, static_cast<unsigned int>(i+1)));
  }

  for(int i = 0; i != parameterVariables; ++i) {
    std::string str("a" + std::to_string(i + 1));
    symbolTable.push_back(symbol(str, newDynamicalDimension+1+i, newDynamicalDimension + 1 + i));
  }
  
  symbolTable.sort([](const symbol& a, const symbol& b) -> bool {
      return a.parameter < b.parameter;
    });

  std::vector<expression> newSystem;
  std::vector<expression> newJacobian;
  
  compiler::expression_parser parse;
  for(std::vector<std::string>::const_iterator expr = newExpressions.begin();
      expr != newExpressions.end(); ++expr) {
    AST ast;
    try {
      ast = parse.parse(*expr, symbolTable);
    } catch (const parser::syntax_exception& exc) {
      return false;
    }
    compiler::function<double, const double*> function = ast.compile();
    newSystem.push_back(expression{ast, function});
    for(symbol sym : symbolTable) {
      //We must check that the symbol is indeed a variable and not a parameter
      if(sym.parameter == 0 || sym.parameter >= get_dynamical_variables())
        continue;
      AST diffAst = AST(ast).differentiate(sym.name);
      compiler::function<double, const double*> diffFunction = diffAst.compile();
      newJacobian.push_back(expression{diffAst, diffFunction});
    }
  }

  // We have successfully compiled!

  system = newSystem;
  jacobian = newJacobian;

  expressions = newExpressions;
  dynamicalDimension = static_cast<int>(expressions.size());

  for(std::unordered_map<dynamical_window_id, dynamical_window>::iterator iter
	= dynamicalWindows.begin(); iter != dynamicalWindows.end(); ++iter) {
    iter->second.clear();
  }

  solutions.clear();
  compiled = true;
  return true;
}

int model::add_dynamical_window(const dynamical_window_specification& spec,
				int width, int height) {
  assert(spec.horizontalAxisVariable < get_dynamical_variables() || !compiled);
  assert(spec.verticalAxisVariable < get_dynamical_variables() || !compiled);
  window2d window(point2d(width, height),
		  point2d(spec.horizontalAxisMax - spec.horizontalAxisMin,
			  spec.verticalAxisMax - spec.verticalAxisMin),
		  point2d(spec.horizontalAxisMin, spec.verticalAxisMin));
  dynamicalWindows.insert(
      std::make_pair(
          uniqueDynamicalId,
          dynamical_window(*this, window, spec.horizontalAxisVariable,
			   spec.verticalAxisVariable)));
  return uniqueDynamicalId++;
}

void model::set_dynamical_window_specification(const dynamical_window_specification& spec,
					       dynamical_window_id id) {
  dynamicalWindows.at(id).set_specification(spec);
}

void model::delete_dynamical_window(dynamical_window_id id) {
  dynamicalWindows.erase(id);
}

void model::delete_all_dynamical_windows() {
  dynamicalWindows.clear();
}

int model::get_dynamical_variables() const {
  return dynamicalDimension + 1;
}

int model::get_dynamical_dimension() const {
  return dynamicalDimension;
}

void model::move_dynamical_window(int x, int y, int) { assert(false); }
void model::scale_dynamical_window(double scale, int x, int y, int) { assert(false); }

void model::paint_dynamical_window(dynamical_window_id id) {
  dynamicalWindows.at(id).paint();
}
void model::resize_dynamical_window(dynamical_window_id id, double width, double height) {
  dynamicalWindows.at(id).resize(width, height);
}

void model::set_dynamical_window(const window2d& window, dynamical_window_id id) {
  dynamicalWindows.at(id).set_window(window);
}

const model::dynamical_window& model::get_dynamical_window(dynamical_window_id id) const {
  // Dangerous if another window is added after returning!!
  return dynamicalWindows.at(id);
}

bool model::on_vertical_axis(double x, double y, dynamical_window_id id) const {
  dynamicalWindows.at(id).on_vertical_axis(x, y);
}
bool model::on_horizontal_axis(double x, double y, dynamical_window_id id) const {
  dynamicalWindows.at(id).on_horizontal_axis(x, y);
}
} // namespace gui
} // namespace dynslover
