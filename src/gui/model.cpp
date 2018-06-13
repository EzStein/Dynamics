#include "gui/model.h"

#include <vector>
#include <cassert>

#include <glad/glad.h>
#include <wx/glcanvas.h>

#include "compiler/expression_parser.h"
#include "math/util.h"
#include "math/vector.h"
#include "constants.h"
#include "gui/gl_util.h"

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

window2d::window2d(point2d size, point2d span, point2d position) :
    size(size), span(span), position(position) { }

point2d window2d::pixel_coordinate_of(point2d real) const {
  point2d pixel;
  pixel.x() = (real.x() - position.x()) * size.x() / span.x();
  pixel.y() = (position.y() - real.y()) * size.y() / span.y();
  return pixel;
}

point2d window2d::real_coordinate_of(point2d pixel) const {
  point2d real;
  real.x() = pixel.x() * span.x() / size.x() + position.x();
  real.y() = position.y() - pixel.y() * span.y() / size.y();
  return real;
}

point2d window2d::get_size() const {
  return size;
}

point2d window2d::get_position() const{
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
  position.y() += -pixel.y() * span.y() / size.y();
}

void window2d::move_real(point2d real) {
  position.x() += real.x();
  position.y() += real.y();
}

void window2d::scale_pixel(point2d scale, point2d pixel) {
  -pixel;
  move_pixel(pixel);
  span.x() *= scale.x();
  span.y() *= scale.y();
  -pixel;
  move_pixel(pixel);
}

void window2d::scale_real(point2d scale, point2d real) {
  scale_pixel(scale, pixel_coordinate_of(real));
}

point2d window2d::get_center() const {
  point2d center;
  center.x() = position.x() + span.x()/2;
  center.y() = position.y() - span.y()/2;
  return center;
}

math::square_matrix window2d::get_transformation_matrix() {
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

solution::solution(const math::vector& initial, double tMax, double tMin,
                   double increment, const std::vector<math::vector>& data) :
    initial(initial), tMax(tMax), tMin(tMin), increment(increment), data(data) { }

dynamical_window::dynamical_window(const window2d& window, int horizontalAxisVariable,
                                   int verticalAxisVariable, GLuint vao, GLuint program,
				   const std::unordered_map<solution_id, solution>& solutions) :
  window(window), horizontalAxisVariable(horizontalAxisVariable), vao(vao), program(program),
    verticalAxisVariable(verticalAxisVariable) {
  
  for(std::unordered_map<solution_id, solution>::const_iterator iter = solutions.begin();
      iter != solutions.end(); ++iter) {
    // Foreach solution
    std::vector<float> data;
    size_t points = iter->second.data.size();
    size_t size = points * 2 * sizeof(float);
    for(std::vector<math::vector>::const_iterator point = iter->second.data.begin();
	point != iter->second.data.end(); ++point) {
      data.push_back((*point)[horizontalAxisVariable]);
      data.push_back((*point)[verticalAxisVariable]);
    }
    vbo_manager vbo(reinterpret_cast<unsigned char*>(data.data()), size, GL_DYNAMIC_DRAW);
    solution_render_data val{vbo, points};
    solutionRenderData.insert(std::make_pair(iter->first, val));
  }
}

void dynamical_window::set_horizontal_axis_variable(int val) {
  horizontalAxisVariable = val;
}
int dynamical_window::get_horizontal_axis_variable() const {
  return horizontalAxisVariable;
}
void dynamical_window::set_vertical_axis_variable(int val) {
  verticalAxisVariable = val;
}
int dynamical_window::get_vertical_axis_variable() const {
  return verticalAxisVariable;
}
void dynamical_window::set_window(const window2d& val) {
  window = val;
}

const window2d& dynamical_window::get_window() const {
  return window;
}

void dynamical_window::generate_vbo(solution_id id, const std::vector<math::vector>& solution) {
  std::vector<float> data;
  size_t points = solution.size();
  size_t size = points * 2 * sizeof(float);
  for(std::vector<math::vector>::const_iterator point = solution.begin();
      point != solution.end(); ++point) {
    data.push_back((*point)[horizontalAxisVariable]);
    data.push_back((*point)[verticalAxisVariable]);
  }
  vbo_manager vbo(reinterpret_cast<unsigned char*>(data.data()), size, GL_DYNAMIC_DRAW);
  solution_render_data val{vbo, points};
  solutionRenderData.insert(std::make_pair(id, val));
}

void dynamical_window::paint() {
  glViewport(0, 0, window.get_size().x(), window.get_size().y());
  glUseProgram(program);
  glBindVertexArray(vao);
  GLuint location =
    glGetUniformLocation(program, constants::vertex_shader::kTransformationUniform.c_str());
  math::square_matrix mat(window.get_transformation_matrix());
  float float_data[16];
  
  for(int i = 0; i != 4; ++i) {
    for(int j = 0; j != 4; ++j) {
      float_data[4*i+j] =
	mat[i][j];
    }
  }
  
  glUniformMatrix4fv(location, 1, true, float_data);
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  for(solution_iter iter = solutionRenderData.begin();
      iter != solutionRenderData.end(); ++iter) {
    glBindVertexBuffer(constants::vertex_shader::kVertexBinding,
		       iter->second.vbo.get_vbo(), 0, 2 * sizeof(float));
    glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
  }
}
  
void dynamical_window::resize() { }

vbo_manager::vbo_manager(GLenum storage) : storage(storage), size(0) {
  glGenBuffers(1, &vbo);
}

vbo_manager::vbo_manager(unsigned char* data, size_t size, GLenum storage)
   : storage(storage), size(size) {
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_COPY_WRITE_BUFFER, vbo);
  glBufferData(GL_COPY_WRITE_BUFFER, size, data, storage);
}

