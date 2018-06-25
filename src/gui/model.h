#ifndef DYNSOLVER_GUI_MODEL_H_
#define DYNSOLVER_GUI_MODEL_H_

#include <vector>
#include <unordered_map>

#include <glad/glad.h>

#include "common.h"
#include "compiler/ast/AST.h"
#include "compiler/function.h"
#include "math/vector.h"
#include "gl/buffer.h"
#include "gl/program.h"
#include "gl/vertex_array.h"
#include "gl/font.h"
#include "gl/text_renderer.h"

class wxGLCanvas;
class wxGLContext;

namespace dynsolver {
namespace math {
class square_matrix;
}
namespace gui {

class app;

// Contains helper classes
// Represents a point in 2 dimensions.
class point2d : public math::vector {
 public:
  // Constructs a point2d without initializing its element.
  point2d();
  
  // Constructs a point at position (x, y).
  // May also be used to represent a scaling vector, etc.
  point2d(double x, double y);

  // Allows for mutable access of x and y.
  const double& x() const;
  double& x();
    
  const double& y() const;
  double& y();
};

// Represents a window into a 2d plane that has both a size in pixels
// a spanning size of the plane and a location.
// Note that pixels are given realative to the bottom left corner
// of the plane. In particular, upwards is the POSITIVE y direction.
class window2d {
 private:
  // The size in pixels of window. The x coordinate of size is the width,
  // and the y coordinate of size is the height.
  point2d size;

  // The real spanning distance of this window. The distance spanned by
  // the window in the x and y directions.
  point2d span;

  // This is the position of the bottom right corner of the window in the plane.
  point2d position;

 public:
  // Constructs a window with the given information.
  window2d(point2d size, point2d span, point2d position);
  
  // Returns the pixel associated with the real coordinate provided.
  point2d pixel_coordinate_of(point2d real) const;
  
  // Returns the real cordinated associated with the given pixel.
  point2d real_coordinate_of(point2d pixel) const;

  // True if the pixel corrdinates are nonnegative and are
  // within the bounds of size exclusive.
  bool contains_pixel(point2d pixel) const;

  // Returns true if the real coordinate provided is within the span
  // of the window.
  bool contains_real(point2d real) const;

  // Translates the window by the number of pixels specified in pixel.
  // Note that upwards movement is in the negative y direction.
  void move_pixel(point2d pixel);

  // Translates the window by the real value provided.
  void move_real(point2d real);

  void set_span(const point2d& newSpan);

  // Scales the window by scale about the real point provided.
  // The resulting window has a the same size as before, but the
  // new x span is scale.x() * span.x()
  // and new y span of scale.y() * span.y().
  // The point real remains fixed in the new window. That is, it is
  // associated with the same pixel as before.
  void scale_real(point2d scale, point2d real);

  // Scales the window by scale about the pixel point provided.
  void scale_pixel(point2d scale, point2d pixel);

  const point2d& get_size() const;
  void set_size(const point2d&);
  
  const point2d& get_position() const;
  const point2d& get_span() const;

  // Returns the center of the this window in real coordinates.
  point2d get_center() const;

  // This transformation matrix is a 4x4 matrix which maps
  // real coordinates to normalized device coordinates for use in opengl.
  math::square_matrix real_to_ndc();

  // This transformation matrix is a 4x4 matrix which maps
  // pixel coordinate to normalized device coordinates for use in opengl.
  // In pixel coordinates, the origin is the lower left corner of the screen.
  math::square_matrix pixel_to_ndc();
};

struct solution_specification {
  solution_specification();
  
  // The starting point of the solution including the initial value of t.
  math::vector initialValue;

  // Invariant:
  // tMax must be strictly greater than tMin. initial[0] must be
  // strictly between tMax and tMin
  // increment is strictly positive.
  
  // The bounds on how far tMax and tMin should run.
  double tMax;
  double tMin;

  // Currently we use standard euler's method. This indicates the standard
  // increment.
  double increment;
};

// A specification for drawing a solution through an initial point.
struct solution {
  solution_specification specification;

