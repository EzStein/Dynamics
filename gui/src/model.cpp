#include "gui/model.h"

#include <vector>
#include <cassert>
#include <functional>

#include <glad/glad.h>
#include <wx/glcanvas.h>
#include "math/eigenvalue.h"

#include "parser/syntax_exception.h"
#include "compiler/expression_parser.h"
#include "math/util.h"
#include "math/vector.h"
#include "gl/shader.h"
#include "util/util.h"
#include "gui/app.h"
#include "math/window2d.h"
#include "math/vector2d.h"
#include "math/matrix_4x4.h"

namespace dynsolver {
namespace gui {

gen_singular_point_ret::gen_singular_point_ret()
  : singularPoint(), success(false) { }

gen_singular_point_ret::gen_singular_point_ret(const singular_point& singularPoint,
					       bool success)
  : singularPoint(singularPoint), success(success) { }

gen_separatrix_ret::gen_separatrix_ret() : success(false) { }
gen_separatrix_ret::gen_separatrix_ret(const singular_point& singularPoint,
				       const separatrix& sep,
				       bool success)
  : singularPoint(singularPoint), sep(sep), success(success) { }


find_separatrix_intersection_ret::find_separatrix_intersection_ret()
  : success(false) { }

find_separatrix_intersection_ret::find_separatrix_intersection_ret
(const ::math::vector2d intersection, bool success) : intersection(intersection),
						    success(success) { }

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

isocline_specs::isocline_specs() : init(1), glColor(0,0.5,0.5,1) { }

singular_point_specs::singular_point_specs() : init(1), glColor(0.5,0,0.5,1) { }

separatrix_specs::separatrix_specs() : glColor(0.5, 0.5, 0, 1) { }

hopf_bifurcation_specs::hopf_bifurcation_specs() : glColor(1.0, 0, 0, 1) { }

saddle_node_bifurcation_specs::saddle_node_bifurcation_specs()
  : glColor(0, 0, 1.0, 1) { }

limit_cycle_bifurcation_specs::limit_cycle_bifurcation_specs()
  : glColor(1.0, 1.0, 0, 1) { }

saddle_connection_bifurcation_specs::saddle_connection_bifurcation_specs()
  : glColor(0, 1.0, 1.0, 1) { }

model::parameter_window::parameter_window(model& model,
					  const parameter_specs& specs)
  : modelData(model), specs(specs),
    realTickDistanceX(0.5),
    realTickDistanceY(0.5),
    axesVbo(nullptr, 0, GL_DYNAMIC_DRAW),
    ticksPerLabel(5),
    tickFontSize(12),
    crossVbo(nullptr, 0, GL_STATIC_DRAW),
    crossVboVertices(4) {
  // Draw the parameter position cross hairs.
  GLfloat crossData[8] = { -1, -1, 1, 1,
			   -1, 1, 1, -1 };
  crossVbo.set_data(reinterpret_cast<unsigned char*>(crossData),
		    8 * sizeof(float));
}

void model::parameter_window::gen_hopf_bifurcation_vbo(hopf_bifurcation_id id) {
  int horizIndex = modelData.parameterIndex.at(specs.horizAxisVar);
  int vertIndex = modelData.parameterIndex.at(specs.vertAxisVar);
  std::vector<float> vboData;
  const std::list<::math::vector>& data
    = modelData.hopfBifurcations.at(id).data;
  for(std::list<::math::vector>::const_iterator iter = data.begin();
      iter != data.end(); ++iter) {
    vboData.push_back((*iter)[horizIndex]);
    vboData.push_back((*iter)[vertIndex]);
  }
  if(hopfBifurcationData.find(id) == hopfBifurcationData.end()) {
    gl::buffer buffer =
      gl::buffer(reinterpret_cast<unsigned char*>(vboData.data()),
		 data.size() * 2 * sizeof(float), GL_STATIC_DRAW);
    render_data renderData{buffer, static_cast<GLsizei>(data.size())};
    hopfBifurcationData.insert(std::make_pair(id, renderData));
  } else {
    hopfBifurcationData.at(id).vbo
      .set_data(reinterpret_cast<unsigned char*>(vboData.data()),
		data.size() * 2 * sizeof(float));
    hopfBifurcationData.at(id).vertices = data.size();
  }
}

void model::parameter_window::gen_saddle_node_bifurcation_vbo(saddle_node_bifurcation_id id) {
  int horizIndex = modelData.parameterIndex.at(specs.horizAxisVar);
  int vertIndex = modelData.parameterIndex.at(specs.vertAxisVar);
  std::vector<float> vboData;
  const std::list<::math::vector>& data
    = modelData.saddleNodeBifurcations.at(id).data;
  for(std::list<::math::vector>::const_iterator iter = data.begin();
      iter != data.end(); ++iter) {
    vboData.push_back((*iter)[horizIndex]);
    vboData.push_back((*iter)[vertIndex]);
  }
  if(saddleNodeBifurcationData.find(id) == saddleNodeBifurcationData.end()) {
    gl::buffer buffer =
      gl::buffer(reinterpret_cast<unsigned char*>(vboData.data()),
		 data.size() * 2 * sizeof(float), GL_STATIC_DRAW);
    render_data renderData{buffer, static_cast<GLsizei>(data.size())};
    saddleNodeBifurcationData.insert(std::make_pair(id, renderData));
  } else {
    saddleNodeBifurcationData.at(id).vbo
      .set_data(reinterpret_cast<unsigned char*>(vboData.data()),
		data.size() * 2 * sizeof(float));
    saddleNodeBifurcationData.at(id).vertices = data.size();
  }
}

void model::parameter_window::gen_limit_cycle_bifurcation_vbo(limit_cycle_bifurcation_id id) {
  int horizIndex = modelData.parameterIndex.at(specs.horizAxisVar);
  int vertIndex = modelData.parameterIndex.at(specs.vertAxisVar);
  std::vector<float> vboData;
  const std::list<::math::vector>& data
    = modelData.limitCycleBifurcations.at(id).data;
  for(std::list<::math::vector>::const_iterator iter = data.begin();
      iter != data.end(); ++iter) {
    vboData.push_back((*iter)[horizIndex]);
    vboData.push_back((*iter)[vertIndex]);
  }
  if(limitCycleBifurcationData.find(id) == limitCycleBifurcationData.end()) {
    gl::buffer buffer =
      gl::buffer(reinterpret_cast<unsigned char*>(vboData.data()),
		 data.size() * 2 * sizeof(float), GL_STATIC_DRAW);
    render_data renderData{buffer, static_cast<GLsizei>(data.size())};
    limitCycleBifurcationData.insert(std::make_pair(id, renderData));
  } else {
    limitCycleBifurcationData.at(id).vbo
      .set_data(reinterpret_cast<unsigned char*>(vboData.data()),
		data.size() * 2 * sizeof(float));
    limitCycleBifurcationData.at(id).vertices = data.size();
  }
}


void model::parameter_window::gen_saddle_connection_bifurcation_vbo(saddle_connection_bifurcation_id id) {
  int horizIndex = modelData.parameterIndex.at(specs.horizAxisVar);
  int vertIndex = modelData.parameterIndex.at(specs.vertAxisVar);
  std::vector<float> vboData;
  const std::list<::math::vector>& data
    = modelData.saddleConnectionBifurcations.at(id).data;
  for(std::list<::math::vector>::const_iterator iter = data.begin();
      iter != data.end(); ++iter) {
    vboData.push_back((*iter)[horizIndex]);
    vboData.push_back((*iter)[vertIndex]);
  }
  if(saddleConnectionBifurcationData.find(id) == saddleConnectionBifurcationData.end()) {
    gl::buffer buffer =
      gl::buffer(reinterpret_cast<unsigned char*>(vboData.data()),
		 data.size() * 2 * sizeof(float), GL_STATIC_DRAW);
    render_data renderData{buffer, static_cast<GLsizei>(data.size())};
    saddleConnectionBifurcationData.insert(std::make_pair(id, renderData));
  } else {
    saddleConnectionBifurcationData.at(id).vbo
      .set_data(reinterpret_cast<unsigned char*>(vboData.data()),
		data.size() * 2 * sizeof(float));
    saddleConnectionBifurcationData.at(id).vertices = data.size();
  }
}

void model::parameter_window::delete_hopf_bifurcation(hopf_bifurcation_id id) {
  hopfBifurcationData.erase(id);
}
void model::parameter_window::delete_saddle_node_bifurcation(saddle_node_bifurcation_id id) {
  saddleNodeBifurcationData.erase(id);
}
void model::parameter_window::delete_saddle_connection_bifurcation(saddle_connection_bifurcation_id id) {
  saddleConnectionBifurcationData.erase(id);
}
void model::parameter_window::delete_limit_cycle_bifurcation(limit_cycle_bifurcation_id id) {
  limitCycleBifurcationData.erase(id);
}

bool model::parameter_window::select_hopf_bifurcation(int x, int y, hopf_bifurcation_id*) {
  return false;
}

bool model::parameter_window::select_saddle_node_bifurcation(int x, int y, saddle_node_bifurcation_id*) {
  return false;
}

bool model::parameter_window::select_limit_cycle_bifurcation(int x, int y, limit_cycle_bifurcation_id*) {
  return false;
}

bool model::parameter_window::select_saddle_connection_bifurcation(int x, int y, saddle_connection_bifurcation_id*) {
  return false;
}

::math::vector model::parameter_window::get_point(const ::math::vector2d& pos) const {
  ::math::vector ret(modelData.parameters, 0.0);
  ::math::vector2d real(specs.viewport.real_coordinate_of(pos));
  ret[modelData.parameterIndex.at(specs.horizAxisVar)] = real[0];
  ret[modelData.parameterIndex.at(specs.vertAxisVar)] = real[1];
  return ret;
}

bool model::parameter_window::on_parameter_position(const ::math::vector2d& pos) const {
  double x = modelData.parameterPosition[modelData.parameterIndex
					 .at(specs.horizAxisVar)];
  double y = modelData.parameterPosition[modelData.parameterIndex
					 .at(specs.vertAxisVar)];
  ::math::vector2d pixelParamPos(specs.viewport.pixel_of(::math::vector2d(x, y)));
  return pos.distance(pixelParamPos) < 5;
}

void model::parameter_window::set_specs(const parameter_specs& newSpecs) {
  if(specs.horizAxisVar != newSpecs.horizAxisVar ||
     specs.vertAxisVar != newSpecs.vertAxisVar) {
    specs = newSpecs;
    // Update VBO's
  } else {
    specs = newSpecs;
  }
}

void model::parameter_window::set_viewport(const ::math::window2d& viewport) {
  specs.viewport = viewport;
}

void model::parameter_window::paint() {
  glViewport(0, 0, specs.viewport.get_size().x(),
	     specs.viewport.get_size().y());
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  modelData.draw_axes_2d(specs.viewport, ticksPerLabel,
			 tickFontSize,
			 realTickDistanceX, realTickDistanceY,
			 axesVbo);

  glUseProgram(modelData.program2d.get_handle());
  glBindVertexArray(modelData.vao2d.get_handle());

  double realParamX =
    modelData.parameterPosition[modelData.parameterIndex.at(specs.horizAxisVar)];
  double realParamY =
    modelData.parameterPosition[modelData.parameterIndex.at(specs.vertAxisVar)];
  ::math::vector2d paramPixel(specs.viewport
			    .pixel_of(::math::vector2d(realParamX, realParamY)));
  GLfloat transformationMatrix[16];
  (specs.viewport.pixel_to_ndc() *
   ::math::matrix_4x4::translation(paramPixel.x(), paramPixel.y()) *
   ::math::matrix_4x4::scale(6,6)).as_float_array(transformationMatrix);
  glUniformMatrix4fv(modelData.k2dTransformationUniformLocation,
		     1, true, transformationMatrix);
#ifdef GL_VERSION_4_3
  glBindVertexBuffer(model::k2dVertexBinding, crossVbo.get_handle(),
		     0, 2 * sizeof(float));
#else
  glBindBuffer(GL_ARRAY_BUFFER, crossVbo.get_handle());
  glVertexAttribPointer(k2dPositionAttribute, 2, GL_FLOAT,
                        GL_FALSE, 0, reinterpret_cast<void*>(0));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
  glDrawArrays(GL_LINES, 0, 4);

  specs.viewport.real_to_ndc().as_float_array(transformationMatrix);
  glUniformMatrix4fv(modelData.k2dTransformationUniformLocation,
		     1, true, transformationMatrix);
  draw_hopf_bifurcations();
  draw_saddle_node_bifurcations();
  draw_limit_cycle_bifurcations();
  draw_saddle_connection_bifurcations();
}

void model::parameter_window::draw_hopf_bifurcations() {

  for(std::unordered_map<hopf_bifurcation_id, render_data>::const_iterator iter
	= hopfBifurcationData.begin(); iter != hopfBifurcationData.end();
      ++iter) {
    color glColor(modelData.hopfBifurcations.at(iter->first).specs.glColor);
    if(iter->first == modelData.selectedHopfBifurcationId) {
      glColor.invert();
    }
    glUniform4f(modelData.k2dColorUniformLocation, glColor.get_red(),
		glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
#ifdef GL_VERSION_4_3
    glBindVertexBuffer(model::k2dVertexBinding,
		       iter->second.vbo.get_handle(),
		       0, 2 * sizeof(float));
#else
    glBindBuffer(GL_ARRAY_BUFFER, iter->second.vbo.get_handle());
    glVertexAttribPointer(k2dPositionAttribute, 2, GL_FLOAT,
                          GL_FALSE, 0, reinterpret_cast<void*>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
    glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
  }
}

void model::parameter_window::draw_saddle_node_bifurcations() {
  for(std::unordered_map<saddle_node_bifurcation_id, render_data>::const_iterator iter
	= saddleNodeBifurcationData.begin(); iter != saddleNodeBifurcationData.end();
      ++iter) {
    color glColor(modelData.saddleNodeBifurcations.at(iter->first).specs.glColor);
    if(iter->first == modelData.selectedSaddleNodeBifurcationId) {
      glColor.invert();
    }
    glUniform4f(modelData.k2dColorUniformLocation, glColor.get_red(),
		glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
#ifdef GL_VERSION_4_3
    glBindVertexBuffer(model::k2dVertexBinding,
		       iter->second.vbo.get_handle(),
		       0, 2 * sizeof(float));
#else
    glBindBuffer(GL_ARRAY_BUFFER, iter->second.vbo.get_handle());
    glVertexAttribPointer(k2dPositionAttribute, 2, GL_FLOAT,
                          GL_FALSE, 0, reinterpret_cast<void*>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
    glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
  }
}

void model::parameter_window::draw_limit_cycle_bifurcations() {
  for(std::unordered_map<limit_cycle_bifurcation_id, render_data>::const_iterator iter
	= limitCycleBifurcationData.begin(); iter != limitCycleBifurcationData.end();
      ++iter) {
    color glColor(modelData.limitCycleBifurcations.at(iter->first).specs.glColor);
    if(iter->first == modelData.selectedLimitCycleBifurcationId) {
      glColor.invert();
    }
    glUniform4f(modelData.k2dColorUniformLocation, glColor.get_red(),
		glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
#ifdef GL_VERSION_4_3
    glBindVertexBuffer(model::k2dVertexBinding,
		       iter->second.vbo.get_handle(),
		       0, 2 * sizeof(float));
#else
    glBindBuffer(GL_ARRAY_BUFFER, iter->second.vbo.get_handle());
    glVertexAttribPointer(k2dPositionAttribute, 2, GL_FLOAT,
                          GL_FALSE, 0, reinterpret_cast<void*>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
    glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
  }
}

void model::parameter_window::draw_saddle_connection_bifurcations() {
  for(std::unordered_map<saddle_connection_bifurcation_id, render_data>::const_iterator iter
	= saddleConnectionBifurcationData.begin(); iter != saddleConnectionBifurcationData.end();
      ++iter) {
    color glColor(modelData.saddleConnectionBifurcations.at(iter->first).specs.glColor);
    if(iter->first == modelData.selectedSaddleConnectionBifurcationId) {
      glColor.invert();
    }
    glUniform4f(modelData.k2dColorUniformLocation, glColor.get_red(),
		glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
#ifdef GL_VERSION_4_3
    glBindVertexBuffer(model::k2dVertexBinding,
		       iter->second.vbo.get_handle(),
		       0, 2 * sizeof(float));
#else
    glBindBuffer(GL_ARRAY_BUFFER, iter->second.vbo.get_handle());
    glVertexAttribPointer(k2dPositionAttribute, 2, GL_FLOAT,
                          GL_FALSE, 0, reinterpret_cast<void*>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
    glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
  }
}

void model::parameter_window::resize(int width, int height) {
  double changeWidth = (width - specs.viewport.get_size().x())
    * specs.viewport.get_span().x() / specs.viewport.get_size().x();
  double changeHeight = (height - specs.viewport.get_size().y())
    * specs.viewport.get_span().y() / specs.viewport.get_size().y();
  specs.viewport.set_size(::math::vector2d(width, height));
  specs.viewport.set_span(::math::vector2d(changeWidth +
					 specs.viewport.get_span().x(),
					 changeHeight +
					 specs.viewport.get_span().y()));
}

bool model::parameter_window::on_vert_axis(const ::math::vector2d& pos) const {
  ::math::vector2d axesPixel(specs.viewport.pixel_of(::math::vector2d(0, 0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > specs.viewport.get_size().x())
    axesPixel.x() = specs.viewport.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > specs.viewport.get_size().y())
    axesPixel.y() = specs.viewport.get_size().y();
  double axis = axesPixel.x();
  return axis - 5 < pos.x() && pos.x() < axis + 5;
}

bool model::parameter_window::on_horiz_axis(const ::math::vector2d& pos) const {
  ::math::vector2d axesPixel(specs.viewport.pixel_of(::math::vector2d(0, 0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > specs.viewport.get_size().x())
    axesPixel.x() = specs.viewport.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > specs.viewport.get_size().y())
    axesPixel.y() = specs.viewport.get_size().y();
  double axis = axesPixel.y();
  return axis - 5 < pos.y() && pos.y() < axis + 5;
}

const parameter_specs& model::parameter_window::get_specs() const {
  return specs;
}

model::dynamical_window::dynamical_window(model& model,
					  const dynamical_specs& spec)
  : modelData(model), specs(spec),
    axesVbo(nullptr, 0, GL_DYNAMIC_DRAW),
    ticksPerLabel(5),
    tickFontSize(12),
    realTickDistanceX(0.5),
    realTickDistanceY(0.5) {
  // Generate VBO's for each solution and update the axes.
  for(solution_const_iter iter = modelData.solutions.begin();
      iter != modelData.solutions.end(); ++iter) {
    gen_solution_vbo(iter->first);
  }
  // Generate a vbo for each isocline
  for(isocline_const_iter iter = modelData.isoclines.begin();
      iter != modelData.isoclines.end(); ++iter) {
    gen_isocline_vbo(iter->first);
  }
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

void model::dynamical_window::set_viewport_2d(const ::math::window2d& val) {
  specs.viewport2d = val;
}

void model::dynamical_window::set_viewport_3d(const ::math::window3d& val) {
  specs.viewport3d = val;
}

bool model::dynamical_window::select_separatrix(int x, int y, separatrix_id* id) {
  return false;
}

bool model::dynamical_window::select_solution(int x, int y, solution_id* id) {
  typedef std::list<::math::dynamical_point>::const_iterator point_const_iter;
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
	double xReal;
	double yReal;
	double xPrevReal;
	double yPrevReal;
	if(specs.horzAxisVar == modelData.varDiffName) {
	  xReal = point->t;
	  xPrevReal = prevPoint->t;
	} else {
	  xReal = point->vars[modelData.dynamicalVarIndex.at(specs.horzAxisVar)];
	  xPrevReal = prevPoint->vars[modelData.dynamicalVarIndex.at(specs.horzAxisVar)];
	}
	if(specs.vertAxisVar == modelData.varDiffName) {
	  yReal = point->t;
	  yPrevReal = prevPoint->t;
	} else {
	  yReal = point->vars[modelData.dynamicalVarIndex.at(specs.vertAxisVar)];
	  yPrevReal = prevPoint->vars[modelData.dynamicalVarIndex.at(specs.vertAxisVar)];
	}
	::math::vector2d real(xReal, yReal);
	::math::vector2d pixel(specs.viewport2d.pixel_of(real));
	::math::vector2d realPrev(xPrevReal, yPrevReal);
	::math::vector2d pixelPrev(specs.viewport2d.pixel_of(realPrev));
	const int tolerance(5);
	::math::vector2d cursor(x, y);
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
  typedef std::list<::math::vector>::const_iterator point_const_iter;
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
	double xReal;
	double yReal;
	double xPrevReal;
	double yPrevReal;
	
	xReal = (*point)[modelData.dynamicalVarIndex.at(specs.horzAxisVar)];
	xPrevReal = (*prevPoint)[modelData.dynamicalVarIndex.at(specs.horzAxisVar)];
	yReal = (*point)[modelData.dynamicalVarIndex.at(specs.vertAxisVar)];
	yPrevReal = (*prevPoint)[modelData.dynamicalVarIndex.at(specs.vertAxisVar)];
       
	::math::vector2d real(xReal, yReal);
	::math::vector2d pixel(specs.viewport2d.pixel_of(real));
	::math::vector2d realPrev(xPrevReal, yPrevReal);
	::math::vector2d pixelPrev(specs.viewport2d.pixel_of(realPrev));
	const int tolerance(5);
	::math::vector2d cursor(x, y);
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
      double xReal;
      double yReal;
      xReal = iter->second.position[modelData.dynamicalVarIndex.at(specs.horzAxisVar)];
      yReal = iter->second.position[modelData.dynamicalVarIndex.at(specs.vertAxisVar)];
      ::math::vector2d real(xReal, yReal);
      ::math::vector2d pixel(specs.viewport2d.pixel_of(real));
      const double tolerance = 5;
      if(pixel.distance(::math::vector2d(x, y)) <= tolerance) {
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
#ifdef GL_VERSION_4_3
    glBindVertexBuffer(model::kPath3dVertexBinding,
		       iter->second.vbo3d.get_handle(),
                       0, 3 * sizeof(float));
#else
    glBindBuffer(GL_ARRAY_BUFFER, iter->second.vbo3d.get_handle());
    glVertexAttribPointer(kPath3dPositionAttribute, 3, GL_FLOAT,
                          GL_FALSE, 0, reinterpret_cast<void*>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
    glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
  }
}
void model::dynamical_window::draw_isoclines_3d() {
  // We don't draw isoclines if we are viewing the time component.
  if(specs.xAxisVar != modelData.varDiffName &&
     specs.yAxisVar != modelData.varDiffName &&
     specs.zAxisVar != modelData.varDiffName) {
    for(isocline_data_const_iter iter = isoclineData.begin();
	iter != isoclineData.end(); ++iter) {
      color glColor(modelData.isoclines.at(iter->first).specs.glColor);
      if(iter->first == modelData.selectedIsoclineId) {
	glColor.invert();
      }
      glUniform4f(modelData.kPath3dColorUniformLocation, glColor.get_red(),
		  glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
#ifdef GL_VERSION_4_3
      glBindVertexBuffer(model::kPath3dVertexBinding,
			 iter->second.vbo3d.get_handle(),
                         0, 3 * sizeof(float));
#else
      glBindBuffer(GL_ARRAY_BUFFER, iter->second.vbo3d.get_handle());
      glVertexAttribPointer(kPath3dPositionAttribute, 3, GL_FLOAT,
                            GL_FALSE, 0, reinterpret_cast<void*>(0));
      glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
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
      
      ::math::matrix_4x4 mat = ::math::matrix_4x4::translation(pixelCoordinate.x(),
							   pixelCoordinate.y());
      mat *= ::math::matrix_4x4::scale(6,6);
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
#ifdef GL_VERSION_4_3
  glBindVertexBuffer(model::kPath3dVertexBinding,
		     tmp.get_handle(), 0, 3 * sizeof(float));
#else
  glBindBuffer(GL_ARRAY_BUFFER, tmp.get_handle());
  glVertexAttribPointer(kPath3dPositionAttribute, 3, GL_FLOAT,
                        GL_FALSE, 0, reinterpret_cast<void*>(0));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

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
#ifdef GL_VERSION_4_3
    glBindVertexBuffer(model::k2dVertexBinding,
		       iter->second.vbo2d.get_handle(),
                       0, 2 * sizeof(float));
#else
    glBindBuffer(GL_ARRAY_BUFFER, iter->second.vbo2d.get_handle());
    glVertexAttribPointer(k2dPositionAttribute, 2, GL_FLOAT,
                          GL_FALSE, 0, reinterpret_cast<void*>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
    glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
  }
}
void model::dynamical_window::draw_isoclines_2d() {
  if(specs.horzAxisVar != modelData.varDiffName &&
     specs.vertAxisVar != modelData.varDiffName) {
    for(isocline_data_const_iter iter = isoclineData.begin();
	iter != isoclineData.end(); ++iter) {
      color glColor(modelData.isoclines.at(iter->first).specs.glColor);
      if(iter->first == modelData.selectedIsoclineId) {
	glColor.invert();
      }
      glUniform4f(modelData.k2dColorUniformLocation, glColor.get_red(),
		  glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
#ifdef GL_VERSION_4_3
      glBindVertexBuffer(model::k2dVertexBinding,
			 iter->second.vbo2d.get_handle(),
                         0, 2 * sizeof(float));
#else
      glBindBuffer(GL_ARRAY_BUFFER, iter->second.vbo2d.get_handle());
      glVertexAttribPointer(k2dPositionAttribute, 2, GL_FLOAT,
                            GL_FALSE, 0, reinterpret_cast<void*>(0));
      glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
      glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
    }
  }
}
void model::dynamical_window::draw_separatrices_2d() {
  if(specs.horzAxisVar != modelData.varDiffName &&
     specs.vertAxisVar != modelData.varDiffName) {
    for(separatrix_data_const_iter iter = separatrixData.begin();
	iter != separatrixData.end(); ++iter) {
      color glColor(modelData.separatrices.at(iter->first).specs.glColor);
      if(iter->first == modelData.selectedSeparatrixId) {
	glColor.invert();
      }
      glUniform4f(modelData.k2dColorUniformLocation, glColor.get_red(),
		  glColor.get_green(), glColor.get_blue(), glColor.get_alpha());
#ifdef GL_VERSION_4_3
      glBindVertexBuffer(model::k2dVertexBinding,
			 iter->second.vbo2d.get_handle(),
                         0, 2 * sizeof(float));
#else
      glBindBuffer(GL_ARRAY_BUFFER, iter->second.vbo2d.get_handle());
      glVertexAttribPointer(k2dPositionAttribute, 2, GL_FLOAT,
                            GL_FALSE, 0, reinterpret_cast<void*>(0));
      glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
      glDrawArrays(GL_LINE_STRIP, 0, iter->second.vertices);
    }
  }
}
void model::dynamical_window::draw_singular_points_2d() {
  // We don't draw it if we are viewing the t axis.
  if(specs.horzAxisVar != modelData.varDiffName &&
     specs.vertAxisVar != modelData.varDiffName) {
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
      double xReal;
      double yReal;
      xReal = iter->second.position[modelData.dynamicalVarIndex.at(specs.horzAxisVar)];
      yReal = iter->second.position[modelData.dynamicalVarIndex.at(specs.vertAxisVar)];
      ::math::vector2d realCoordinate(xReal, yReal);
      GLfloat transformationMatrix[16];
      ::math::vector2d pixelCoordinate
	(specs.viewport2d.pixel_of(realCoordinate));
      (specs.viewport2d.pixel_to_ndc() *
       ::math::matrix_4x4::translation(pixelCoordinate.x(), pixelCoordinate.y()) *
       ::math::matrix_4x4::scale(6, 6)).as_float_array(transformationMatrix);
      glUniformMatrix4fv(modelData.k2dTransformationUniformLocation,
			 1, true, transformationMatrix);
      GLuint handle;
      GLsizei vertices;
      switch(iter->second.type) {
      case singular_point::type::STABLE_NODE:
      case singular_point::type::UNSTABLE_NODE:
	handle = modelData.squareVbo.get_handle();
	vertices = modelData.squareVboVertices;
	break;
      case singular_point::type::SADDLE:
	handle = modelData.triangleVbo.get_handle();
	vertices = modelData.triangleVboVertices;
	break;
      case singular_point::type::STABLE_FOCUS:
      case singular_point::type::UNSTABLE_FOCUS:
	handle = modelData.circleVbo.get_handle();
	vertices = modelData.circleVboVertices;
	break;
      case singular_point::type::SADDLE_FOCUS:
      case singular_point::type::STABLE_FOCUS_NODE:
      case singular_point::type::UNSTABLE_FOCUS_NODE:
      case singular_point::type::SADDLE_FOCUS_NODE:
      default:
	handle = modelData.starVbo.get_handle();
	vertices = modelData.starVboVertices;
      }
#ifdef GL_VERSION_4_3
      glBindVertexBuffer(model::k2dVertexBinding,
			 handle, 0, 2 * sizeof(float));
#else
      glBindBuffer(GL_ARRAY_BUFFER, handle);
      glVertexAttribPointer(k2dPositionAttribute, 2, GL_FLOAT,
                            GL_FALSE, 0, reinterpret_cast<void*>(0));
      glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
      glDrawArrays(GL_LINE_LOOP, 0, vertices);
    }
  }
}

void model::draw_axes_2d(const ::math::window2d& viewport, int ticksPerLabel,
			 double tickFontSize,
			 double& realTickDistanceX, double& realTickDistanceY,
			 gl::buffer& axesVbo) const {
  GLfloat transformationMatrix[16];
  // Render axes text.
    // Render the axes
  // Compute the distance between each tick.
  int numberOfTicksX =
    viewport.get_span().x()/realTickDistanceX;
  int pixelTickDistanceX = viewport.get_size().x()
    /static_cast<double>(numberOfTicksX);
  if(pixelTickDistanceX < model::minPixelTickDist) {
    realTickDistanceX *= ticksPerLabel;
  } else if(pixelTickDistanceX > model::maxPixelTickDist) {
    realTickDistanceX /= ticksPerLabel;
  }
  int numberOfTicksY =
    viewport.get_span().y()/realTickDistanceY;
  int pixelTickDistanceY =
    viewport.get_size().y()/static_cast<double>(numberOfTicksY);
  if(pixelTickDistanceY < model::minPixelTickDist) {
    realTickDistanceY *= ticksPerLabel;
  } else if(pixelTickDistanceY > model::maxPixelTickDist) {
    realTickDistanceY /= ticksPerLabel;
  }

  // The x and y values in pixels for where the x and y axes should cross.
  ::math::vector2d axesPixel
    (viewport.pixel_of(::math::vector2d(0.0, 0.0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > viewport.get_size().x())
    axesPixel.x() = viewport.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > viewport.get_size().y())
    axesPixel.y() = viewport.get_size().y();
  
  // Set up tick data in pixel coordinates.
  std::vector<float> axesVboData;
  double xStart = realTickDistanceX *
    static_cast<int>(viewport.get_position().x()
		     / realTickDistanceX);
  double yStart = realTickDistanceY *
    static_cast<int>(viewport.get_position().y()
		     / realTickDistanceY);
  for(double x = xStart;
      x < viewport.get_position().x()
	+ viewport.get_span().x(); x += realTickDistanceX) {
    float realCoordinate =
      viewport.pixel_of(::math::vector2d(x, 0)).x();
    // First vertex
    axesVboData.push_back(realCoordinate); // X
    axesVboData.push_back(axesPixel.y() - 5); // Y

    // Second vertex
    axesVboData.push_back(realCoordinate); // X
    axesVboData.push_back(axesPixel.y() + 5); // Y
  }
  for(double y = yStart; y < viewport.get_position().y()
	+ viewport.get_span().y();
      y += realTickDistanceY) {
    float realCoordinate =
      viewport.pixel_of(::math::vector2d(0, y)).y();
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
  
  axesVboData.push_back(viewport.get_size().x());
  axesVboData.push_back(axesPixel.y());

  axesVboData.push_back(axesPixel.x());
  axesVboData.push_back(0);
  
  axesVboData.push_back(axesPixel.x());
  axesVboData.push_back(viewport.get_size().y());
  
  axesVbo.set_data(reinterpret_cast<unsigned char*>(axesVboData.data()),
		   axesVboData.size()*sizeof(float));
  GLsizei axesVboVertices = axesVboData.size() / 2;

  glUseProgram(program2d.get_handle());
  glBindVertexArray(vao2d.get_handle());
  // Generate pixel to ndc transform
  viewport.pixel_to_ndc()
    .as_float_array(transformationMatrix);
  glUniformMatrix4fv(k2dTransformationUniformLocation,
		     1, true, transformationMatrix);
  // Set color to Black
  glUniform4f(k2dColorUniformLocation, 0.0f, 0.0f, 0.0f, 1.0f);
#ifdef GL_VERSION_4_3
  glBindVertexBuffer(model::k2dVertexBinding, axesVbo.get_handle(),
		     0, 2 * sizeof(float));
#else
  glBindBuffer(GL_ARRAY_BUFFER, axesVbo.get_handle());
  glVertexAttribPointer(k2dPositionAttribute, 2, GL_FLOAT,
                        GL_FALSE, 0, reinterpret_cast<void*>(0));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
  glDrawArrays(GL_LINES, 0, axesVboVertices);

  // Render Axes text
  const double tolerance = 0.001;
  axesPixel = viewport.pixel_of(::math::vector2d(0.0, 0.0));
  int yTextOffset(10);
  int xTextOffset(10);
  if(axesPixel.x() < 0.0) {
    axesPixel.x() = 0.0;
  }
  if(axesPixel.x() > viewport.get_size().x()) {
    axesPixel.x() = viewport.get_size().x();
    xTextOffset = -40;
  }
  if(axesPixel.y() < 0.0) {
    axesPixel.y() = 0.0;
  }
  if(axesPixel.y() > viewport.get_size().y()) {
    axesPixel.y() = viewport.get_size().y();
    yTextOffset = -20;
  }
  ::math::vector2d axesReal(viewport
			  .real_coordinate_of(axesPixel));
  xStart = realTickDistanceX * ticksPerLabel *
    static_cast<int>(viewport.get_position().x() /
		     (realTickDistanceX * ticksPerLabel));
  yStart = realTickDistanceY * ticksPerLabel *
    static_cast<int>(viewport.get_position().y() /
		     (realTickDistanceY * ticksPerLabel));
  for(double x = xStart; x > viewport.get_position().x();
      x -= ticksPerLabel * realTickDistanceX) {
    if(std::abs(x) < tolerance) continue;
    ::math::vector2d pixel(viewport
			 .pixel_of(::math::vector2d(x,
						  axesReal.y())));
    std::string text = util::double_to_string(x, 2);
    textRenderer.render_text(text, pixel.x() - text.size() * 3,
				       pixel.y() + yTextOffset,
				       font, tickFontSize);
  }
  for(double x = xStart; x < viewport.get_position().x()
	+ viewport.get_span().x();
      x += ticksPerLabel * realTickDistanceX) {
    if(std::abs(x) < tolerance) continue;
    ::math::vector2d pixel(viewport
			 .pixel_of(::math::vector2d(x, axesReal.y())));
    std::string text = util::double_to_string(x, 2);
    textRenderer.render_text(text, pixel.x() - text.size() * 3,
			     pixel.y() + yTextOffset,
			     font, tickFontSize);
  }
  for(double y = yStart; y > viewport.get_position().y();
      y -= ticksPerLabel * realTickDistanceY) {
    if(std::abs(y) < tolerance) continue;
    ::math::vector2d pixel(viewport
			 .pixel_of(::math::vector2d(axesReal.x(),y)));
    std::string text = util::double_to_string(y, 2);
    textRenderer.render_text(text,
			     pixel.x() + xTextOffset,
			     pixel.y() - 5,
			     font,
			     tickFontSize);
  }
  for(double y = yStart; y < viewport.get_position().y()
	+ viewport.get_span().y();
      y += ticksPerLabel * realTickDistanceY) {
    if(std::abs(y) < tolerance) continue;
    ::math::vector2d pixel(viewport
			 .pixel_of(::math::vector2d(axesReal.x(),y)));
    std::string text = util::double_to_string(y, 2);
    textRenderer.render_text(text, pixel.x() + xTextOffset,
			     pixel.y() - 5, font,
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
    modelData.draw_axes_2d(specs.viewport2d, ticksPerLabel,
			   tickFontSize,
			   realTickDistanceX, realTickDistanceY,
			   axesVbo);
    glUseProgram(modelData.program2d.get_handle());
    glBindVertexArray(modelData.vao2d.get_handle());
    float transformationMatrix[16];
    specs.viewport2d.real_to_ndc().as_float_array(transformationMatrix);
    glUniformMatrix4fv(modelData.k2dTransformationUniformLocation,
		       1, true, transformationMatrix);

    draw_solutions_2d();
    draw_isoclines_2d();
    draw_separatrices_2d();

    // Note that these calls changes the transformation uniform.
    draw_singular_points_2d();
  }
}
  
void model::dynamical_window::resize(int width, int height) {
  double changeWidth = (width - specs.viewport2d.get_size().x())
    * specs.viewport2d.get_span().x() /
    specs.viewport2d.get_size().x();
  double changeHeight = (height - specs.viewport2d.get_size().y())
    * specs.viewport2d.get_span().y() /
    specs.viewport2d.get_size().y();
  specs.viewport2d.set_size(::math::vector2d(width, height));
  specs.viewport2d
    .set_span(::math::vector2d(changeWidth +
			     specs.viewport2d.get_span().x(),
			     changeHeight +
			     specs.viewport2d.get_span().y()));
  specs.viewport3d.set_width(width);
  specs.viewport3d.set_height(height);
}

::math::dynamical_point model::dynamical_window::get_point(const ::math::vector2d& pos) const {
  ::math::vector2d real = specs.viewport2d.real_coordinate_of(pos);
  ::math::dynamical_point point;
  point.vars = ::math::vector(modelData.dynamicalVars, 0.0);
  point.t = 0;
  if(specs.horzAxisVar == modelData.varDiffName) {
    point.t = real.x();
  } else {
    point.vars[modelData.dynamicalVarIndex.at(specs.horzAxisVar)] = real.x();
  }
  if(specs.vertAxisVar == modelData.varDiffName) {
    point.t = real.y();
  } else {
    point.vars[modelData.dynamicalVarIndex.at(specs.vertAxisVar)] = real.y();
  }
  return point;
}

bool model::dynamical_window::on_vert_axis(const ::math::vector2d& pos) const {
  ::math::vector2d axesPixel(specs.viewport2d
			   .pixel_of(::math::vector2d(0.0, 0.0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > specs.viewport2d.get_size().x())
    axesPixel.x() = specs.viewport2d.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > specs.viewport2d.get_size().y())
    axesPixel.y() = specs.viewport2d.get_size().y();
  double axis = axesPixel.x();
  return axis - 5 < pos.x() && pos.x() < axis + 5;
}
bool model::dynamical_window::on_horiz_axis(const ::math::vector2d& pos) const {
  ::math::vector2d axesPixel(specs.viewport2d
			   .pixel_of(::math::vector2d(0.0, 0.0)));
  if(axesPixel.x() < 0.0)
    axesPixel.x() = 0.0;
  if(axesPixel.x() > specs.viewport2d.get_size().x())
    axesPixel.x() = specs.viewport2d.get_size().x();
  if(axesPixel.y() < 0.0)
    axesPixel.y() = 0.0;
  if(axesPixel.y() > specs.viewport2d.get_size().y())
    axesPixel.y() = specs.viewport2d.get_size().y();
  double axis = axesPixel.y();
  return axis - 5 < pos.y() && pos.y() < axis + 5;
}

void model::dynamical_window::delete_solution(solution_id id) {
  solutionData.erase(id);
}

void model::dynamical_window::delete_separatrix(separatrix_id id) {
  separatrixData.erase(id);
}

void model::dynamical_window::delete_isocline(isocline_id id) {
  isoclineData.erase(id);
}

void model::dynamical_window::gen_solution_vbo(solution_id id) {
  const std::list<::math::dynamical_point>& solution(modelData.solutions.at(id).data);
  std::vector<float> data2d;
  std::vector<float> data3d;
  size_t points = solution.size();
  size_t size2d = points * 2 * sizeof(float);
  size_t size3d = points * 3 * sizeof(float);
  for(std::list<::math::dynamical_point>::const_iterator point = solution.begin();
      point != solution.end(); ++point) {
    double xReal;
    double yReal;
    if(specs.horzAxisVar == modelData.varDiffName) {
      xReal = point->t;
    } else {
      xReal = point->vars[modelData.dynamicalVarIndex.at(specs.horzAxisVar)];
    }
    if(specs.vertAxisVar == modelData.varDiffName) {
      yReal = point->t;
    } else {
      yReal = point->vars[modelData.dynamicalVarIndex.at(specs.vertAxisVar)];
    }
    data2d.push_back(xReal);
    data2d.push_back(yReal);

    double zReal;
    if(specs.xAxisVar == modelData.varDiffName) {
      xReal = point->t;
    } else {
      xReal = point->vars[modelData.dynamicalVarIndex.at(specs.xAxisVar)];
    }
    if(specs.yAxisVar == modelData.varDiffName) {
      yReal = point->t;
    } else {
      yReal = point->vars[modelData.dynamicalVarIndex.at(specs.yAxisVar)];
    }
    if(specs.zAxisVar == modelData.varDiffName) {
      zReal = point->t;
    } else {
      zReal = point->vars[modelData.dynamicalVarIndex.at(specs.zAxisVar)];
    }

    data3d.push_back(xReal);
    data3d.push_back(yReal);
    data3d.push_back(zReal);
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
    render_data val{vbo2d, vbo3d, points};
    solutionData.insert(std::make_pair(id, val));
  }
}

void model::dynamical_window::gen_separatrix_vbo(separatrix_id id) {
  const std::list<::math::vector2d>& separatrix(modelData.separatrices.at(id).data);
  std::vector<float> data2d;
  std::vector<float> data3d;
  size_t points = separatrix.size();
  size_t size2d = 0;
  size_t size3d = 0;
  if(specs.horzAxisVar != modelData.varDiffName &&
     specs.vertAxisVar != modelData.varDiffName) {
    size2d = points * 2 * sizeof(float);
    for(std::list<::math::vector2d>::const_iterator point = separatrix.begin();
	point != separatrix.end(); ++point) {
      double xReal;
      double yReal;
      xReal = (*point)[modelData.dynamicalVarIndex.at(specs.horzAxisVar)];
      yReal = (*point)[modelData.dynamicalVarIndex.at(specs.vertAxisVar)];
      data2d.push_back(xReal);
      data2d.push_back(yReal); 
    }
  }
  // If the render Data already exists.
  if(separatrixData.find(id) != separatrixData.end()) {
    separatrixData.at(id).vertices = points;
    separatrixData.at(id)
      .vbo2d.set_data(reinterpret_cast<unsigned char *>(data2d.data()), size2d);
  } else {
    gl::buffer vbo2d(reinterpret_cast<unsigned char*>(data2d.data()),
		     size2d, GL_DYNAMIC_DRAW);
    gl::buffer vbo3d(nullptr, size3d, GL_DYNAMIC_DRAW);
    render_data val{vbo2d, vbo3d, points};
    separatrixData.insert(std::make_pair(id, val));
  }
}

void model::dynamical_window::gen_isocline_vbo(isocline_id id) {
  const std::list<::math::vector>& isocline(modelData.isoclines.at(id).data);
  std::vector<float> data2d;
  std::vector<float> data3d;
  size_t points = isocline.size();
  size_t size2d = 0;
  size_t size3d = 0;
  if(specs.horzAxisVar != modelData.varDiffName &&
     specs.vertAxisVar != modelData.varDiffName) {
    size2d = points * 2 * sizeof(float);
    for(std::list<::math::vector>::const_iterator point = isocline.begin();
	point != isocline.end(); ++point) {
      double xReal;
      double yReal;
      xReal = (*point)[modelData.dynamicalVarIndex.at(specs.horzAxisVar)];
      yReal = (*point)[modelData.dynamicalVarIndex.at(specs.vertAxisVar)];
      data2d.push_back(xReal);
      data2d.push_back(yReal); 
    }
  }
  if(specs.xAxisVar != modelData.varDiffName &&
     specs.yAxisVar != modelData.varDiffName &&
     specs.zAxisVar != modelData.varDiffName) {
    size3d = points * 3 * sizeof(float);
    for(std::list<::math::vector>::const_iterator point = isocline.begin();
	point != isocline.end(); ++point) {
      double xReal;
      double yReal;
      double zReal;
      xReal = (*point)[modelData.dynamicalVarIndex.at(specs.xAxisVar)];
      yReal = (*point)[modelData.dynamicalVarIndex.at(specs.yAxisVar)];
      zReal = (*point)[modelData.dynamicalVarIndex.at(specs.zAxisVar)];

      data3d.push_back(xReal);
      data3d.push_back(yReal);
      data3d.push_back(zReal);
    }
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
    render_data val{vbo2d, vbo3d, points};
    isoclineData.insert(std::make_pair(id, val));
  }
}

const std::string model::k2dVertexShaderFilePath(VERTEX_SHADER_PATH_2D);
const GLuint model::k2dPositionAttribute(0);
const GLuint model::k2dVertexBinding(0);
const std::string model::k2dTransformationUniform("transformation");

const std::string model::k2dFragmentShaderFilePath(FRAGMENT_SHADER_PATH_2D);
const std::string model::k2dColorUniform("inColor");

const std::string model::kPath3dVertexShaderFilePath(VERTEX_SHADER_PATH_3D);
const GLuint model::kPath3dPositionAttribute(0);
const GLuint model::kPath3dVertexBinding(0);
const std::string model::kPath3dTransformationUniform("transformation");

const std::string model::kPath3dFragmentShaderFilePath(FRAGMENT_SHADER_PATH_3D);
const std::string model::kPath3dColorUniform("inColor");

const int model::minPixelTickDist = 15;
const int model::maxPixelTickDist = 15 * 5 * 2;

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
const separatrix_id model::kNoSeparatrixId = 0;
const hopf_bifurcation_id model::kNoHopfBifurcationId = 0;
const saddle_node_bifurcation_id model::kNoSaddleNodeBifurcationId = 0;
const limit_cycle_bifurcation_id model::kNoLimitCycleBifurcationId = 0;
const saddle_connection_bifurcation_id model::kNoSaddleConnectionBifurcationId = 0;

model::model() : font(DEFAULT_FONT_PATH),
		 dynamicalVars(0),
		 parameters(0),
		 selectedSolutionId(kNoSolutionId),
		 selectedSingularPointId(kNoSingularPointId),
		 selectedIsoclineId(kNoIsoclineId),
		 selectedSeparatrixId(kNoSeparatrixId),
		 selectedHopfBifurcationId(kNoHopfBifurcationId),
		 selectedSaddleNodeBifurcationId(kNoSaddleNodeBifurcationId),
		 selectedLimitCycleBifurcationId(kNoLimitCycleBifurcationId),
		 selectedSaddleConnectionBifurcationId(kNoSaddleConnectionBifurcationId),
		 uniqueDynamicalId(kNoDynamicalId),
		 uniqueParameterId(kNoParameterId),
		 uniqueSolutionId(kNoSolutionId),
		 uniqueSingularPointId(kNoSingularPointId),
		 uniqueIsoclineId(kNoIsoclineId),
		 uniqueSeparatrixId(kNoSeparatrixId),
		 uniqueHopfBifurcationId(kNoHopfBifurcationId),
		 uniqueSaddleNodeBifurcationId(kNoSaddleNodeBifurcationId),
		 uniqueLimitCycleBifurcationId(kNoLimitCycleBifurcationId),
		 uniqueSaddleConnectionBifurcationId(kNoSaddleConnectionBifurcationId),
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
		 circleVbo(nullptr, 0, GL_STATIC_DRAW),
		 triangleVbo(nullptr, 0, GL_STATIC_DRAW),
		 squareVbo(nullptr, 0, GL_STATIC_DRAW),
		 starVbo(nullptr, 0, GL_STATIC_DRAW) {
  glUseProgram(program2d.get_handle());
  glBindVertexArray(vao2d.get_handle());
  glEnableVertexAttribArray(k2dPositionAttribute);
#ifdef GL_VERSION_4_3
  glVertexAttribFormat(k2dPositionAttribute,
		       2, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(k2dPositionAttribute,
			k2dVertexBinding);
#endif
  glUseProgram(programPath3d.get_handle());
  glBindVertexArray(vaoPath3d.get_handle());
  glEnableVertexAttribArray(kPath3dPositionAttribute);
#ifdef GL_VERSION_4_3
  glVertexAttribFormat(kPath3dPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(kPath3dPositionAttribute, kPath3dVertexBinding);
#endif
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

  // Initialize triangle vbo

  GLfloat triangleData[6] = {
    -1, -1, 1, -1, 0, 1
  };
  triangleVboVertices = 3;
  triangleVbo.set_data(reinterpret_cast<unsigned char*>(triangleData),
		       triangleVboVertices * 2 * sizeof(GLfloat));

  GLfloat squareData[8] = {
    -1, -1, 1, -1, 1, 1, -1, 1
  };
  squareVboVertices = 4;
  squareVbo.set_data(reinterpret_cast<unsigned char*>(squareData),
		     squareVboVertices * 2 * sizeof(GLfloat));

  GLfloat starData[16] = {
    -1, -1, 0, -0.5, 1, -1, 0.5, 0, 1, 1, 0, 0.5, -1, 1, -0.5, 0
  };
  starVboVertices = 8;
  starVbo.set_data(reinterpret_cast<unsigned char*>(starData),
		   starVboVertices * 2 * sizeof(GLfloat));
}

void model::set_no_compile() {
  compiled = false;
  dynamicalWindows.clear();
  solutions.clear();
  singularPoints.clear();
  system.clear();
  partials.clear();
  doublePartials.clear();
  expressions.clear();
}


const std::unordered_map<singular_point_id, singular_point>&
model::get_singular_points() const {
  return singularPoints;
}

void model::add_separatrix(const separatrix_specs& specs) {
  ++uniqueSeparatrixId;
  separatrices.insert(std::make_pair(uniqueSeparatrixId,
				     separatrix{specs, std::list<::math::vector2d>()}));
  generate_separatrix_data(uniqueSeparatrixId);
  // We now need to update the VBO's of each window.
  for(dynamical_iter iter = dynamicalWindows.begin();
      iter != dynamicalWindows.end(); ++iter) {
    iter->second.gen_separatrix_vbo(uniqueSeparatrixId);
  }
}

void model::add_solution(const solution_specs& spec) {
  ++uniqueSolutionId;
  solutions.insert(std::make_pair(uniqueSolutionId,
				  solution{spec, std::list<::math::dynamical_point>()}));
  generate_solution_data(uniqueSolutionId);
  
  // We now need to update the VBO's of each window.
  for(dynamical_iter iter = dynamicalWindows.begin();
      iter != dynamicalWindows.end(); ++iter) {
    iter->second.gen_solution_vbo(uniqueSolutionId);
  }
}

bool model::add_isocline(const isocline_specs& spec) {
  ++uniqueIsoclineId;
  isoclines.insert(std::make_pair(uniqueIsoclineId, isocline{spec,
	  std::list<::math::vector>()}));
  bool success = generate_isocline_data(uniqueIsoclineId);
  if(success) {
    // We now need to update the VBO's of each window.
    for(dynamical_iter iter = dynamicalWindows.begin();
	iter != dynamicalWindows.end(); ++iter) {
      iter->second.gen_isocline_vbo(uniqueIsoclineId);
    }
    return true;
  } else {
    // We now must remove the dummy isocline.
    isoclines.erase(uniqueIsoclineId);
    --uniqueIsoclineId;
    return false;
  }
}

bool model::add_singular_point(const singular_point_specs& specs) {
  ++uniqueSingularPointId;
  singularPoints.insert(std::make_pair(uniqueSingularPointId,
				       singular_point(specs, ::math::vector())));
  bool success = generate_singular_point_data(uniqueSingularPointId);
  if(success) {
    return true;
  } else {
    // We now must remove the dummy singular point.
    singularPoints.erase(uniqueSingularPointId);
    --uniqueSingularPointId;
    return false;
  }
}

bool model::add_hopf_bifurcation(const hopf_bifurcation_specs& specs) {
  ++uniqueHopfBifurcationId;
  hopfBifurcations.insert(std::make_pair(uniqueHopfBifurcationId,
					 hopf_bifurcation{specs,
					     std::list<::math::vector>()}));
  bool success = generate_hopf_bifurcation_data(uniqueHopfBifurcationId);
  if(success) {
    for(parameter_iter iter = parameterWindows.begin();
	iter != parameterWindows.end(); ++iter) {
      iter->second.gen_hopf_bifurcation_vbo(uniqueHopfBifurcationId);
    }
    return true;
  } else {
    hopfBifurcations.erase(uniqueHopfBifurcationId);
    --uniqueHopfBifurcationId;
    return false;
  }
}

bool model::add_saddle_node_bifurcation(const saddle_node_bifurcation_specs& specs) {
  ++uniqueSaddleNodeBifurcationId;
  saddleNodeBifurcations.insert(std::make_pair(uniqueSaddleNodeBifurcationId,
					       saddle_node_bifurcation{specs,
						   std::list<::math::vector>()}));
  bool success = generate_saddle_node_bifurcation_data(uniqueSaddleNodeBifurcationId);
  if(success) {
    for(parameter_iter iter = parameterWindows.begin();
	iter != parameterWindows.end(); ++iter) {
      iter->second.gen_saddle_node_bifurcation_vbo(uniqueSaddleNodeBifurcationId);
    }
    return true;
  } else {
    saddleNodeBifurcations.erase(uniqueSaddleNodeBifurcationId);
    --uniqueSaddleNodeBifurcationId;
    return false;
  }
}

bool model::add_limit_cycle_bifurcation(const limit_cycle_bifurcation_specs& specs) {
  ++uniqueLimitCycleBifurcationId;
  limitCycleBifurcations.insert(std::make_pair(uniqueLimitCycleBifurcationId,
					       limit_cycle_bifurcation{specs,
						   std::list<::math::vector>()}));
  bool success = generate_limit_cycle_bifurcation_data(uniqueLimitCycleBifurcationId);
  if(success) {
    for(parameter_iter iter = parameterWindows.begin();
	iter != parameterWindows.end(); ++iter) {
      iter->second.gen_limit_cycle_bifurcation_vbo(uniqueLimitCycleBifurcationId);
    }
    return true;
  } else {
    limitCycleBifurcations.erase(uniqueLimitCycleBifurcationId);
    --uniqueLimitCycleBifurcationId;
    return false;
  }
}

bool model::add_saddle_connection_bifurcation(const saddle_connection_bifurcation_specs& specs) {
  ++uniqueSaddleConnectionBifurcationId;
  saddleConnectionBifurcations.insert(std::make_pair(uniqueSaddleConnectionBifurcationId,
					       saddle_connection_bifurcation{specs,
						   std::list<::math::vector>()}));
  bool success = generate_saddle_connection_bifurcation_data(uniqueSaddleConnectionBifurcationId);
  if(success) {
    for(parameter_iter iter = parameterWindows.begin();
	iter != parameterWindows.end(); ++iter) {
      iter->second.gen_saddle_connection_bifurcation_vbo(uniqueSaddleConnectionBifurcationId);
    }
    return true;
  } else {
    saddleConnectionBifurcations.erase(uniqueSaddleConnectionBifurcationId);
    --uniqueSaddleConnectionBifurcationId;
    return false;
  }
}

bool model::generate_saddle_connection_bifurcation_data(saddle_connection_bifurcation_id id) {
  saddle_connection_bifurcation_specs specs = saddleConnectionBifurcations.at(id).specs;
  std::vector<std::function<double(const double*)>> systemFunctions;
  ::math::vector p1 =
    singularPoints.at(separatrices.at(specs.separatrix1).specs.singularPoint).position;
  
  ::math::vector p2 =
    singularPoints.at(separatrices.at(specs.separatrix2).specs.singularPoint).position;
  
  systemFunctions.push_back([&](const double* arr)->double {
      find_separatrix_intersection_ret ret1 =
	find_separatrix_intersection(arr[0],
				     arr[1],
				     p1[0],
				     p1[1],
				     specs.transversalA,
				     specs.transversalB,
				     specs.separatrix1);
      
      if(!ret1.success) {
	throw no_intersection_found_exception();
      }
      find_separatrix_intersection_ret ret2 =
	find_separatrix_intersection(arr[0],
				     arr[1],
				     p2[0],
				     p2[1],
				     specs.transversalA,
				     specs.transversalB,
				     specs.separatrix2);
      if(!ret2.success) {
	throw no_intersection_found_exception();
      }
      return ret1.intersection.distance(ret2.intersection);
    });
  std::vector<std::function<double(const double*)>> jacobianFunctions;
  jacobianFunctions.push_back([&](const double* arr)->double {
      ::math::vector vec(2, 0.0);
      vec[0] = arr[0];
      vec[1] = arr[1];
      return ::math::derivative(systemFunctions[0], vec, 0);
    });
  jacobianFunctions.push_back([&](const double* arr)->double {
      ::math::vector vec(2, 0.0);
      vec[0] = arr[0];
      vec[1] = arr[1];
      return ::math::derivative(systemFunctions[0], vec, 1);
    });

  std::vector<int> varIndex;
  varIndex.push_back(0);
  varIndex.push_back(1);

  std::unordered_set<int> constraintVars;
  constraintVars.insert(0);
  constraintVars.insert(1);

  /*  find_separatrix_intersection_ret ret1 =
    find_separatrix_intersection(specs.init[0],
				 specs.init[1],
				 p1[0],
				 p1[1],
				 specs.transversalA,
				 specs.transversalB,
				 specs.separatrix1);
  find_separatrix_intersection_ret ret2 =
    find_separatrix_intersection(specs.init[0],
				 specs.init[1],
				 p2[0],
				 p2[1],
				 specs.transversalA,
				 specs.transversalB,
				 specs.separatrix2);

  std::cout << "Inter 1: " << ret1.intersection.to_string() << std::endl;
  std::cout << "Inter 2: " << ret2.intersection.to_string() << std::endl;
  std::cout << "Dist: "<<systemFunctions[0](specs.init.data()) << std::endl;
  std::cout << "Deriv A: "<<jacobianFunctions[0](specs.init.data()) << std::endl;
  std::cout << "Deriv B: "<<jacobianFunctions[1](specs.init.data()) << std::endl;*/
  
  ::math::find_implicit_path_ret ret;
  ret = ::math::find_implicit_path(systemFunctions,
				 jacobianFunctions,
				 specs.init,
				 varIndex,
				 constraintVars,
				 specs.searchRadius,
				 specs.searchInc,
				 specs.inc,
				 specs.span);
  
  if(!ret.success) {
    return false;
  }

  saddleConnectionBifurcations.at(id).data = ret.data;
  saddleConnectionBifurcations.at(id).specs = specs;
  return true;
}

bool model::generate_limit_cycle_bifurcation_data(limit_cycle_bifurcation_id) {
  return false;
}

gen_singular_point_ret
model::gen_singular_point(const singular_point_specs& specs,
			  const ::math::vector& parameterVals) {
  std::vector<compiler::function<double, const double*>> systemFunctions;
  for(std::vector<expression>::const_iterator iter = system.begin();
      iter != system.end(); ++iter) {
    systemFunctions.push_back(iter->function);
  }
  std::vector<compiler::function<double, const double*>> jacobianFunctions;
  for(int r = 0; r != dynamicalVars; ++r) {
    for(int c = 0; c != dynamicalVars; ++c) {
      jacobianFunctions.push_back(partials[r][dynamicalIndexToSymbol[c]]
				  .function);
    }
  }
  std::vector<int> indices;
  ::math::vector init(symbolsToIndex.size());
  init[varDiffIndex] = 0; // This value is ignored.
  for(int i = 0; i != parameters; ++i) {
    init[parameterIndexToSymbol[i]] = parameterVals[i];
  }
  for(int i = 0; i != dynamicalVars; ++i) {
    init[dynamicalIndexToSymbol[i]] = specs.init[i];
    indices.push_back(dynamicalIndexToSymbol[i]);
  }
 
  if(!::math::newton_method(systemFunctions,
			  jacobianFunctions, indices,
			  init)) {
    std::cout << "Couldn't find singular point" << std::endl;
    return gen_singular_point_ret{singular_point(), false};
  }
  // We now create a new specification and update it the new init val.
  singular_point_specs newSpecs(specs);
  for(int i = 0; i != dynamicalVars; ++i) {
    newSpecs.init[i] = init[dynamicalIndexToSymbol[i]];
  }

  std::vector<double> data(symbolsToIndex.size());
  data[varDiffIndex] = 0; // This value is ignored for autonomous systems.
  for(int i = 0; i != dynamicalVars; ++i) {
    data[dynamicalIndexToSymbol[i]] = newSpecs.init[i];
  }
  for(int i = 0; i != parameters; ++i) {
    data[parameterIndexToSymbol[i]] = parameterVals[i];
  }
  // We now compute the jacobian matrix at this point and
  // evaluate its eigenvalues.
  ::math::square_matrix mat(dynamicalVars);
  for(int r = 0; r != dynamicalVars; ++r) {
    for(int c = 0; c != dynamicalVars; ++c) {
      mat[r][c] = partials[r][dynamicalIndexToSymbol[c]].function(data.data());
    }
  }
  std::vector<::math::eigenvalue> eigenvalues = mat.find_eigenvalues();
  // Compute the type of the singular point.
  // We first check whether they are all real or all complex, or mixed.
  bool foundReal(false);
  bool foundNonReal(false);
  for(int i = 0; i != eigenvalues.size(); ++i) {
    bool isReal = eigenvalues[i].get_value().get_root().is_real();
    foundReal |= isReal;
    foundNonReal |= !isReal;
  }
  bool foundPositive(false);
  bool foundNegative(false);
  for(int i = 0; i != eigenvalues.size(); ++i) {
    bool isPositive = eigenvalues[i].get_value().get_root().real() > 0;
    foundPositive |= isPositive;
    foundNegative |= !isPositive;
  }
  enum singular_point::type type;
  if(foundReal && !foundNonReal) {
    if(!foundPositive && foundNegative) {
      type = singular_point::type::STABLE_NODE;
    } else if(foundPositive && !foundNegative) {
      type = singular_point::type::UNSTABLE_NODE;
    } else {
      type = singular_point::type::SADDLE;
    }
  } else if(!foundReal && foundNonReal) {
    if(!foundPositive && foundNegative) {
      type = singular_point::type::STABLE_FOCUS;
    } else if(foundPositive && !foundNegative) {
      type = singular_point::type::UNSTABLE_FOCUS;
    } else {
      type = singular_point::type::SADDLE_FOCUS;
    }
  } else {
    if(!foundPositive && foundNegative) {
      type = singular_point::type::STABLE_FOCUS_NODE;
    } else if(foundPositive && !foundNegative) {
      type = singular_point::type::UNSTABLE_FOCUS_NODE;
    } else {
      type = singular_point::type::SADDLE_FOCUS_NODE;
    }
  }
  return gen_singular_point_ret(singular_point(newSpecs, newSpecs.init, eigenvalues, type), true);
}

bool model::generate_singular_point_data(singular_point_id id) {
  singular_point_specs specs = singularPoints.at(id).specs;
  gen_singular_point_ret ret = gen_singular_point(specs, parameterPosition);
  if(!ret.success) return false;
  singularPoints.at(id) = ret.singularPoint;
  return true;
}

void model::delete_singular_point(singular_point_id id) {
  singularPoints.erase(id);
}

namespace {
std::list<symbol>
gen_symbol_table(const std::string& varDiffName,
		 const std::vector<std::string>& dynamicalVarNames,
		 const std::vector<std::string>& parameterNames,
		 const std::map<std::string, int>& symbolsToIndex,
		 const std::unordered_map<int, std::string>& indexToSymbols) {
  std::list<symbol> symbolTable;
  int varDiffIndex(symbolsToIndex.at(varDiffName));
  symbolTable.push_back(symbol(varDiffName, varDiffIndex, varDiffIndex));
  for(int i = 0; i != dynamicalVarNames.size(); ++i) {
    std::string symbolName(dynamicalVarNames[i]);
    int index(symbolsToIndex.at(symbolName));
    symbolTable.push_back(symbol(symbolName, index, index));
  }
  for(int i = 0; i != parameterNames.size(); ++i) {
    std::string symbolName(parameterNames[i]);
    int index(symbolsToIndex.at(symbolName));
    symbolTable.push_back(symbol(symbolName, index, index));
  }
  symbolTable.sort([](const symbol& a, const symbol& b) -> bool {
      return a.parameter < b.parameter;
    });

  return symbolTable;
}
}
                    
bool model::compile(const std::string& newVarDiffName,
		    const std::vector<std::string>& newDynamicalVarNames,
		    const std::vector<std::string>& newParameterNames,
		    const std::vector<std::string>& newExpressions) {

  int newDynamicalVars = newExpressions.size();
  int newParameters = newParameterNames.size();
  std::map<std::string, int> newSymbolsToIndex;
  std::unordered_map<int, std::string> newIndexToSymbols;

  int index = -1;
  ++index;
  newSymbolsToIndex.insert(std::make_pair(newVarDiffName, index));
  newIndexToSymbols.insert(std::make_pair(index, newVarDiffName));
  for(const std::string& dynamicalVarName : newDynamicalVarNames) {
    ++index;
    newSymbolsToIndex.insert(std::make_pair(dynamicalVarName, index));
    newIndexToSymbols.insert(std::make_pair(index, dynamicalVarName));
  }
  for(const std::string& parameterName : newParameterNames) {
    ++index;
    newSymbolsToIndex.insert(std::make_pair(parameterName, index));
    newIndexToSymbols.insert(std::make_pair(index, parameterName));
  }
  std::list<symbol> symbolTable(gen_symbol_table(newVarDiffName,
						 newDynamicalVarNames,
						 newParameterNames,
						 newSymbolsToIndex,
						 newIndexToSymbols));
  std::vector<expression> newSystem;
  compiler::expression_parser parse;
  std::vector<std::vector<std::vector<expression>>> newDoublePartials;
  std::vector<std::vector<expression>> newPartials;
  for(int exprIndex = 0; exprIndex != newDynamicalVars; ++exprIndex) {
    AST ast;
    try {
      ast = parse.parse(newExpressions[exprIndex], symbolTable);
    } catch (const parser::syntax_exception& exc) {
      return false;
    }
    compiler::function<double, const double*> function = ast.compile();
    newSystem.push_back(expression{ast, function});
    newPartials.push_back(std::vector<expression>());
    newDoublePartials.push_back(std::vector<std::vector<expression>>());
    for(int partial1 = 0; partial1 != newSymbolsToIndex.size(); ++partial1) {
      std::string partialVar = newIndexToSymbols.at(partial1);
      AST partialAst = AST(ast).differentiate(partialVar);
      compiler::function<double, const double*> partialFunction
      	= partialAst.compile();
      expression partialExpr{partialAst, partialFunction};
      newPartials[exprIndex].push_back(partialExpr);
      newDoublePartials[exprIndex].push_back(std::vector<expression>());
      for(int partial2 = 0; partial2 != newSymbolsToIndex.size(); ++partial2) {
	std::string doublePartialVar = newIndexToSymbols.at(partial2);
	AST doublePartialAst = AST(partialAst).differentiate(doublePartialVar);
	compiler::function<double, const double*> doublePartialFunction
		  = doublePartialAst.compile();
	 expression doublePartialExpr{doublePartialAst, doublePartialFunction};
	 newDoublePartials[exprIndex][partial1].push_back(doublePartialExpr);
      }
    }
  }

  // We have successfully compiled!
  // We may now update our state.
  system = newSystem;
  partials = newPartials;
  doublePartials = newDoublePartials;
  expressions = newExpressions;
  dynamicalVars = newDynamicalVars;
  dynamicalVarNames = newDynamicalVarNames;
  parameterNames = newParameterNames;
  varDiffName = newVarDiffName;
  symbolsToIndex = newSymbolsToIndex;
  indexToSymbols = newIndexToSymbols;
  parameters = newParameters;
  varDiffIndex = 0;
  dynamicalVarIndex.clear();
  dynamicalIndexToSymbol.clear();
  for(int i = 0; i != dynamicalVarNames.size(); ++i) {
    dynamicalVarIndex.insert(std::make_pair(dynamicalVarNames[i], i));
    dynamicalIndexToSymbol.push_back(symbolsToIndex[dynamicalVarNames[i]]);
  }
  parameterIndex.clear();
  parameterIndexToSymbol.clear();
  for(int i = 0; i != parameterNames.size(); ++i) {
    parameterIndex.insert(std::make_pair(parameterNames[i], i));
    parameterIndexToSymbol.push_back(symbolsToIndex[parameterNames[i]]);
  }
  if(parameters == 0) 
    parameterPosition = ::math::vector(1, 0.0);
  else
    parameterPosition = ::math::vector(parameters, 0.0);
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
  separatrices.clear();
  hopfBifurcations.clear();
  saddleNodeBifurcations.clear();
  limitCycleBifurcations.clear();
  saddleConnectionBifurcations.clear();
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

int model::get_dynamical_dimension() const {
  return dynamicalVars;
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
				      const ::math::window2d& window) {
  dynamicalWindows.at(id).set_viewport_2d(window);
}

void model::set_dynamical_viewport_3d(dynamical_id id,
				      const ::math::window3d& window) {
  dynamicalWindows.at(id).set_viewport_3d(window);
}

void model::set_parameter_viewport(parameter_id id,
				   const ::math::window2d &window) {
  parameterWindows.at(id).set_viewport(window);
}

bool model::on_dynamical_vert_axis(dynamical_id id,
				   const ::math::vector2d& pos) const {
  return dynamicalWindows.at(id).on_vert_axis(pos);
}
bool model::on_dynamical_horiz_axis(dynamical_id id,
				    const ::math::vector2d& pos) const {
  return dynamicalWindows.at(id).on_horiz_axis(pos);
}

bool model::on_parameter_horiz_axis(parameter_id id,
				    const ::math::vector2d& pos) const {
  return parameterWindows.at(id).on_horiz_axis(pos);
}

bool model::on_parameter_vert_axis(parameter_id id,
				    const ::math::vector2d& pos) const {
  return parameterWindows.at(id).on_vert_axis(pos);
}

void model::set_solution_specs(solution_id id,
			       const solution_specs& spec) {
  solutions.at(id).specs = spec;
  generate_solution_data(id);
  for(std::unordered_map<dynamical_id, dynamical_window>::iterator iter
	= dynamicalWindows.begin(); iter != dynamicalWindows.end(); ++iter) {
    iter->second.gen_solution_vbo(id);
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

void model::delete_separatrix(separatrix_id id) {
  separatrices.erase(id);
  for(std::unordered_map<dynamical_id, dynamical_window>::iterator iter
	= dynamicalWindows.begin(); iter != dynamicalWindows.end(); ++iter) {
    iter->second.delete_separatrix(id);
  }
}

void model::delete_hopf_bifurcation(hopf_bifurcation_id id) {
  hopfBifurcations.erase(id);
  for(std::unordered_map<parameter_id, parameter_window>::iterator iter
	= parameterWindows.begin(); iter != parameterWindows.end(); ++iter) {
    iter->second.delete_hopf_bifurcation(id);
  }
}
void model::delete_saddle_node_bifurcation(saddle_node_bifurcation_id id) {
  saddleNodeBifurcations.erase(id);
  for(std::unordered_map<parameter_id, parameter_window>::iterator iter
	= parameterWindows.begin(); iter != parameterWindows.end(); ++iter) {
    iter->second.delete_saddle_node_bifurcation(id);
  }
}

void model::delete_limit_cycle_bifurcation(limit_cycle_bifurcation_id id) {
  limitCycleBifurcations.erase(id);
  for(std::unordered_map<parameter_id, parameter_window>::iterator iter
	= parameterWindows.begin(); iter != parameterWindows.end(); ++iter) {
    iter->second.delete_limit_cycle_bifurcation(id);
  }
}

void model::delete_saddle_connection_bifurcation(saddle_connection_bifurcation_id id) {
  saddleConnectionBifurcations.erase(id);
  for(std::unordered_map<parameter_id, parameter_window>::iterator iter
	= parameterWindows.begin(); iter != parameterWindows.end(); ++iter) {
    iter->second.delete_saddle_connection_bifurcation(id);
  }
}

void model::generate_solution_data(solution_id id) {
  solution_specs spec(solutions.at(id).specs);
  std::vector<compiler::function<double, const double*>> systemFunctions;
  for(std::vector<expression>::const_iterator iter = system.begin();
      iter != system.end(); ++iter) {
    systemFunctions.push_back(iter->function);
  }

  ::math::vector input(symbolsToIndex.size());
  for(int i = 0; i != parameterNames.size(); ++i) {
    input[parameterIndexToSymbol[i]] = parameterPosition[i];
  }
  for(int i = 0; i != dynamicalVarNames.size(); ++i) {
    input[dynamicalIndexToSymbol[i]] = spec.init[i];
  }
  input[varDiffIndex] = spec.tStart;

  // Set the data
  solutions.at(id).data =
    ::math::euler_method(systemFunctions, dynamicalIndexToSymbol,
		       varDiffIndex, input,
		       spec.inc, spec.tMin, spec.tMax);
}

bool model::select_solution(dynamical_id id, int x, int y) {
  return dynamicalWindows.at(id).select_solution(x, y, &selectedSolutionId);
}

bool model::select_separatrix(dynamical_id id, int x, int y) {
  return dynamicalWindows.at(id).select_separatrix(x, y, &selectedSeparatrixId);
}

bool model::select_isocline(dynamical_id id, int x, int y) {
  return dynamicalWindows.at(id).select_isocline(x, y, &selectedIsoclineId);
}

bool model::select_singular_point(dynamical_id id, int x, int y) {
  return dynamicalWindows.at(id)
    .select_singular_point(x, y, &selectedSingularPointId);
}

bool model::select_hopf_bifurcation(parameter_id id, int x, int y) {
  return parameterWindows.at(id)
    .select_hopf_bifurcation(x, y, &selectedHopfBifurcationId);
}

bool model::select_saddle_node_bifurcation(parameter_id id, int x, int y) {
  return parameterWindows.at(id)
    .select_saddle_node_bifurcation(x, y, &selectedSaddleNodeBifurcationId);
}

bool model::select_limit_cycle_bifurcation(parameter_id id, int x, int y) {
  return parameterWindows.at(id)
    .select_limit_cycle_bifurcation(x, y, &selectedLimitCycleBifurcationId);
}

bool model::select_saddle_connection_bifurcation(parameter_id id, int x, int y) {
  return parameterWindows.at(id)
    .select_saddle_connection_bifurcation(x, y, &selectedSaddleConnectionBifurcationId);
}

void model::deselect_separatrix() {
  selectedSeparatrixId = kNoSeparatrixId;
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

void model::deselect_hopf_bifurcation() {
  selectedHopfBifurcationId = kNoHopfBifurcationId;
}

void model::deselect_saddle_node_bifurcation() {
  selectedSaddleNodeBifurcationId = kNoSaddleNodeBifurcationId;
}

void model::deselect_limit_cycle_bifurcation() {
  selectedLimitCycleBifurcationId = kNoLimitCycleBifurcationId;
}

void model::deselect_saddle_connection_bifurcation() {
  selectedSaddleConnectionBifurcationId = kNoSaddleConnectionBifurcationId;
}

void model::select_separatrix(separatrix_id id) {
  selectedSeparatrixId = id;
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

void model::select_hopf_bifurcation(hopf_bifurcation_id id) {
  selectedHopfBifurcationId = id;
}

void model::select_saddle_node_bifurcation(saddle_node_bifurcation_id id) {
  selectedSaddleNodeBifurcationId = id;
}

void model::select_limit_cycle_bifurcation(limit_cycle_bifurcation_id id) {
  selectedLimitCycleBifurcationId = id;
}

void model::select_saddle_connection_bifurcation(saddle_connection_bifurcation_id id) {
  selectedSaddleConnectionBifurcationId = id;
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

separatrix_id model::get_selected_separatrix() const {
  return selectedSeparatrixId;
}

hopf_bifurcation_id model::get_selected_hopf_bifurcation() const {
  return selectedHopfBifurcationId;
}

saddle_node_bifurcation_id model::get_selected_saddle_node_bifurcation() const {
  return selectedSaddleNodeBifurcationId;
}

limit_cycle_bifurcation_id model::get_selected_limit_cycle_bifurcation() const {
  return selectedLimitCycleBifurcationId;
}

saddle_connection_bifurcation_id model::get_selected_saddle_connection_bifurcation() const {
  return selectedSaddleConnectionBifurcationId;
}

const std::unordered_map<separatrix_id, separatrix>&
model::get_separatrices() const {
  return separatrices;
}

const std::unordered_map<solution_id, solution>& model::get_solutions() const {
  return solutions;
}

const std::unordered_map<isocline_id, isocline>& model::get_isoclines() const {
  return isoclines;
}

const std::unordered_map<hopf_bifurcation_id, hopf_bifurcation>&
model::get_hopf_bifurcations() const {
  return hopfBifurcations;
}

const std::unordered_map<saddle_node_bifurcation_id, saddle_node_bifurcation>&
model::get_saddle_node_bifurcations() const {
  return saddleNodeBifurcations;
}

const std::unordered_map<saddle_connection_bifurcation_id, saddle_connection_bifurcation>&
model::get_saddle_connection_bifurcations() const {
  return saddleConnectionBifurcations;
}

const std::unordered_map<limit_cycle_bifurcation_id, limit_cycle_bifurcation>&
model::get_limit_cycle_bifurcations() const {
  return limitCycleBifurcations;
}

find_separatrix_intersection_ret
model::find_separatrix_intersection(double paramA,
				    double paramB,
				    double sing1,
				    double sing2,
				    const ::math::vector2d& transA,
				    const ::math::vector2d& transB,
				    const separatrix_id& id) {
  separatrix_specs specs = separatrices.at(id).specs;
  specs.inc = 0.01;
  singular_point_specs singularPointSpecs;
  ::math::vector initCondition(2);
  initCondition[0] = sing1;
  initCondition[1] = sing2;
  singularPointSpecs.init = initCondition;
  gen_separatrix_ret ret = gen_separatrix(singularPointSpecs,
					  specs,
					  ::math::vector2d(paramA, paramB));
  if(!ret.success) {
    std::cout << "No Separatrix" << std::endl;
    return find_separatrix_intersection_ret();
  }

  // If its a stable separatrix we go in reverse direction.
  if(ret.sep.specs.type == separatrix_specs::type::STABLE) {
    ret.sep.data.reverse();
  }
  ::math::vector2d prev = *(ret.sep.data.begin());
  ::math::vector2d orig(prev);
  for(const ::math::vector2d& next : ret.sep.data) {
    if(::math::splits(orig, next, transA, transB)) {
      ::math::vector2d inter = ::math::intersect(prev, next, transA, transB);
      return find_separatrix_intersection_ret(inter,
					      true);
    }
    prev = next;
  }
  std::cout << "Fail No Intersection" << std::endl;
  return find_separatrix_intersection_ret();
}

gen_separatrix_ret model::gen_separatrix(const singular_point_specs& singularPointSpecs,
					 const separatrix_specs& separatrixSpecs,
					 ::math::vector parameterVals) {
  gen_singular_point_ret ret = gen_singular_point(singularPointSpecs,
						  parameterVals);
  if(!ret.success) {
    std::cout << "Fail No Singular Point" << std::endl;
    return gen_separatrix_ret();
  }
  singular_point singularPoint(ret.singularPoint);
  if(singularPoint.type != singular_point::type::SADDLE) {
    std::cout << "Fail Not Saddle" << std::endl;
    return gen_separatrix_ret();
  }
  
  ::math::vector init(symbolsToIndex.size());
  init[varDiffIndex] = 0; // This is ignored by the functions.
  for(int i = 0; i != parameters; ++i) {
    init[parameterIndexToSymbol[i]] = parameterVals[i];
  }
  // We are working in 2 dimensions. For a stable separatrix, our starting
  // conditions should be a slight perturbation along the stable eigenvector
  // we should move in the negative t direction.
  int index;
  if(separatrixSpecs.type == separatrix_specs::type::STABLE) {
    if(singularPoint.eigenvalues[0].get_value().get_root().real() < 0) {
      index = 0;
    } else {
      index = 1;
    }
  } else {
    if(singularPoint.eigenvalues[0].get_value().get_root().real() > 0) {
      index = 0;
    } else {
      index = 1;
    }
  }
  double multiplier = 1e-5;
  if(!separatrixSpecs.direction) {
    multiplier *= -1;
  }
  init[dynamicalIndexToSymbol[0]]
    = singularPoint.position[0]
    + multiplier * singularPoint.eigenvalues[index].get_vectors()[0][0];
  init[dynamicalIndexToSymbol[1]]
    = singularPoint.position[1]
    + multiplier * singularPoint.eigenvalues[index].get_vectors()[0][1];

  std::vector<compiler::function<double, const double*>> systemFunctions;
  for(std::vector<expression>::const_iterator iter = system.begin();
      iter != system.end(); ++iter) {
    systemFunctions.push_back(iter->function);
  }

  std::list<::math::dynamical_point> points;
  if(separatrixSpecs.type == separatrix_specs::type::STABLE) {
    points = ::math::euler_method(systemFunctions,
				dynamicalIndexToSymbol,
				varDiffIndex,
				init,
				separatrixSpecs.inc,
				-separatrixSpecs.time,
				0);
  } else {
    points = ::math::euler_method(systemFunctions,
				dynamicalIndexToSymbol,
				varDiffIndex,
				init,
				separatrixSpecs.inc,
				0,
				separatrixSpecs.time);
  }

  std::list<::math::vector2d> data;
  for(const ::math::dynamical_point& point : points) {
    data.push_back(::math::vector2d(point.vars[0], point.vars[1]));
  }
  
  return gen_separatrix_ret{singularPoint,
      separatrix{separatrixSpecs, data},
      true};
}


bool model::generate_separatrix_data(separatrix_id id) {
  // We first verify that the singular point associated with this separatrix
  // still exists and is still a saddle.
  const separatrix_specs& specs = separatrices.at(id).specs;
  
  if(singularPoints.find(specs.singularPoint) == singularPoints.end()) {
    return false;
  }
  const singular_point& singularPoint = singularPoints.at(specs.singularPoint);
  if(singularPoint.type != singular_point::type::SADDLE) {
    return false;
  }

  singular_point_specs singularPointSpecs =
    singularPoints.at(specs.singularPoint).specs;
  gen_separatrix_ret ret = gen_separatrix(singularPointSpecs,
					  specs,
					  parameterPosition);
  if(!ret.success) return false;
  separatrices.at(id) = ret.sep;
  
  return true;
}

bool model::generate_hopf_bifurcation_data(hopf_bifurcation_id id) {
  // We work in two dimensions with two parameters.
  // We solve f(x, y, a, b) = 0, g(x, y, a, b) = 0, f_x + g_y = 0,
  // and some additional constraint. Initially the constraint is a circle about
  // the initial points.
  hopf_bifurcation_specs specs = hopfBifurcations.at(id).specs;
  std::vector<std::function<double(const double*)>> systemFunctions;
  std::vector<std::function<double(const double*)>> jacobianFunctions;

  // f = 0
  systemFunctions.push_back([&](const double* arr) -> double {
      return system[0].function(arr);
    });
  // g = 0
  systemFunctions.push_back([&](const double* arr) -> double {
      return system[1].function(arr);
    });
  // f_x + g_y = 0
  systemFunctions.push_back([&](const double* arr) -> double {
      return partials[0][dynamicalIndexToSymbol[0]].function(arr)
	+ partials[1][dynamicalIndexToSymbol[1]].function(arr);
    });

  double searchRadius = 0.001;
  // (a-a_0)^2 + (b-b_0)^2 - r^2 = 0
  systemFunctions.push_back([&](const double* arr) -> double {
      double a = arr[parameterIndexToSymbol[0]] - specs.init.parameters[0];
      double b = arr[parameterIndexToSymbol[1]] - specs.init.parameters[1];
      return a * a + b * b - searchRadius * searchRadius;
    });

  // We now build the jacobian.
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[0][dynamicalIndexToSymbol[0]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[0][dynamicalIndexToSymbol[1]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[0][parameterIndexToSymbol[0]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[0][parameterIndexToSymbol[1]].function(arr);
    });

  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[1][dynamicalIndexToSymbol[0]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[1][dynamicalIndexToSymbol[1]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[1][parameterIndexToSymbol[0]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[1][parameterIndexToSymbol[1]].function(arr);
    });

  jacobianFunctions.push_back([&](const double* arr) -> double {
      return
	doublePartials[0][dynamicalIndexToSymbol[0]][dynamicalIndexToSymbol[0]]
	.function(arr)
	+
	doublePartials[1][dynamicalIndexToSymbol[1]][dynamicalIndexToSymbol[0]]
	.function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return
	doublePartials[0][dynamicalIndexToSymbol[0]][dynamicalIndexToSymbol[1]]
	.function(arr)
	+
	doublePartials[1][dynamicalIndexToSymbol[1]][dynamicalIndexToSymbol[1]]
	.function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return
	doublePartials[0][dynamicalIndexToSymbol[0]][parameterIndexToSymbol[0]]
	.function(arr)
	+
	doublePartials[1][dynamicalIndexToSymbol[1]][parameterIndexToSymbol[0]]
	.function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return
	doublePartials[0][dynamicalIndexToSymbol[0]][parameterIndexToSymbol[1]]
	.function(arr)
	+
	doublePartials[1][dynamicalIndexToSymbol[1]][parameterIndexToSymbol[1]]
	.function(arr);
    });

  jacobianFunctions.push_back([&](const double* arr) -> double {
      return 0;
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return 0;
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return 2 * (arr[parameterIndexToSymbol[0]] - specs.init.parameters[0]);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return 2 * (arr[parameterIndexToSymbol[1]] - specs.init.parameters[1]);
    });
  
  // Setup the initial search vector
  ::math::vector init(symbolsToIndex.size());
  std::vector<int> varIndex;
  init[varDiffIndex] = 0; // This value is ignored by the functions
  for(int i = 0; i != dynamicalVars; ++i) {
    init[dynamicalIndexToSymbol[i]] = specs.init.dynamicalVars[i];
    varIndex.push_back(dynamicalIndexToSymbol[i]);
  }
  for(int i = 0; i != parameters; ++i) {
    init[parameterIndexToSymbol[i]] = specs.init.parameters[i];
    varIndex.push_back(parameterIndexToSymbol[i]);
  }

  std::unordered_set<int> constraintVars;
  constraintVars.insert(2);
  constraintVars.insert(3);

  ::math::find_implicit_path_ret ret =
    ::math::find_implicit_path(systemFunctions,
			     jacobianFunctions,
			     init,
			     varIndex,
			     constraintVars,
			     specs.searchRadius,
			     specs.searchInc,
			     specs.inc,
			     specs.span);

  if(!ret.success) return false;

  std::list<::math::vector> points;
  for(const ::math::vector& point : ret.data) {
    ::math::vector vec(2);
    vec[0] = point[2];
    vec[1] = point[3];
    points.push_back(vec);
  }
  
  hopfBifurcations.at(id).data = points;
  hopfBifurcations.at(id).specs = specs;
  return true;
}
bool model::generate_saddle_node_bifurcation_data(saddle_node_bifurcation_id id) {
  // We work in two dimensions with two parameters.
  // We solve f(x, y, a, b) = 0, g(x, y, a, b) = 0, f_x * g_y - g_x * f_y = 0,
  // and some additional constraint. Initially the constraint is a circle about
  // the initial points.
  saddle_node_bifurcation_specs specs = saddleNodeBifurcations.at(id).specs;
  std::vector<std::function<double(const double*)>> systemFunctions;
  std::vector<std::function<double(const double*)>> jacobianFunctions;

  // f = 0
  systemFunctions.push_back([&](const double* arr) -> double {
      return system[0].function(arr);
    });
  // g = 0
  systemFunctions.push_back([&](const double* arr) -> double {
      return system[1].function(arr);
    });
  // f_x * g_y - g_x * f_y = 0
  systemFunctions.push_back([&](const double* arr) -> double {
      return partials[0][dynamicalIndexToSymbol[0]].function(arr)
	* partials[1][dynamicalIndexToSymbol[1]].function(arr)
	- partials[1][dynamicalIndexToSymbol[0]].function(arr)
	* partials[0][dynamicalIndexToSymbol[1]].function(arr);
    });

  // We now build the jacobian.
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[0][dynamicalIndexToSymbol[0]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[0][dynamicalIndexToSymbol[1]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[0][parameterIndexToSymbol[0]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[0][parameterIndexToSymbol[1]].function(arr);
    });

  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[1][dynamicalIndexToSymbol[0]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[1][dynamicalIndexToSymbol[1]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[1][parameterIndexToSymbol[0]].function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return partials[1][parameterIndexToSymbol[1]].function(arr);
    });

  jacobianFunctions.push_back([&](const double* arr) -> double {
      return
	partials[0][dynamicalIndexToSymbol[0]].function(arr) *
	doublePartials[1][dynamicalIndexToSymbol[1]][dynamicalIndexToSymbol[0]]
	.function(arr)
	+
	partials[1][dynamicalIndexToSymbol[1]].function(arr) *
	doublePartials[0][dynamicalIndexToSymbol[0]][dynamicalIndexToSymbol[0]]
	.function(arr)
	-
	partials[1][dynamicalIndexToSymbol[0]].function(arr) *
	doublePartials[0][dynamicalIndexToSymbol[1]][dynamicalIndexToSymbol[0]]
	.function(arr)
	-
	partials[0][dynamicalIndexToSymbol[1]].function(arr) *
	doublePartials[1][dynamicalIndexToSymbol[0]][dynamicalIndexToSymbol[0]]
	.function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return
	partials[0][dynamicalIndexToSymbol[0]].function(arr) *
	doublePartials[1][dynamicalIndexToSymbol[1]][dynamicalIndexToSymbol[1]]
	.function(arr)
	+
	partials[1][dynamicalIndexToSymbol[1]].function(arr) *
	doublePartials[0][dynamicalIndexToSymbol[0]][dynamicalIndexToSymbol[1]]
	.function(arr)
	-
	partials[1][dynamicalIndexToSymbol[0]].function(arr) *
	doublePartials[0][dynamicalIndexToSymbol[1]][dynamicalIndexToSymbol[1]]
	.function(arr)
	-
	partials[0][dynamicalIndexToSymbol[1]].function(arr) *
	doublePartials[1][dynamicalIndexToSymbol[0]][dynamicalIndexToSymbol[1]]
	.function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return
	partials[0][dynamicalIndexToSymbol[0]].function(arr) *
	doublePartials[1][dynamicalIndexToSymbol[1]][parameterIndexToSymbol[0]]
	.function(arr)
	+
	partials[1][dynamicalIndexToSymbol[1]].function(arr) *
	doublePartials[0][dynamicalIndexToSymbol[0]][parameterIndexToSymbol[0]]
	.function(arr)
	-
	partials[1][dynamicalIndexToSymbol[0]].function(arr) *
	doublePartials[0][dynamicalIndexToSymbol[1]][parameterIndexToSymbol[0]]
	.function(arr)
	-
	partials[0][dynamicalIndexToSymbol[1]].function(arr) *
	doublePartials[1][dynamicalIndexToSymbol[0]][parameterIndexToSymbol[0]]
	.function(arr);
    });
  jacobianFunctions.push_back([&](const double* arr) -> double {
      return
	partials[0][dynamicalIndexToSymbol[0]].function(arr) *
	doublePartials[1][dynamicalIndexToSymbol[1]][parameterIndexToSymbol[1]]
	.function(arr)
	+
	partials[1][dynamicalIndexToSymbol[1]].function(arr) *
	doublePartials[0][dynamicalIndexToSymbol[0]][parameterIndexToSymbol[1]]
	.function(arr)
	-
	partials[1][dynamicalIndexToSymbol[0]].function(arr) *
	doublePartials[0][dynamicalIndexToSymbol[1]][parameterIndexToSymbol[1]]
	.function(arr)
	-
	partials[0][dynamicalIndexToSymbol[1]].function(arr) *
	doublePartials[1][dynamicalIndexToSymbol[0]][parameterIndexToSymbol[1]]
	.function(arr);
    });

  
  
  // Setup the initial search vector
  ::math::vector init(symbolsToIndex.size());
  std::vector<int> varIndex;
  init[varDiffIndex] = 0; // This value is ignored by the functions
  for(int i = 0; i != dynamicalVars; ++i) {
    init[dynamicalIndexToSymbol[i]] = specs.init.dynamicalVars[i];
    std::cout << init[dynamicalIndexToSymbol[i]] << std::endl;
    varIndex.push_back(dynamicalIndexToSymbol[i]);
  }
  for(int i = 0; i != parameters; ++i) {
    init[parameterIndexToSymbol[i]] = specs.init.parameters[i];
    varIndex.push_back(parameterIndexToSymbol[i]);
  }
  std::unordered_set<int> constraintVars;
  constraintVars.insert(2);
  constraintVars.insert(3);

  ::math::find_implicit_path_ret ret =
    ::math::find_implicit_path(systemFunctions,
			     jacobianFunctions,
			     init,
			     varIndex,
			     constraintVars,
			     specs.searchRadius,
			     specs.searchInc,
			     specs.inc,
			     specs.span);

  if(!ret.success) return false;

  std::list<::math::vector> points;
  for(const ::math::vector& point : ret.data) {
    ::math::vector vec(2);
    vec[0] = point[2];
    vec[1] = point[3];
    points.push_back(vec);
  }
  saddleNodeBifurcations.at(id).data = points;
  saddleNodeBifurcations.at(id).specs = specs;
  return true;
}

bool model::generate_isocline_data(isocline_id id) {
  // Let n be the dimension of the system.
  // We solve the equation g(x1, x2, ..., xn, k) = F(x) - ka = 0 for x and k
  // where x is an n dimensional vector of the system and k is a scalar
  // multiple. a is the vector indicating the direction of flow through the
  // isocline we are trying to find. Thus we have a system of n + 1 unknowns and
  // n constraints. We must choose a further constraint by limiting one of the
  // variables to a given value and computing the others. For instance we
  // arbitrarily choose to add the equation x1 - m = 0 for some value m.
  //
  // The input vector to newtos method is organized according to symbolsToIndex
  // The variable k is given the index after all symbols. In particular,
  // its index is symbolsToIndex.size()
  //
  // The systemFunctions are organized as the dynamical variables followd
  // by the variable k.
  //
  // jacobianSystem has the jacobian of F(x) as a submatrix aligned against
  // the top left corner. The last column is the k partial derivative of each
  // function so this amounts to (-a, 0) as the last column. If the restriction
  // functions is of the form x_l - m = 0 then the last row are all
  // zeros  except in the entry l - 1 which is a 1.
  //
  // The indices vector is simply a vector containing the integers,
  // 1 2 3 4 ... n+1
  isocline_specs specs = isoclines.at(id).specs;
  int kIndex = symbolsToIndex.size();
  std::vector<std::function<double(const double*)>> systemFunctions;
  std::vector<std::function<double(const double*)>> jacobianFunctions;
  for(int i = 0; i != dynamicalVars; ++i) {
    systemFunctions.push_back([&, i](const double* arr) -> double {
	return system[i].function(arr) - arr[kIndex] * specs.direction[i];
      });
  }
  
  for(int r = 0; r != dynamicalVars; ++r) {
    for(int c = 0; c != dynamicalVars; ++c) {
      jacobianFunctions.push_back(partials[r][dynamicalIndexToSymbol[c]]
				  .function);
    }
    jacobianFunctions.push_back([&, r](const double*) -> double {
	return -specs.direction[r];
      });
  }
  
  // Setup the initial search vector
  ::math::vector init(kIndex + 1);
  init[varDiffIndex] = 0; // This value is ignored by the functions
  for(int i = 0; i != dynamicalVars; ++i) {
    init[dynamicalIndexToSymbol[i]] = specs.init[i];
  }
  for(int i = 0; i != parameters; ++i) {
    init[parameterIndexToSymbol[i]] = parameterPosition[i];
  }
  // Initially we set the kValue to be 1.
  init[kIndex] = 1;

  // Setup the var index vector.
  std::vector<int> varIndex(dynamicalIndexToSymbol);
  varIndex.push_back(kIndex);

  std::unordered_set<int> constraintVars;
  for(int i = 0; i != dynamicalVars; ++i) {
    constraintVars.insert(i);
  }

  ::math::find_implicit_path_ret ret =
    ::math::find_implicit_path(systemFunctions,
			     jacobianFunctions,
			     init,
			     varIndex,
			     constraintVars,
			     specs.searchRadius,
			     specs.searchInc,
			     specs.inc,
			     specs.span);

  if(!ret.success) return false;
  
  isoclines.at(id).data = ret.data;
  isoclines.at(id).specs = specs;
  return true;
}

const dynamical_specs& model::get_dynamical_specs(dynamical_id id) const {
  return dynamicalWindows.at(id).get_specs();
}

const parameter_specs& model::get_parameter_specs(parameter_id id) const {
  return parameterWindows.at(id).get_specs();
}

int model::get_parameters() const {
  return parameters;
}

const std::vector<std::string>& model::get_dynamical_names() const {
  return dynamicalVarNames;
}

const std::vector<std::string>& model::get_parameter_names() const {
  return parameterNames;
}
const std::string& model::get_var_diff_name() const {
  return varDiffName;
}

void model::set_parameter_position(parameter_id id, const ::math::vector2d& pos) {
  const parameter_specs& specs = parameterWindows.at(id).get_specs();
  ::math::vector2d realPos(specs.viewport.real_coordinate_of(pos));
  parameterPosition[parameterIndex.at(specs.horizAxisVar)] = realPos.x();
  parameterPosition[parameterIndex.at(specs.vertAxisVar)] = realPos.y();
  // For each solution
  for(solution_const_iter iter = solutions.begin();
      iter != solutions.end(); ++iter ) {
    generate_solution_data(iter->first);
  }
  std::vector<isocline_id> toRemoveIsocline;
  for(isocline_const_iter iter = isoclines.begin();
      iter != isoclines.end(); ++iter ) {
    if(!generate_isocline_data(iter->first)) {
      toRemoveIsocline.push_back(iter->first);
    }
  }
  for(std::vector<isocline_id>::const_iterator iter = toRemoveIsocline.begin();
      iter != toRemoveIsocline.end(); ++iter) {
    delete_isocline(*iter);
  }
  std::vector<singular_point_id> toRemoveSingularPoint;
  for(singular_point_const_iter iter = singularPoints.begin();
      iter != singularPoints.end(); ++iter ) {
    if(!generate_singular_point_data(iter->first)) {
      toRemoveSingularPoint.push_back(iter->first);
    }
  }
  for(std::vector<singular_point_id>::const_iterator iter
	= toRemoveSingularPoint.begin();
      iter != toRemoveSingularPoint.end(); ++iter) {
    delete_singular_point(*iter);
  }
  // Now that we have removed the singular points, we update and potentially
  // remove separatrices.
  std::vector<separatrix_id> toRemoveSeparatrix;
  for(separatrix_const_iter iter = separatrices.begin();
      iter != separatrices.end(); ++iter) {
    if(!generate_separatrix_data(iter->first)) {
      toRemoveSeparatrix.push_back(iter->first);
    }
  }
  for(std::vector<separatrix_id>::const_iterator iter
	= toRemoveSeparatrix.begin(); iter != toRemoveSeparatrix.end(); ++iter) {
    delete_separatrix(*iter);
  }
  // We now update the VBO's of all the dynamical/parameter windows
  for(dynamical_iter iter = dynamicalWindows.begin();
      iter != dynamicalWindows.end(); ++iter) {
    for(solution_const_iter sol = solutions.begin();
	sol != solutions.end(); ++sol) {
      iter->second.gen_solution_vbo(sol->first);
    }
    for(isocline_const_iter iso = isoclines.begin();
	iso != isoclines.end(); ++iso) {
      iter->second.gen_isocline_vbo(iso->first);
    }
    for(separatrix_const_iter sep = separatrices.begin();
	sep != separatrices.end(); ++sep) {
      iter->second.gen_separatrix_vbo(sep->first);
    }
  }
}

bool model::on_parameter_position(parameter_id id, const ::math::vector2d& pos) const {
  return parameterWindows.at(id).on_parameter_position(pos);
}

// Returns the dynamical point associated with the given mouse position
// in this dynamical window.
::math::dynamical_point model::get_dynamical_point(dynamical_id id, const ::math::vector2d& pos) const {
  return dynamicalWindows.at(id).get_point(pos);
}

::math::vector model::get_parameter_point(parameter_id id,
					const ::math::vector2d& pos) const {
  return parameterWindows.at(id).get_point(pos);
}

const ::math::vector& model::get_parameter_position() const {
  return parameterPosition;
}
} // namespace gui
}