vbo_manager::~vbo_manager() {
  // Does nothing if VBO is 0
  glDeleteBuffers(1, &vbo);
}

vbo_manager::vbo_manager(const vbo_manager& other) :
  storage(other.storage), size(other.size) {
  glGenBuffers(1, &vbo);
  if(size != 0) {
    glBindBuffer(GL_COPY_WRITE_BUFFER, vbo);
    glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, storage);
    glBindBuffer(GL_COPY_READ_BUFFER, other.vbo);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
  }
}

vbo_manager::vbo_manager(vbo_manager&& other) : vbo(other.vbo),
  storage(other.storage), size(other.size) {
  // Prevent the vbo from being destroyed.
  other.vbo = 0;
}

vbo_manager& vbo_manager::operator=(const vbo_manager& other) {
  if(&other == this) return *this;
  glDeleteBuffers(1, &vbo);
  
  storage = other.storage;
  size = other.size;
  glGenBuffers(1, &vbo);
  if(size != 0) {
    glBindBuffer(GL_COPY_WRITE_BUFFER, vbo);
    glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, storage);
    glBindBuffer(GL_COPY_READ_BUFFER, other.vbo);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
  }
}

vbo_manager& vbo_manager::operator=(vbo_manager&& other) {
  if(&other == this) return *this;
  storage = other.storage;
  size = other.size;
  vbo = other.vbo;
  
  // Prevent the vbo from being destroyed.
  other.vbo = 0;
  return *this;
}

void vbo_manager::set_data(unsigned char* data, size_t newSize) {
  glBindBuffer(GL_COPY_WRITE_BUFFER, vbo);
  if(newSize > size) {
    // The buffer must increase so we reallocate.
    glBufferData(GL_COPY_WRITE_BUFFER, newSize, data, storage);
  } else {
    // We can reuse the same buffer
    glBufferSubData(GL_COPY_WRITE_BUFFER, 0, newSize, data);
  }
  size = newSize;
}

GLuint vbo_manager::get_vbo() const {
  return vbo;
}

size_t vbo_manager::get_size() const {
  return size;
}

parameter_window::parameter_window(const window2d& window, int horizontalAxisVariable,
                                   int verticalAxisVariable) :
    window(window), horizontalAxisVariable(horizontalAxisVariable),
    verticalAxisVariable(verticalAxisVariable) { }

model::model() : uniqueDynamicalId(0), uniqueParameterId(0),
                 parameterPosition(1, 0.0),
                 defaultWindow(window2d(point2d(500, 500),
                                        point2d(10, 10),
                                        point2d(-5, 5))),
                 compiled(false),
		 glInitialized(false) { }

model::~model() {
  if(glInitialized) {
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
  }
}