  std::list<math::vector> data;
};

struct singular_point_specification {
  singular_point_specification();
  math::vector initialValue;
};

// Used to pass to and from the dynamical window dialog to obtain
// imformation on the viewport and axes for generating or changing a dynamical
// window.
struct dynamical_window_specification {
  unsigned int dynamicalVariables;
  
  int horizontalAxisVariable;
  int verticalAxisVariable;

  // The real minimum and maximum viewport bounds.
  double horizontalAxisMin;
  double horizontalAxisMax;
  double verticalAxisMin;
  double verticalAxisMax;
};

// A tuple which associates an abstract syntax tree with its compiled
// function.
struct expression {
  AST ast;
  compiler::function<double, const double*> function;
};

// This class is used to hold all the data behind the GUI system.
// In general, the data can only be accessed using getters and setters.
// This class almost completely decoupled from the view. It is the responsibility of
// the view to update the user interface when it expects that a value in the
// model has changed. The exception is that all opengl work occurs in this
// file.
//
// NOTE: The model may NOT be constructed until an opengl context is set current,
// and the opengl function pointers are loaded.
class model {
public:
  static const std::string k2dVertexShaderFilePath;
  static const GLuint k2dPositionAttribute;
  static const GLuint k2dVertexBinding;
  static const std::string k2dTransformationUniform;
  
  static const std::string k2dFragmentShaderFilePath;
  static const std::string k2dColorUniform;
  
 private:
    // Represents information associated with an instance of a dynamical window.
  // Multiple dynamical windows may be open at any given time.
  class dynamical_window {
  private:
    // A reference to the containing model. Since this is an inner class of model,
    // it may access all of its private variables.
    model& modelData;
    
    // This represents the viewport area of the dynamical plane visible
    // in this window.
    window2d window;
  
    // An integer representing which variable corresponds to which axis.
    // A 0 indicates the variable of differentiation t. 1 indicates x (x1),
    // 2 indicates y (x2), etc.. -1 indicates that no axes have been selected.
    int horizontalAxisVariable;
    int verticalAxisVariable;

    gl::buffer axesVbo;
    GLsizei axesVboVertexCount;

    struct solution_render_data {
      gl::buffer vbo;
      unsigned int vertices;
    };
  
    std::unordered_map<dynamical_window_id, solution_render_data> solutionRenderData;

    // The number of ticks between each labeled tick.
    int ticksPerLabel;

    // The font size of the labeled tick.
    double tickFontSize;

    // The maximal distance between two axis ticks in pixels. If the number of
    // axis tickets jumps above the maximum, then the number of tickets are doubled,
    // and the real distance between each tick is halved. If it falls below the minimum,
    // the distance between each tick is doubled.
    const int minimumPixelTickDistance;
    const int maximumPixelTickDistance;


    // The current tick distance in real coordnate values.
    double realTickDistanceX;
    double realTickDistanceY;
  public:
    dynamical_window(model&,
		     const window2d& window,
		     int horizontalAxisVariable,
		     int verticalAxisVariable);

    void set_specification(const dynamical_window_specification& spec);

    int get_horizontal_axis_variable() const;

    int get_vertical_axis_variable() const;
    void set_window(const window2d&);

    // Renders the window to the currently bound context.
    void paint();

    // Called when the dynamical window is resized. The new
    // width and height is given.
    void resize(double width, double height);

    // Generates a VBo for the corresponding solution data and id.
    void generate_vbo(solution_id, const std::list<math::vector>&);
    
    // Generates a VBo for the corresponding solution data and id.
    void update_vbo(solution_id);

    const window2d& get_window() const;

    void clear();

    bool on_vertical_axis(double, double) const;
    bool on_horizontal_axis(double, double) const;
  };
  
  gl::font font;
  gl::text_renderer textRenderer;
  
  // The number of variables involved including the variable of differentiation, t.
  // Should always be 2 or greater. This number shall be denoted as n, and the dimension
  // of the vector field shall be denoted as d = n - 1.
  // We only store the dimension of the vector field.
  int dynamicalDimension;

