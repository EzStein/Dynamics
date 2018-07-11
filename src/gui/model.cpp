#include "gui/model.h"

#include <vector>
#include <cassert>
#include <functional>

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
#include "math/window2d.h"
#include "math/vector2d.h"
#include "math/matrix_4x4.h"

namespace dynsolver {
namespace gui {

color::color(double r, double g, double b, double a)
  : r(r), g(g), b(b), a(a) { }

double color::get_red() const {
  return r;
}
double color::get_green() const {
  return g;
}
double color::get_blue() const {
  return b;
}
double color::get_alpha() const {
  return a;
}

void color::set_red(double red) {
  r = red;
}
void color::set_green(double green) {
  g = green;
}
void color::set_blue(double blue) {
  b = blue;
}
void color::set_alpha(double alpha) {
  a = alpha;
}

void color::invert() {
  r = 1.0 - r;
  g = 1.0 - g;
  b = 1.0 - b;
}

solution_specs::solution_specs() : glColor(0.5,0.5,0,1), init(1) { }

isocline_specs::isocline_specs() : glColor(0,0.5,0.5,1), init(1) { }

singular_point_specs::singular_point_specs() : init(1), glColor(0.5,0,0.5,1) { }

model::parameter_window::parameter_window(model& model,
					  const parameter_specs& spec)
  : modelData(model) {
}

void model::parameter_window::set_specs(const parameter_specs& spec) {
}

void model::parameter_window::set_viewport(const math::window2d&) {
}

void model::parameter_window::paint() {
}

void model::parameter_window::resize(int width, int height) {
}

bool model::parameter_window::on_vert_axis(const math::vector2d&) const {
}

bool model::parameter_window::on_horiz_axis(const math::vector2d&) const {
}

const parameter_specs& model::parameter_window::get_specs() const {
}

model::dynamical_window::dynamical_window(model& model,
					  const dynamical_specs& spec)
  : modelData(model), specs(spec),
    axesVbo(nullptr, 0, GL_DYNAMIC_DRAW),
    axesVboVertices(0),
    ticksPerLabel(5),
    tickFontSize(12),
    minPixelTickDist(15),
    maxPixelTickDist(minPixelTickDist * ticksPerLabel * 2),
    realTickDistanceX(0.5),
    realTickDistanceY(0.5) {
  // Generate VBO's for each solution and update the axes.
  for(solution_const_iter iter = modelData.solutions.begin();
      iter != modelData.solutions.end(); ++iter) {
    gen_solution_vbo(iter->first);
  }
  for(isocline_const_iter iter = modelData.isoclines.begin();
      iter != modelData.isoclines.end(); ++iter) {
    gen_isocline_vbo(iter->first);
  }
  update_axes_vbo();
}
const dynamical_specs& model::dynamical_window::get_specs() const {
  return specs;
}

void model::dynamical_window::set_specs(const dynamical_specs& other) {
  if(specs.horzAxisVar != other.horzAxisVar ||
     specs.vertAxisVar != other.vertAxisVar ||
     specs.xAxisVar != other.xAxisVar ||
     specs.yAxisVar != other.yAxisVar ||
     specs.zAxisVar != other.zAxisVar) {
    specs = other;
    // Update VBO's
    for(solution_const_iter iter = modelData.solutions.begin();
	iter != modelData.solutions.end(); ++iter) {
      gen_solution_vbo(iter->first);
    }
    for(isocline_const_iter iter = modelData.isoclines.begin();
	iter != modelData.isoclines.end(); ++iter) {
      gen_isocline_vbo(iter->first);
    }
  } else {
    specs = other;
  }
}

void model::dynamical_window::set_viewport_2d(const math::window2d& val) {
  specs.viewport2d = val;
}

void model::dynamical_window::set_viewport_3d(const math::window3d& val) {
  specs.viewport3d = val;
}

bool model::dynamical_window::select_solution(int x, int y, solution_id* id) {
  typedef std::list<math::vector>::const_iterator point_const_iter;
  if(specs.is3d) {
    return false;
  } else {
    for(model::solution_const_iter iter = modelData.solutions.begin();
	iter != modelData.solutions.end(); ++iter) {
      if(iter->second.data.size() <= 2) continue;
      point_const_iter point = iter->second.data.begin();
      ++point;
      point_const_iter prevPoint = iter->second.data.begin();
      for(; point != iter->second.data.end(); ++point, ++prevPoint) {
	math::vector2d real((*point)[specs.horzAxisVar],
			    (*point)[specs.vertAxisVar]);
	math::vector2d pixel(specs.viewport2d.pixel_of(real));
	math::vector2d realPrev((*prevPoint)[specs.horzAxisVar],
				(*prevPoint)[specs.vertAxisVar]);
	math::vector2d pixelPrev(specs.viewport2d.pixel_of(realPrev));
	const int tolerance(5);
	math::vector2d cursor(x, y);
	if(cursor.line_segment_distance(pixel, pixelPrev) <= tolerance) {
	  *id = iter->first;
	  return true;
	}
      }
    }
    return false;
  }
}

bool model::dynamical_window::select_isocline(int x, int y, isocline_id* id) {
  typedef std::list<math::vector>::const_iterator point_const_iter;
  if(specs.is3d) {
    return false;
  } else {
    for(model::isocline_const_iter iter = modelData.isoclines.begin();
	iter != modelData.isoclines.end(); ++iter) {
      if(iter->second.data.size() <= 2) continue;
      point_const_iter point = iter->second.data.begin();
      ++point;
      point_const_iter prevPoint = iter->second.data.begin();
      for(; point != iter->second.data.end(); ++point, ++prevPoint) {
	math::vector2d real((*point)[specs.horzAxisVar],
			    (*point)[specs.vertAxisVar]);
	math::vector2d pixel(specs.viewport2d.pixel_of(real));
	math::vector2d realPrev((*prevPoint)[specs.horzAxisVar],
				(*prevPoint)[specs.vertAxisVar]);
	math::vector2d pixelPrev(specs.viewport2d.pixel_of(realPrev));
	const int tolerance(5);
	math::vector2d cursor(x, y);
	if(cursor.line_segment_distance(pixel, pixelPrev) <= tolerance) {
	  *id = iter->first;
	  return true;
	}
      }
    }
    return false;
  }
}

bool model::dynamical_window::select_singular_point(int x,int y,
						    singular_point_id* id) {
  if(specs.is3d) {
    return false;
  } else {
    for(model::singular_point_const_iter iter = modelData.singularPoints.begin();
	iter != modelData.singularPoints.end(); ++iter) {
      math::vector2d real(iter->second.position[specs.horzAxisVar],
			  iter->second.position[specs.vertAxisVar]);
      math::vector2d pixel(specs.viewport2d.pixel_of(real));
      const double tolerance = 5;
      if(pixel.distance(math::vector2d(x, y)) <= tolerance) {
	*id = iter->first;
	return true;
      }
    }
    return false;
  }
}

void model::dynamical_window::draw_solutions_3d() {
  for(solution_data_const_iter iter = solutionData.begin();
      iter != solutionData.end(); ++iter) {
    color glColor(modelData.solutions.at(iter->first).specs.glColor);
    if(iter->first == modelData.selectedSolutionId) {
      glColor.invert();
    }
    glUniform4f(modelData.kPath3dColorUniformLocation, glColor.get_red(),
		glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
    glBindVertexBuffer(model::kPath3dVertexBinding,
		       iter->second.vbo3d.get_handle(), 0, 3 * sizeof(float));
    glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
  }
}
void model::dynamical_window::draw_isoclines_3d() {
  // We don't draw isoclines if we are viewing the time component.
  if(specs.xAxisVar != 0 && specs.yAxisVar != 0 && specs.zAxisVar != 0) {
    for(isocline_data_const_iter iter = isoclineData.begin();
	iter != isoclineData.end(); ++iter) {
      color glColor(modelData.isoclines.at(iter->first).specs.glColor);
      if(iter->first == modelData.selectedIsoclineId) {
	glColor.invert();
      }
      glUniform4f(modelData.kPath3dColorUniformLocation, glColor.get_red(),
		  glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
      glBindVertexBuffer(model::kPath3dVertexBinding,
			 iter->second.vbo3d.get_handle(), 0, 3 * sizeof(float));
      glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
    }
  }
}
void model::dynamical_window::draw_singular_points_3d() {
  /*  if(specs.xAxisVar != 0 && specs.yAxisVar != 0 && specs.zAxisVar != 0) {
    glUseProgram(modelData.program2d.get_handle());
    glBindVertexArray(modelData.vao2d.get_handle());
    glBindVertexBuffer(model::k2dVertexBinding,
		       modelData.circleVbo.get_handle(),
		       0, 2 * sizeof(float));
    for(singular_point_data_const_iter iter = singularPointData.begin();
	iter != singularPointData.end(); ++iter) {
      color glColor(modelData.singularPoints.at(iter->first).specs.glColor);
      if(iter->first == modelData.selectedSingularPointId) {
	glColor.invert();
      }
      glUniform4f(modelData.k2dColorUniformLocation, glColor.get_red(),
		  glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
      
      math::matrix_4x4 mat = math::matrix_4x4::translation(pixelCoordinate.x(),
							   pixelCoordinate.y());
      mat *= math::matrix_4x4::scale(6,6);
      GLfloat transformationMatrix[16];
      mat.as_float_array(transformationMatrix);
      glUniformMatrix4fv(modelData.k2dTransformationUniformLocation,
			 1, true, transformationMatrix);
      glDrawArrays(GL_LINE_LOOP, 0, modelData.circleVboVertices);
    }
    } */
}
void model::dynamical_window::draw_axes_3d() {
  GLfloat axes[4 * 3] = {
    0, 0, 0,
    0, 0, 1000,
    0, 1000, 0,
    1000, 0, 0};
  GLuint elements[2] = {
    0, 1
  };
  gl::buffer tmp(reinterpret_cast<unsigned char*>(axes),
		 4 * 3 * sizeof(float), GL_STATIC_DRAW);
  glBindVertexBuffer(model::kPath3dVertexBinding,
		     tmp.get_handle(), 0, 3 * sizeof(float));

  glUniform4f(modelData.kPath3dColorUniformLocation, 1.0, 0.0, 0.0, 1.0);
  glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_INT, elements);
  elements[1] = 2;
  glUniform4f(modelData.kPath3dColorUniformLocation, 0.0, 1.0, 0.0, 1.0);
  glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_INT, elements);
  elements[1] = 3;
  glUniform4f(modelData.kPath3dColorUniformLocation, 0.0, 0.0, 1.0, 1.0);
  glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_INT, elements);
}

void model::dynamical_window::draw_solutions_2d() {
  for(solution_data_const_iter iter = solutionData.begin();
      iter != solutionData.end(); ++iter) {
    color glColor(modelData.solutions.at(iter->first).specs.glColor);
    if(iter->first == modelData.selectedSolutionId) {
      glColor.invert();
    }
    glUniform4f(modelData.k2dColorUniformLocation, glColor.get_red(),
		glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
    glBindVertexBuffer(model::k2dVertexBinding,
		       iter->second.vbo2d.get_handle(), 0, 2 * sizeof(float));
    glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
  }
}
void model::dynamical_window::draw_isoclines_2d() {
  if(specs.horzAxisVar != 0 && specs.vertAxisVar != 0) {
    for(isocline_data_const_iter iter = isoclineData.begin();
	iter != isoclineData.end(); ++iter) {
      color glColor(modelData.isoclines.at(iter->first).specs.glColor);
      if(iter->first == modelData.selectedIsoclineId) {
	glColor.invert();
      }
      glUniform4f(modelData.k2dColorUniformLocation, glColor.get_red(),
		  glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
      glBindVertexBuffer(model::k2dVertexBinding,
			 iter->second.vbo2d.get_handle(), 0, 2 * sizeof(float));
      glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
    }
  }
}
void model::dynamical_window::draw_singular_points_2d() {
  // We don't draw it if we are viewing the t axis.
  if(specs.horzAxisVar != 0 && specs.vertAxisVar != 0) {
    glBindVertexBuffer(model::k2dVertexBinding,
		       modelData.circleVbo.get_handle(),
		       0, 2 * sizeof(float));
    for(model::singular_point_const_iter iter = modelData.singularPoints.begin();
	iter != modelData.singularPoints.end(); ++iter) {
      color glColor(modelData.singularPoints.at(iter->first).specs.glColor);
      if(iter->first == modelData.selectedSingularPointId) {
	glColor.invert();
      }
      glUniform4f(modelData.k2dColorUniformLocation,
		  glColor.get_red(),
		  glColor.get_green(),
		  glColor.get_blue(),
		  glColor.get_alpha());
      math::vector2d realCoordinate
	(iter->second.position[specs.horzAxisVar],
	 iter->second.position[specs.vertAxisVar]);
      GLfloat transformationMatrix[16];
      math::vector2d pixelCoordinate
	(specs.viewport2d.pixel_of(realCoordinate));
      (specs.viewport2d.pixel_to_ndc() *
       math::matrix_4x4::translation(pixelCoordinate.x(), pixelCoordinate.y()) *
       math::matrix_4x4::scale(6, 6)).as_float_array(transformationMatrix);
      glUniformMatrix4fv(modelData.k2dTransformationUniformLocation,
			 1, true, transformationMatrix);
      glDrawArrays(GL_LINE_LOOP, 0, modelData.circleVboVertices);
    }
  }
}
void model::dynamical_window::draw_axes_2d() {
  GLfloat transformationMatrix[16];
  // Render axes text.
  update_axes_vbo();
  // Generate pixel to ndc transform
  specs.viewport2d.pixel_to_ndc()
    .as_float_array(transformationMatrix);
  glUniformMatrix4fv(modelData.k2dTransformationUniformLocation,
		     1, true, transformationMatrix);
  // Set color to Black
  glUniform4f(modelData.k2dColorUniformLocation, 0.0f, 0.0f, 0.0f, 1.0f);
  glBindVertexBuffer(model::k2dVertexBinding, axesVbo.get_handle(),
		     0, 2 * sizeof(float));
  glDrawArrays(GL_LINES, 0, axesVboVertices);

  // Render Axes text
  const double tolerance = 0.001;
  math::vector2d axesPixel(specs.viewport2d
			   .pixel_of(math::vector2d(0.0, 0.0)));
  int yTextOffset(10);
  int xTextOffset(10);
  if(axesPixel.x() < 0.0) {
    axesPixel.x() = 0.0;
  }
  if(axesPixel.x() > specs.viewport2d.get_size().x()) {
    axesPixel.x() = specs.viewport2d.get_size().x();
    xTextOffset = -40;
  }
  if(axesPixel.y() < 0.0) {
    axesPixel.y() = 0.0;
  }
  if(axesPixel.y() > specs.viewport2d.get_size().y()) {
    axesPixel.y() = specs.viewport2d.get_size().y();
    yTextOffset = -20;
  }
  math::vector2d axesReal(specs.viewport2d
			  .real_coordinate_of(axesPixel));
  double xStart = realTickDistanceX * ticksPerLabel *
    static_cast<int>(specs.viewport2d.get_position().x() /
		     (realTickDistanceX * ticksPerLabel));
  double yStart = realTickDistanceY * ticksPerLabel *
    static_cast<int>(specs.viewport2d.get_position().y() /
		     (realTickDistanceY * ticksPerLabel));
  for(double x = xStart; x > specs.viewport2d.get_position().x();
      x -= ticksPerLabel * realTickDistanceX) {
    if(std::abs(x) < tolerance) continue;
    math::vector2d pixel(specs.viewport2d
			 .pixel_of(math::vector2d(x,
						  axesReal.y())));
    std::string text = util::double_to_string(x, 2);
    modelData.textRenderer.render_text(text, pixel.x() - text.size() * 3,
				       pixel.y() + yTextOffset,
				       modelData.font, tickFontSize);
  }
  for(double x = xStart; x < specs.viewport2d.get_position().x()
	+ specs.viewport2d.get_span().x();
      x += ticksPerLabel * realTickDistanceX) {
    if(std::abs(x) < tolerance) continue;
    math::vector2d pixel(specs.viewport2d
			 .pixel_of(math::vector2d(x, axesReal.y())));
    std::string text = util::double_to_string(x, 2);
    modelData.textRenderer.render_text(text, pixel.x() - text.size() * 3,
				       pixel.y() + yTextOffset,
				       modelData.font, tickFontSize);
  }
  for(double y = yStart; y > specs.viewport2d.get_position().y();
      y -= ticksPerLabel * realTickDistanceY) {
    if(std::abs(y) < tolerance) continue;
    math::vector2d pixel(specs.viewport2d
			 .pixel_of(math::vector2d(axesReal.x(),y)));
    std::string text = util::double_to_string(y, 2);
    modelData.textRenderer.render_text(text,
				       pixel.x() + xTextOffset,
				       pixel.y() - 5,
				       modelData.font,
				       tickFontSize);
  }
  for(double y = yStart; y < specs.viewport2d.get_position().y()
	+ specs.viewport2d.get_span().y();
      y += ticksPerLabel * realTickDistanceY) {
    if(std::abs(y) < tolerance) continue;
    math::vector2d pixel(specs.viewport2d
			 .pixel_of(math::vector2d(axesReal.x(),y)));
    std::string text = util::double_to_string(y, 2);
    modelData.textRenderer.render_text(text, pixel.x() + xTextOffset,
				       pixel.y() - 5, modelData.font,
				       tickFontSize);
  }
}

void model::dynamical_window::paint() {
  glViewport(0, 0, specs.viewport2d.get_size().x(),
	     specs.viewport2d.get_size().y());
  if(specs.is3d) {
    // Enable Depth filtering and writing
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    // Clear Buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set program, VAO and matrix transform
    glUseProgram(modelData.programPath3d.get_handle());
    glBindVertexArray(modelData.vaoPath3d.get_handle());
    float transformationMatrix[16];
    (specs.viewport3d.camera_to_clip() * specs.viewport3d.world_to_camera())
      .as_float_array(transformationMatrix);
    glUniformMatrix4fv(modelData.kPath3dTransformationUniformLocation,
		       1, true, transformationMatrix);

    // Render
    draw_axes_3d();
    draw_solutions_3d();
    draw_isoclines_3d();

    // Note that this call changes the transformation uniform.
    draw_singular_points_3d();
  } else {
    // Disable depth filtering
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    // Clear background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set program and transformation.
    glUseProgram(modelData.program2d.get_handle());
    glBindVertexArray(modelData.vao2d.get_handle());
    float transformationMatrix[16];
    specs.viewport2d.real_to_ndc().as_float_array(transformationMatrix);
    glUniformMatrix4fv(modelData.k2dTransformationUniformLocation,
		       1, true, transformationMatrix);

    draw_solutions_2d();
    draw_isoclines_2d();

    // Note that these calls changes the transformation uniform.
    draw_singular_points_2d();
    draw_axes_2d();
  }
}
  
void model::dynamical_window::resize(int width, int height) {
  double changeWidth = (width - specs.viewport2d.get_size().x())
    * specs.viewport2d.get_span().x() /
    specs.viewport2d.get_size().x();
  double changeHeight = (height - specs.viewport2d.get_size().y())
    * specs.viewport2d.get_span().y() /
    specs.viewport2d.get_size().y();
  specs.viewport2d.set_size(math::vector2d(width, height));
  specs.viewport2d
    .set_span(math::vector2d(changeWidth +
			     specs.viewport2d.get_span().x(),
			     changeHeight +
			     specs.viewport2d.get_span().y()));
  specs.viewport3d.set_width(width);
  specs.viewport3d.set_height(height);
}

bool model::dynamical_window::on_vertical_axis(int x, int y) const {
  math::vector2d axesPixel(specs.viewport2d
			   .pixel_of(math::vector2d(0.0, 0.0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > specs.viewport2d.get_size().x())
    axesPixel.x() = specs.viewport2d.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > specs.viewport2d.get_size().y())
    axesPixel.y() = specs.viewport2d.get_size().y();
  double axis = axesPixel.x();
  return axis - 5 < x && x < axis + 5;
}
bool model::dynamical_window::on_horizontal_axis(int x, int y) const {
  math::vector2d axesPixel(specs.viewport2d
			   .pixel_of(math::vector2d(0.0, 0.0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > specs.viewport2d.get_size().x())
    axesPixel.x() = specs.viewport2d.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > specs.viewport2d.get_size().y())
    axesPixel.y() = specs.viewport2d.get_size().y();
  double axis = axesPixel.y();
  return axis - 5 < y && y < axis + 5;
}

void model::dynamical_window::delete_solution(solution_id id) {
  solutionData.erase(id);
}

void model::dynamical_window::add_solution(solution_id id) {
  gen_solution_vbo(id);
}

void model::dynamical_window::add_isocline(isocline_id id) {
  gen_isocline_vbo(id);
}

void model::dynamical_window::delete_isocline(isocline_id id) {
  isoclineData.erase(id);
}



void model::dynamical_window::gen_solution_vbo(solution_id id) {
  const std::list<math::vector>& solution(modelData.solutions.at(id).data);
  std::vector<float> data2d;
  std::vector<float> data3d;
  size_t points = solution.size();
  size_t size2d = points * 2 * sizeof(float);
  size_t size3d = points * 3 * sizeof(float);
  for(std::list<math::vector>::const_iterator point = solution.begin();
      point != solution.end(); ++point) {
    data2d.push_back((*point)[specs.horzAxisVar]);
    data2d.push_back((*point)[specs.vertAxisVar]);

    data3d.push_back((*point)[specs.xAxisVar]);
    data3d.push_back((*point)[specs.yAxisVar]);
    data3d.push_back((*point)[specs.zAxisVar]);
  }
  // If the render Data already exists.
  if(solutionData.find(id) != solutionData.end()) {
    solutionData.at(id).vertices = points;
    solutionData.at(id)
      .vbo2d.set_data(reinterpret_cast<unsigned char *>(data2d.data()), size2d);
    solutionData.at(id)
      .vbo3d.set_data(reinterpret_cast<unsigned char *>(data3d.data()), size3d);
  } else {
    gl::buffer vbo2d(reinterpret_cast<unsigned char*>(data2d.data()),
		     size2d, GL_DYNAMIC_DRAW);
    gl::buffer vbo3d(reinterpret_cast<unsigned char*>(data3d.data()),
		     size3d, GL_DYNAMIC_DRAW);
    solution_data val{vbo2d, vbo3d, points};
    solutionData.insert(std::make_pair(id, val));
  }
}

void model::dynamical_window::gen_isocline_vbo(isocline_id id) {
  const std::list<math::vector>& isocline(modelData.isoclines.at(id).data);
  std::vector<float> data2d;
  std::vector<float> data3d;
  size_t points = isocline.size();
  size_t size2d = points * 2 * sizeof(float);
  size_t size3d = points * 3 * sizeof(float);
  for(std::list<math::vector>::const_iterator point = isocline.begin();
      point != isocline.end(); ++point) {
    data2d.push_back((*point)[specs.horzAxisVar]);
    data2d.push_back((*point)[specs.vertAxisVar]);

    data3d.push_back((*point)[specs.xAxisVar]);
    data3d.push_back((*point)[specs.yAxisVar]);
    data3d.push_back((*point)[specs.zAxisVar]);
  }
  // If the render Data already exists.
  if(isoclineData.find(id) != isoclineData.end()) {
    isoclineData.at(id).vertices = points;
    isoclineData.at(id)
      .vbo2d.set_data(reinterpret_cast<unsigned char *>(data2d.data()), size2d);
    isoclineData.at(id)
      .vbo3d.set_data(reinterpret_cast<unsigned char *>(data3d.data()), size3d);
  } else {
    gl::buffer vbo2d(reinterpret_cast<unsigned char*>(data2d.data()),
		     size2d, GL_DYNAMIC_DRAW);
    gl::buffer vbo3d(reinterpret_cast<unsigned char*>(data3d.data()),
		     size3d, GL_DYNAMIC_DRAW);
    isocline_data val{vbo2d, vbo3d, points};
    isoclineData.insert(std::make_pair(id, val));
  }
}

void model::dynamical_window::update_axes_vbo() {
  // Render the axes
  // Compute the distance between each tick.
  int numberOfTicksX =
    specs.viewport2d.get_span().x()/realTickDistanceX;
  int pixelTickDistanceX = specs.viewport2d.get_size().x()
    /static_cast<double>(numberOfTicksX);
  if(pixelTickDistanceX < minPixelTickDist) {
    realTickDistanceX *= ticksPerLabel;
  } else if(pixelTickDistanceX > maxPixelTickDist) {
    realTickDistanceX /= ticksPerLabel;
  }
  int numberOfTicksY =
    specs.viewport2d.get_span().y()/realTickDistanceY;
  int pixelTickDistanceY =
    specs.viewport2d.get_size().y()/static_cast<double>(numberOfTicksY);
  if(pixelTickDistanceY < minPixelTickDist) {
    realTickDistanceY *= ticksPerLabel;
  } else if(pixelTickDistanceY > maxPixelTickDist) {
    realTickDistanceY /= ticksPerLabel;
  }

  // The x and y values in pixels for where the x and y axes should cross.
  math::vector2d axesPixel
    (specs.viewport2d.pixel_of(math::vector2d(0.0, 0.0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > specs.viewport2d.get_size().x())
    axesPixel.x() = specs.viewport2d.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > specs.viewport2d.get_size().y())
    axesPixel.y() = specs.viewport2d.get_size().y();
  
  // Set up tick data in pixel coordinates.
  std::vector<float> axesVboData;
  double xStart = realTickDistanceX *
    static_cast<int>(specs.viewport2d.get_position().x()
		     / realTickDistanceX);
  double yStart = realTickDistanceY *
    static_cast<int>(specs.viewport2d.get_position().y()
		     / realTickDistanceY);
  for(double x = xStart;
      x < specs.viewport2d.get_position().x()
	+ specs.viewport2d.get_span().x(); x += realTickDistanceX) {
    float realCoordinate =
      specs.viewport2d.pixel_of(math::vector2d(x, 0)).x();
    // First vertex
    axesVboData.push_back(realCoordinate); // X
    axesVboData.push_back(axesPixel.y() - 5); // Y

    // Second vertex
    axesVboData.push_back(realCoordinate); // X
    axesVboData.push_back(axesPixel.y() + 5); // Y
  }
  for(double y = yStart; y < specs.viewport2d.get_position().y()
	+ specs.viewport2d.get_span().y();
      y += realTickDistanceY) {
    float realCoordinate =
      specs.viewport2d.pixel_of(math::vector2d(0, y)).y();
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
  
  axesVboData.push_back(specs.viewport2d.get_size().x());
  axesVboData.push_back(axesPixel.y());

  axesVboData.push_back(axesPixel.x());
  axesVboData.push_back(0);
  
  axesVboData.push_back(axesPixel.x());
  axesVboData.push_back(specs.viewport2d.get_size().y());
  
  axesVbo.set_data(reinterpret_cast<unsigned char*>(axesVboData.data()),
		   axesVboData.size()*sizeof(float));
  axesVboVertices = axesVboData.size() / 2;
}

const std::string model::k2dVertexShaderFilePath
(::dynsolver::app::generate_resource_path(std::vector<std::string>{"gl"},
					  "2d_renderer.vert"));
const GLuint model::k2dPositionAttribute(0);
const GLuint model::k2dVertexBinding(0);
const std::string model::k2dTransformationUniform("transformation");

const std::string model::k2dFragmentShaderFilePath
(::dynsolver::app::generate_resource_path(std::vector<std::string>{"gl"},
					  "2d_renderer.frag"));
const std::string model::k2dColorUniform("inColor");

const std::string model::kPath3dVertexShaderFilePath
(::dynsolver::app::generate_resource_path(std::vector<std::string>{"gl"},
					  "path_3d.vert"));
const GLuint model::kPath3dPositionAttribute(0);
const GLuint model::kPath3dVertexBinding(0);
const std::string model::kPath3dTransformationUniform("transformation");

const std::string model::kPath3dFragmentShaderFilePath
(::dynsolver::app::generate_resource_path(std::vector<std::string>{"gl"},
					  "path_3d.frag"));
const std::string model::kPath3dColorUniform("inColor");

std::vector<gl::shader> model::build_shaders_2d() {
  std::vector<gl::shader> shaders;
  shaders.push_back(gl::shader(util::read_file(model::k2dVertexShaderFilePath),
			       GL_VERTEX_SHADER));
  shaders.push_back(gl::shader(util::read_file(model::k2dFragmentShaderFilePath),
			       GL_FRAGMENT_SHADER));
  return shaders;
}

std::vector<gl::shader> model::build_shaders_path_3d() {
  std::vector<gl::shader> shaders;
  shaders.push_back(gl::shader(util::read_file(model::kPath3dVertexShaderFilePath),
			       GL_VERTEX_SHADER));
  shaders.push_back(gl::shader(util::read_file(model::kPath3dFragmentShaderFilePath),
			       GL_FRAGMENT_SHADER));
  return shaders;
}

const dynamical_id model::kNoDynamicalId = 0;
const parameter_id model::kNoParameterId = 0;
const solution_id model::kNoSolutionId = 0;
const singular_point_id model::kNoSingularPointId = 0;
const isocline_id model::kNoIsoclineId = 0;

model::model() : font(constants::kDefaultFontFilePath),
		 dynamicalDimension(0),
		 selectedSolutionId(kNoSolutionId),
		 selectedSingularPointId(kNoSingularPointId),
		 selectedIsoclineId(kNoIsoclineId),
		 uniqueDynamicalId(kNoDynamicalId),
		 uniqueParameterId(kNoParameterId),
		 uniqueSolutionId(kNoSolutionId),
		 uniqueSingularPointId(kNoSingularPointId),
		 uniqueIsoclineId(kNoIsoclineId),
		 compiled(false),
		 program2d(build_shaders_2d()),
		 programPath3d(build_shaders_path_3d()),
		 k2dTransformationUniformLocation
		 (glGetUniformLocation(program2d.get_handle(),
				       k2dTransformationUniform.c_str())),
		 k2dColorUniformLocation
		 (glGetUniformLocation(program2d.get_handle(),
				       k2dColorUniform.c_str())),
		 kPath3dTransformationUniformLocation
		 (glGetUniformLocation(programPath3d.get_handle(),
				       kPath3dTransformationUniform.c_str())),
		 kPath3dColorUniformLocation
		 (glGetUniformLocation(programPath3d.get_handle(),
				       kPath3dColorUniform.c_str())),
		 circleVbo(nullptr, 0, GL_STATIC_DRAW) {
  glUseProgram(program2d.get_handle());
  glBindVertexArray(vao2d.get_handle());
  glEnableVertexAttribArray(k2dPositionAttribute);
  glVertexAttribFormat(k2dPositionAttribute,
		       2, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(k2dPositionAttribute,
			k2dVertexBinding);
  glUseProgram(programPath3d.get_handle());
  glBindVertexArray(vaoPath3d.get_handle());
  glEnableVertexAttribArray(kPath3dPositionAttribute);
  glVertexAttribFormat(kPath3dPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(kPath3dPositionAttribute, kPath3dVertexBinding);
  glBindVertexArray(0);

  // Initialize circle vbo.
  circleVboVertices = 107;
  const int vertices = circleVboVertices - 6;
  const double pi = 3.14159265359;
  std::vector<GLfloat> data;
  for(int i = 0; i != vertices; ++i) {
    double angle = 2 * pi *
      static_cast<double>(i) / static_cast<double>(vertices);
    data.push_back(std::cos(angle));
    data.push_back(std::sin(angle));
  }
  data.push_back(1.0);
  data.push_back(0.0);
  
  data.push_back(-1.0);
  data.push_back(0.0);
  
  data.push_back(0.0);
  data.push_back(0.0);
  
  data.push_back(0.0);
  data.push_back(1.0);
  
  data.push_back(0.0);
  data.push_back(-1.0);
  
  data.push_back(0.0);
  data.push_back(0.0);
  circleVbo.set_data(reinterpret_cast<unsigned char*>(data.data()),
		     circleVboVertices * 2 * sizeof(GLfloat));
}

void model::set_no_compile() {
  compiled = false;
  dynamicalWindows.clear();
  solutions.clear();
  singularPoints.clear();
  system.clear();
  jacobian.clear();
  expressions.clear();
}


const std::unordered_map<singular_point_id, singular_point>&
model::get_singular_points() const {
  return singularPoints;
}

void model::add_solution(const solution_specs& spec) {
  assert(spec.init.size() == get_dynamical_vars());
  assert(spec.init[0] > spec.tMin && spec.init[0] < spec.tMax
	 && spec.inc > 0);
  assert(compiled);
  

  ++uniqueSolutionId;
  solutions.insert(std::make_pair(uniqueSolutionId,
				  solution{spec, std::list<math::vector>()}));
  generate_solution_data(uniqueSolutionId);
  
  // We now need to update the VBO's of each window.
  for(dynamical_iter iter = dynamicalWindows.begin();
      iter != dynamicalWindows.end(); ++iter) {
    iter->second.add_solution(uniqueSolutionId);
  }
}

bool model::add_singular_point(const singular_point_specs& spec) {
  std::vector<compiler::function<double, const double*>> systemFunctions;
  for(std::vector<expression>::const_iterator iter = system.begin();
      iter != system.end(); ++iter) {
    systemFunctions.push_back(iter->function);
  }
  std::vector<compiler::function<double, const double*>> jacobianFunctions;
  for(std::vector<expression>::const_iterator iter = jacobian.begin();
      iter != jacobian.end(); ++iter) {
    jacobianFunctions.push_back(iter->function);
  }
  
  const double tolerance = 0.0001;
  std::vector<size_t> indices;
  for(int i = 0; i != dynamicalDimension; ++i) {
    indices.push_back(i+1);
  }
  math::vector init(spec.init);
  if(!math::newton_method(init, systemFunctions,
			  jacobianFunctions, indices)) {
    return false;
  }
  // We now check to see if the singular point already exists.
  for(singular_point_const_iter iter = singularPoints.begin();
      iter != singularPoints.end(); ++iter) {
    if(iter->second.position.distance(init) < tolerance) {
      // Already exists
      return true;
    }
  }
  ++uniqueSingularPointId;
  singularPoints.insert(std::make_pair(uniqueSingularPointId,
				       singular_point{spec, init}));
  return true;
}

void model::delete_singular_point(singular_point_id id) {
  singularPoints.erase(id);
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

  for(int i = 0; i != 5; ++i) {
    std::string str("a" + std::to_string(i + 1));
    symbolTable.push_back(symbol(str, newDynamicalDimension+1+i,
				 newDynamicalDimension + 1 + i));
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
      if(sym.parameter == 0 || sym.parameter >= newDynamicalDimension + 1)
        continue;
      AST diffAst = AST(ast).differentiate(sym.name);
      compiler::function<double, const double*> diffFunction
	= diffAst.compile();
      newJacobian.push_back(expression{diffAst, diffFunction});
    }
  }

  // We have successfully compiled!

  system = newSystem;
  jacobian = newJacobian;

  expressions = newExpressions;
  dynamicalDimension = newDynamicalDimension;

  for(dynamical_iter iter = dynamicalWindows.begin();
      iter != dynamicalWindows.end(); ++iter) {
    for(solution_const_iter sol = solutions.begin();
	sol != solutions.end(); ++sol) {
      iter->second.delete_solution(sol->first);
    }
    for(isocline_const_iter iso = isoclines.begin();
	iso != isoclines.end(); ++iso) {
      iter->second.delete_solution(iso->first);
    }
  }
  solutions.clear();
  isoclines.clear();
  singularPoints.clear();
  compiled = true;
  return true;
}

dynamical_id model::add_dynamical(const dynamical_specs& spec) {
  ++uniqueDynamicalId;
  dynamicalWindows.insert(std::make_pair(uniqueDynamicalId,
					 dynamical_window(*this, spec)));
  return uniqueDynamicalId;
}

parameter_id model::add_parameter(const parameter_specs& spec) {
  ++uniqueParameterId;
  parameterWindows.insert(std::make_pair(uniqueParameterId,
					 parameter_window(*this, spec)));
  return uniqueParameterId;
}

void model::set_dynamical_specs(dynamical_id id,
				const dynamical_specs& spec) {
  dynamicalWindows.at(id).set_specs(spec);
}

void model::set_parameter_specs(parameter_id id,
				const parameter_specs& spec) {
  parameterWindows.at(id).set_specs(spec);
}

void model::delete_dynamical(dynamical_id id) {
  dynamicalWindows.erase(id);
}

void model::delete_parameter(parameter_id id) {
  parameterWindows.erase(id);
}

int model::get_dynamical_vars() const {
  return dynamicalDimension + 1;
}

int model::get_dynamical_dimension() const {
  return dynamicalDimension;
}

void model::paint_dynamical(dynamical_id id) {
  dynamicalWindows.at(id).paint();
}
void model::resize_dynamical(dynamical_id id, int width, int height) {
  dynamicalWindows.at(id).resize(width, height);
}

void model::paint_parameter(parameter_id id) {
  parameterWindows.at(id).paint();
}

void model::resize_parameter(parameter_id id, int width, int height) {
  parameterWindows.at(id).resize(width, height);
}

void model::set_dynamical_viewport_2d(dynamical_id id,
				      const math::window2d& window) {
  dynamicalWindows.at(id).set_viewport_2d(window);
}

void model::set_dynamical_viewport_3d(dynamical_id id,
				      const math::window3d& window) {
  dynamicalWindows.at(id).set_viewport_3d(window);
}

void model::set_parameter_viewport(parameter_id id,
				   const math::window2d &window) {
  parameterWindows.at(id).set_viewport(window);
}

bool model::on_dynamical_vertical_axis(dynamical_id id,
				       int x, int y) const {
  return dynamicalWindows.at(id).on_vertical_axis(x, y);
}
bool model::on_dynamical_horizontal_axis(dynamical_id id,
					 int x, int y) const {
  return dynamicalWindows.at(id).on_horizontal_axis(x, y);
}

bool model::on_parameter_horiz_axis(parameter_id id,
				    const math::vector2d& pos) const {
  return parameterWindows.at(id).on_horiz_axis(pos);
}

bool model::on_parameter_vert_axis(parameter_id id,
				    const math::vector2d& pos) const {
  return parameterWindows.at(id).on_vert_axis(pos);
}

void model::set_solution_specs(solution_id id,
			       const solution_specs& spec) {
  solutions.at(id).specs = spec;
  generate_solution_data(id);
  for(std::unordered_map<dynamical_id, dynamical_window>::iterator iter
	= dynamicalWindows.begin(); iter != dynamicalWindows.end(); ++iter) {
    iter->second.add_solution(id);
  }
}

void model::delete_isocline(isocline_id id) {
  isoclines.erase(id);
  for(dynamical_iter iter = dynamicalWindows.begin();
      iter != dynamicalWindows.end(); ++iter) {
    iter->second.delete_isocline(id);
  }
}

void model::delete_solution(solution_id id) {
  solutions.erase(id);
  for(std::unordered_map<dynamical_id, dynamical_window>::iterator iter
	= dynamicalWindows.begin(); iter != dynamicalWindows.end(); ++iter) {
    iter->second.delete_solution(id);
  }
}

void model::generate_solution_data(solution_id id) {
  solution_specs spec(solutions.at(id).specs);

  std::vector<compiler::function<double, const double*>> systemFunctions;
  for(std::vector<expression>::const_iterator iter = system.begin();
      iter != system.end(); ++iter) {
    systemFunctions.push_back(iter->function);
  }
  
  std::list<math::vector> points;
  math::vector tmp(spec.init);
  math::vector parameterPosition(5, 0.0);

  std::vector<size_t> indices;
  for(int i = 0; i != dynamicalDimension; ++i) {
    indices.push_back(i+1);
  }
  
  // Fill in forwards
  while(tmp[0] <= spec.tMax) {
    points.push_back(tmp);
    math::euler(tmp, systemFunctions,
		indices, 0, spec.inc);
  }
  
  tmp = spec.init;
  // Fill in backwards
  while(tmp[0] >= spec.tMin) {
    points.push_front(tmp);
    math::euler(tmp, systemFunctions,
		indices, 0, -spec.inc);
  }

  // Set the data
  solutions.at(id).data = points;
}

bool model::select_solution(dynamical_id id, int x, int y) {
  return dynamicalWindows.at(id).select_solution(x, y, &selectedSolutionId);
}

bool model::select_isocline(dynamical_id id, int x, int y) {
  return dynamicalWindows.at(id).select_isocline(x, y, &selectedIsoclineId);
}

bool model::select_singular_point(dynamical_id id, int x, int y) {
  return dynamicalWindows.at(id)
    .select_singular_point(x, y, &selectedSingularPointId);
}

void model::deselect_solution() {
  selectedSolutionId = kNoSolutionId;
}

void model::deselect_isocline() {
  selectedIsoclineId = kNoIsoclineId;
}

void model::deselect_singular_point() {
  selectedSingularPointId = kNoSingularPointId;
}

void model::select_solution(solution_id id) {
  selectedSolutionId = id;
}
void model::select_singular_point(singular_point_id id) {
  selectedSingularPointId = id;
}
void model::select_isocline(isocline_id id) {
  selectedIsoclineId = id;
}

solution_id model::get_selected_solution() const {
  return selectedSolutionId;
}

isocline_id model::get_selected_isocline() const {
  return selectedIsoclineId;
}

singular_point_id model::get_selected_singular_point() const {
  return selectedSingularPointId;
}

const std::unordered_map<solution_id, solution>& model::get_solutions() const {
  return solutions;
}

const std::unordered_map<isocline_id, isocline>& model::get_isoclines() const {
  return isoclines;
}

bool model::add_isocline(const isocline_specs& specs) {
  // Let n be the dimension of the system.
  // We solve the equation g(x1, x2, ..., xn, k) = F(x) - ka = 0 for x and k
  // where x is an n dimensional vector of the system and k is a scalar
  // multiple. a is the vector indicating the direction of flow through the
  // isocline we are trying to find. Thus we have a system of n + 1 unknowns and
  // n constraints. We must choose a further constraint by limiting one of the
  // variables to a given value and computing the others. For instance we
  // arbitrarily choose to add the equation x1 - m = 0 for some value m.
  //
  // The initial vector is of size n + 2 and is organized as follows.
  // initial[0] is the variable of differentiation
  // initial[1] to initial[n] inclusive are the dynamical variables.
  // initial[n+1] is the variable k.
  //
  // The systemFunctions are organized as
  // systemFunctions[0] to systemFunctions[n - 1] inclusive are the
  // n system functions expressing the vector equation F(x) - ka = 0
  // systemFunctions[n] is the function expressing the additional constraint.
  //
  // jacobianSystem has the jacobian of F(x) as a submatrix aligned against
  // the top left corner. The last column is the k partial derivative of each
  // function so this amounts to -a as the last column. If the restriction
  // functions is of the form x_l - m = 0 then the last row are all
  // zeros  except in the entry l - 1 which is a 1.
  //
  // The indices vector is simply a vector containing the integers,
  // 1 2 3 4 ... n+1
  
  std::vector<std::function<double(const double*)>> systemFunctions;
  std::vector<std::function<double(const double*)>> jacobianFunctions;
  int dimension = get_dynamical_dimension();
  for(int i = 0; i != dimension; ++i) {
    systemFunctions.push_back([=](const double* arr) -> double {
	return system[i].function(arr) - arr[dimension+1] * specs.direction[i];
      });
  }
  int constraintVar(1);
  double constant(specs.init[constraintVar - 1]);
  systemFunctions.push_back([=](const double* arr) -> double {
      return arr[constraintVar] - constant;
    });
  for(int r = 0; r != dimension; ++r) {
    for(int c = 0; c != dimension; ++c) {
      jacobianFunctions.push_back
	(std::function<double(const double*)>
	 (jacobian[r*dimension + c].function));
    }
    jacobianFunctions.push_back([=](const double*) -> double {
	return -specs.direction[r];
      });
  }
  for(int i = 0; i != dimension+1; ++i) {
    if(i == constraintVar - 1) {
      jacobianFunctions.push_back([](const double*) -> double {
	  return 1;
	});
    } else {
      jacobianFunctions.push_back([](const double*) -> double {
	  return 0;
	});
    }
  }
  math::vector init(specs.init.size() + 2);
  init[0] = 0;
  for(int i = 0; i != specs.init.size(); ++i) {
    init[i + 1] = specs.init[i];
  }
  init[init.size() - 1] = 1;
  math::vector original(init);
  math::vector prev(init);
  std::vector<size_t> indices;
  for(int i = 0; i != dimension + 1; ++i) {
    indices.push_back(i + 1);
  }
  std::list<math::vector> points;
  double change = 0.01;
  for(int j = 0; j != 2; ++j) {
    for(int i = 0; i != 10000; ++i) {
      systemFunctions[dimension] = [=](const double* arr) -> double {
	return arr[constraintVar] - constant;
      };
      math::vector newInit(init);
      if(!newton_method(newInit, systemFunctions, jacobianFunctions, indices)) {
	jacobianFunctions[dimension * (dimension + 1) + constraintVar - 1] =
	  [](const double*) -> double {
	  return 0;
	};
	if(constraintVar < dimension) {
	  ++constraintVar;
	} else {
	  constraintVar = 1;
	}
	jacobianFunctions[dimension * (dimension + 1) + constraintVar - 1] =
	  [](const double*) -> double {
	  return 1;
	};
	if(init[constraintVar] - prev[constraintVar] > 0) {
	  std::cout << "PLUS" << std::endl;
	  change = std::abs(change);
	} else {
	  std::cout << "MINUS" << std::endl;
	  change = -std::abs(change);
	}
	constant = prev[constraintVar];
	constant += change;
	systemFunctions[dimension] = [=](const double* arr) -> double {
	  return arr[constraintVar] - constant;
	};
      } else {
	prev = init;
	init = newInit;
	points.push_back(init);
      }
      constant += change;
    }
    jacobianFunctions[dimension * (dimension + 1) + constraintVar - 1] =
      [](const double*) -> double {
      return 0;
    };
    change = -0.01;
    constraintVar = 1;
    constant = specs.init[0];
    init = original;
    prev = original;
    points.reverse();
    jacobianFunctions[dimension * (dimension + 1) + constraintVar - 1] =
      [](const double*) -> double {
      return 1;
    };

  }
  ++uniqueIsoclineId;
  isoclines.insert(std::make_pair(uniqueIsoclineId, isocline{specs, points}));
  // We now need to update the VBO's of each window.
  for(dynamical_iter iter = dynamicalWindows.begin();
      iter != dynamicalWindows.end(); ++iter) {
    iter->second.add_isocline(uniqueIsoclineId);
  }
  return true;
}

const dynamical_specs& model::get_dynamical_specs(dynamical_id id) const {
  return dynamicalWindows.at(id).get_specs();
}

const parameter_specs& model::get_parameter_specs(parameter_id id) const {
  return parameterWindows.at(id).get_specs();
}
} // namespace gui
} // namespace dynslover