void model::add_initial_value_solution(const math::vector& initial,
                                       double tMin, double tMax,
                                       double increment) {
  assert(initial.size() == get_dynamical_variables());
  assert(initial[0] > tMin && initial[0] < tMax && increment > 0);
  assert(compiled);
  std::vector<math::vector> points;
  std::vector<math::vector>::size_type pointsCount =
      static_cast<std::vector<math::vector>::size_type>((tMax - tMin)/increment);
  points.resize(pointsCount, math::vector(get_dynamical_variables(), 0.0));
  std::vector<math::vector>::size_type initialIndex
      = static_cast<std::vector<math::vector>::size_type>(
          (initial[0] - tMin)/increment);
  std::vector<compiler::function<double, const double*>> systemFunctions;

  for(std::vector<expression>::const_iterator iter = system.begin();
      iter != system.end(); ++iter) {
    systemFunctions.push_back(iter->function);
  }
  
  std::vector<math::vector>::size_type i = initialIndex;
  math::vector tmp(initial);
  // Fill in forwards
  while(i < points.size() && tmp[0] <= tMax) {
    points[i] = tmp;
    math::euler(tmp, systemFunctions, parameterPosition, increment);
    ++i;
  }
  while(i != points.size()) {
    points[i] = points[i - 1];
    ++i;
  }
  i = initialIndex;
  tmp = initial;
  
  // Fill in backwards
  while(i > 0 && tmp[0] >= tMin) {
    points[i] = tmp;
    math::euler(tmp, systemFunctions, parameterPosition, -increment);
    --i;
  }
  while(i > 0) {
    points[i] = points[i + 1];
    --i;
  }
  points[i] = points[i + 1];

  int solutionId = ++uniqueSolutionId;
  solutions.insert(std::make_pair(solutionId,
                                  solution(initial, tMax, tMin, increment, points)));
  // We now need to update the VBO's of each window.
  for(std::unordered_map<dynamical_window_id, dynamical_window>::iterator iter
	= dynamicalWindows.begin(); iter != dynamicalWindows.end(); ++iter) {
    iter->second.generate_vbo(solutionId, solutions.at(solutionId).data);
  }
}
                    
bool model::compile(std::vector<std::string> newExpressions) {
  assert(newExpressions.size() >= 1);
  expressions = newExpressions;
  dynamicalDimension = static_cast<int>(expressions.size());
  // parameterVariables remains unchanged.

  for(std::unordered_map<int, dynamical_window>::iterator
          iter = dynamicalWindows.begin();
      iter != dynamicalWindows.end(); ++iter) {
    // For each dynamicalWindow
    iter->second.set_window(defaultWindow);
    iter->second.set_horizontal_axis_variable(0);
    iter->second.set_vertical_axis_variable(1);
  }
  for(std::unordered_map<int, parameter_window>::iterator
          iter = parameterWindows.begin();
      iter != parameterWindows.end(); ++iter) {
    // For each parameterWindow
    iter->second.window = defaultWindow;
    if(parameterVariables >= 2) {
      iter->second.horizontalAxisVariable = 0;
      iter->second.verticalAxisVariable = 1;
    } else if(parameterVariables == 1) {
      iter->second.horizontalAxisVariable = 0;
      iter->second.verticalAxisVariable = 0;
    } else {
      iter->second.horizontalAxisVariable = -1;
      iter->second.verticalAxisVariable = -1;
    }
  }
  
  system.clear();
  jacobian.clear();

  // Set up symbol table.
  std::list<symbol> symbolTable;
  if(dynamicalDimension <= 4) {
    switch(dynamicalDimension) {
      case 4:symbolTable.push_back(symbol("w",4,4));
      case 3:symbolTable.push_back(symbol("z",3,3));
      case 2:symbolTable.push_back(symbol("y",2,2));
      case 1:symbolTable.push_back(symbol("t",0,0));
             symbolTable.push_back(symbol("x",1,1));
    }
  } else {
    symbolTable.push_back(symbol("t",0,0));
    for(int i = 0; i != dynamicalDimension; ++i) {
      std::string str("x" + std::to_string(i + 1));
      symbolTable.push_back(symbol(str,i+1, static_cast<unsigned int>(i+1)));
    }
  }
  
  if(parameterVariables <= 4) {
    switch(parameterVariables) {
      case 4:
        symbolTable.push_back(symbol("d",dynamicalDimension + 4,dynamicalDimension + 4));
      case 3:
        symbolTable.push_back(symbol("c",dynamicalDimension + 3,dynamicalDimension + 3));
      case 2:
        symbolTable.push_back(symbol("b",dynamicalDimension + 2,dynamicalDimension + 2));
      case 1:
        symbolTable.push_back(symbol("a",dynamicalDimension + 1,dynamicalDimension + 1));
      default: break;//Do nothing
    }
  } else {
    for(int i = 0; i != parameterVariables; ++i) {
      std::string str("a" + std::to_string(i + 1));
      symbolTable.push_back(symbol(str, dynamicalDimension+1+i, dynamicalDimension + 1 + i));
    }
  }
  
  symbolTable.sort([](const symbol& a, const symbol& b) -> bool {
      return a.parameter < b.parameter;
    });
  
  compiler::expression_parser parse;
  for(std::vector<std::string>::const_iterator expr = expressions.begin();
      expr != expressions.end(); ++expr) {
    AST ast = parse.parse(*expr, symbolTable);
    compiler::function<double, const double*> function = ast.compile();
    system.push_back(expression{ast, function});
    for(symbol sym : symbolTable) {
      //We must check that the symbol is indeed a variable and not a parameter
      if(sym.parameter == 0 || sym.parameter >= get_dynamical_variables())
        continue;
      AST diffAst = AST(ast).differentiate(sym.name);
      compiler::function<double, const double*> diffFunction = diffAst.compile();
      jacobian.push_back(expression{diffAst, diffFunction});
    }
  }

  solutions.clear();
  parameterPosition = math::vector(parameterVariables, 0.0);
  compiled = true;
  return true;
}