  int parameterVariables;
  math::vector parameterPosition;

  // A list of expressions corresponding to the abstract syntax trees.
  // The size is d.
  std::vector<std::string> expressions;

  // Each dynamical and parameter window will have an id. The id is associated
  // with the some information in the following maps. The id is also used to
  // lookup the appropriate frames in the app class.
  // Consider using strong typdefs
  std::unordered_map<dynamical_window_id, dynamical_window> dynamicalWindows;
  
  // This is an id number which is not currently mapped in dynamicalWindows map.
  dynamical_window_id uniqueDynamicalId;

  
  // The following variables are updated on every recompile.
  
  // The following stores CAS syntax tree info.
  // The system of equations as a syntax tree.
  // These two variables are updated everytime a string is compiled.
  // Variables are ordered from 0 to d.
  // We require that jacobian has size of d * d.
  // System.size() is equal to d.
  std::vector<expression> system;
  
  // The jacobian of the system of equations stored in ROW-MAJOR order.
  // The size is d^2.
  std::vector<expression> jacobian;

  // The following variables are updated on each change to parameterPosition
  // and to each change to system or jacobian (on any recompilation).
  
  // The following variables demonstrate what to draw on the dynamical plane.
  // They are specifications for what to draw, and the actually pixel data.
  // Each solution has a unique id.
  std::unordered_map<solution_id, solution> solutions;
  
  // All solutions mapped have id's strictly less than this number.
  solution_id uniqueSolutionId;

  window2d defaultWindow;

  // True if the model is currently representing a compiled system.
  // False if no system is being viewed. If false, the following variables,
  // have no meaning and do not need to satisfy any invariants.
  bool compiled;

  // These two variables represent the opengl program and VAO.
  gl::vertex_array vao2d;
  gl::program program2d;
  const GLuint k2dTransformationUniformLocation;
  const GLuint k2dColorUniformLocation;
  
  std::vector<gl::shader> build_shaders();

public:
  model();

  void paint_dynamical_window(dynamical_window_id id);
  void resize_dynamical_window(dynamical_window_id id, double width, double height);
  
  // The dynamical dimension is one less than the number of dynamical variables.
  int get_dynamical_dimension() const;

  // Returns the number of dynamical variables including the variable of
  // integration t.
  int get_dynamical_variables() const;
  
  // Adds the initial value solution. Init, is the initial point
  // whose first entry is the time component, second entry is the first
  // variable etc... Requires the size of init to be equal to n. Requires,
  // that init[0] is strictly between tMin and tMax and that tMin
  // is strictly less than tMax. Requires that increment is positive.
  void add_solution(const solution_specification&);

  // Resets most variables and compiles the array of strings into a vector
  // field system of ODE's. Sets the dimension etc... Returns true
  // on success.
  // Requires that the vector has size 1 or more.
  bool compile(std::vector<std::string>);

  // These functions add dynamical and parameter windows. They return the id,
  // of the newly added window. The width and height of the window are specified,
  // in pixels.
  int add_dynamical_window(const dynamical_window_specification&, int width, int height);

  void set_dynamical_window_specification(const dynamical_window_specification& spec,
					  dynamical_window_id id);

  // Deletes all the data associated with the given window.
  void delete_dynamical_window(dynamical_window_id id);

  // Deletes all the data associated with all dynamical windows.
  void delete_all_dynamical_windows();
  
  // Adjusts the position/scale of the dynamical/parameter
  // window according to the number of pixels shown and/or the scale.
  void move_dynamical_window(int x, int y, int dynamicalId);
  void scale_dynamical_window(double scale, int x, int y, int dynamicalId);

  const dynamical_window& get_dynamical_window(dynamical_window_id) const;

  void set_dynamical_window(const window2d& window, dynamical_window_id id);

  // True if the given position in pixels lies on the vertical or horizontal axes
  // respectively for the given dynamical_window
  bool on_vertical_axis(double, double, dynamical_window_id id) const;
  bool on_horizontal_axis(double, double, dynamical_window_id id) const;
};
} // namespace gui
} // namespace dynsolver
#endif