int model::add_parameter_window(window2d window, int horizontalAxisVariable,
                                int verticalAxisVariable) {
  assert(horizontalAxisVariable < parameterVariables);
  assert(verticalAxisVariable < parameterVariables);
  parameterWindows.insert(
      std::make_pair(
          uniqueParameterId,
          parameter_window(window, horizontalAxisVariable, verticalAxisVariable)));
  return uniqueParameterId++;
}

int model::add_dynamical_window(window2d window, int horizontalAxisVariable,
                                int verticalAxisVariable) {
  assert(horizontalAxisVariable < get_dynamical_variables());
  assert(verticalAxisVariable < get_dynamical_variables());
  dynamicalWindows.insert(
      std::make_pair(
          uniqueDynamicalId,
          dynamical_window(window, horizontalAxisVariable, verticalAxisVariable,
			   vao, program, solutions)));
  return uniqueDynamicalId++;
}

void model::set_dynamical_axes(int horizontalVariable, int verticalVariable,
                               int dynamicalId) {
  assert(horizontalVariable < get_dynamical_variables());
  assert(verticalVariable < get_dynamical_variables());
  dynamicalWindows.at(dynamicalId).set_horizontal_axis_variable(horizontalVariable);
  dynamicalWindows.at(dynamicalId).set_vertical_axis_variable(verticalVariable);
}

void model::set_parameter_axes(int horizontalVariable, int verticalVariable,
                               int parameterId) {
  assert(horizontalVariable < parameterVariables);
  assert(verticalVariable < parameterVariables);
  parameterWindows.at(parameterId).horizontalAxisVariable = horizontalVariable;
  parameterWindows.at(parameterId).verticalAxisVariable = verticalVariable;
}

void model::set_parameters(int parameters) {
  parameterVariables = parameters;
}

int model::get_dynamical_variables() const {
  return dynamicalDimension + 1;
}

int model::get_dynamical_dimension() const {
  return dynamicalDimension;
}

void model::move_dynamical_window(int x, int y, int) { assert(false); }
void model::scale_dynamical_window(double scale, int x, int y, int) { assert(false); }
void model::move_parameter_window(int x, int y, int) { assert(false); }
void model::scale_parameter_window(double scale, int x, int y, int) { assert(false); }

void model::initialize_opengl() {
  glInitialized = true;

  // Construct the VAO and program objects for the solution renderer.
  program = gl::compile_program(constants::vertex_shader::kCode,
				constants::fragment_shader::kCode);
  glGenVertexArrays(1, &vao);
  glUseProgram(program);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(constants::vertex_shader::kPositionAttribute);
  glVertexAttribFormat(constants::vertex_shader::kPositionAttribute,
		       2, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(constants::vertex_shader::kPositionAttribute,
			constants::vertex_shader::kVertexBinding);
}

void model::paint_dynamical_window(dynamical_window_id id) {
  dynamicalWindows.at(id).paint();
}
void model::resize_dynamical_window(dynamical_window_id id) {
  dynamicalWindows.at(id).resize();
}

const dynamical_window& model::get_dynamical_window(dynamical_window_id id) const {
  // Dangerous if another window is added after returning!!
  return dynamicalWindows.at(id);
}
} // namespace gui
} // namespace dynslover
