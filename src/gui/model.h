#ifndef DYNSOLVER_GUI_MODEL_H_
#define DYNSOLVER_GUI_MODEL_H_

#include <vector>
#include <unordered_map>

#include <glad/glad.h>

#include "common.h"
#include "compiler/ast/AST.h"
#include "compiler/function.h"
#include "math/vector.h"


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
// a spanning size of the plane and a centered location.
class window2d {
 private:
  // The size in pixels of window. The x coordinate of size is the width,
  // and the y coordinate of size is the height.
  point2d size;

  // The real spanning distance of this window. The distance spanned by
  // the window in the x and y directions.
  point2d span;

  // This is the position of the top left corner of the window in the plane.
  point2d position;

 public:
  // Constructs a window with the given information.
  window2d(point2d size, point2d span, point2d position);
  
  // The following functions assume that the pixel in the top left corner of
  // the window has pixel coordinate (0, 0). The positive x direction grows
  // to the right and the positive y direction grows moving down.
  // Pixel refers to a point in this coordinate system while real refers
  // to the corrdinate that a pixel is mapped to by this window.
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

  // Scales the window by scale about the real point provided.
  // The resulting window has a the same size as before, but the
  // new x span is scale.x() * span.x()
  // and new y span of scale.y() * span.y().
  // The point real remains fixed in the new window. That is, it is
  // associated with the same pixel as before.
  void scale_real(point2d scale, point2d real);

  // Scales the window by scale about the pixel point provided.
  void scale_pixel(point2d scale, point2d pixel);

  point2d get_size() const;
  point2d get_position() const;
  point2d get_center() const;

  // This transformation matrix is a 4x4 matrix which maps
  // real coordinates to normalized device coordinates for use in opengl.
  math::square_matrix get_transformation_matrix();
};

// A specification for drawing a solution through an initial point.
struct solution {
  solution(const math::vector& initial, double tMax, double tMin, double increment,
	   const std::vector<math::vector>&);
  
  // The starting point of the solution including the initial value of t.
  math::vector initial;

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

  std::vector<math::vector> data;
};

// This class manages a vertex buffer object.
// It is an RAII style class representing a VBO.
// One may think of this object as containing the actuall vertex
// data.
class vbo_manager {
private:
  GLenum storage;
  GLuint vbo;
  size_t size;
  
public:
  // Constructs an unintialized VBO.
  vbo_manager(GLenum storage);

  // Constructs a VBO with the given data.
  vbo_manager(unsigned char* data, size_t size, GLenum storage);

  // Destroys the VBO
  ~vbo_manager();

  // Copy and move constructors
  vbo_manager(const vbo_manager&);
  vbo_manager(vbo_manager&&);

  // Copy and move assignment
  vbo_manager& operator=(const vbo_manager&);
  vbo_manager& operator=(vbo_manager&&);

  // Returns the VBO index.
  // NOTE: this index should be used only to read the
  // VBO. Do not use it to modify its contents.
  GLuint get_vbo() const;

  // Returns the number of bytes stored in the VBO.
  size_t get_size() const;

  // Copies the data pointed to by data into the VBO.
  // The number of bytes copied is given by size.
  // Requires that size is nonzero.
  void set_data(unsigned char* data, size_t size);
};

// Represents information associated with an instance of a dynamical window.
// Multiple dynamical windows may be open at any given time.
class dynamical_window {
 private:
  // This represents the viewport area of the dynamical plane visible
  // in this window.
  window2d window;
  
  // An integer representing which variable corresponds to which axis.
  // A 0 indicates the variable of differentiation t. 1 indicates x (x1),
  // 2 indicates y (x2), etc..
  int horizontalAxisVariable;
  int verticalAxisVariable;

  GLuint vao;
  GLuint program;
  
  struct solution_render_data {
    vbo_manager vbo;
    unsigned int vertices;
  };

  typedef std::unordered_map<solution_id,
			     solution_render_data>::const_iterator solution_iter;
  
  // This map associates a solution to the opengl render data.
  std::unordered_map<solution_id, solution_render_data> solutionRenderData;
  
public:
  dynamical_window(const window2d& window, int horizontalAxisVariable,
                   int verticalAxisVariable, GLuint vao, GLuint program,
		   const std::unordered_map<solution_id, solution>&);

  // Getters and setters
  void set_horizontal_axis_variable(int);
  int get_horizontal_axis_variable() const;
  void set_vertical_axis_variable(int);
  int get_vertical_axis_variable() const;
  void set_window(const window2d&);

  // Renders the window to the currently bound context.
  void paint();
  
  void resize();

  // Generates a VBo for the corresponding solution data and id.
  void generate_vbo(solution_id, const std::vector<math::vector>&);

  const window2d& get_window() const;
};

class parameter_window {
 public:  
  // Information about the dynamical plane.
  window2d window;
  
  // An integer representing the variable that corresponds to each axis.
  // 0 is a, 1 is b (a1), 2 is c (a2) etc...
  int horizontalAxisVariable;
  int verticalAxisVariable;
  
  parameter_window(const window2d& window, int horizontalAxisVariable,
                   int verticalAxisVariable);
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
class model {
 private:
  // The number of variables involved including the variable of differentiation, t.
  // Should always be 2 or greater. This number shall be denoted as n, and the dimension
  // of the vector field shall be denoted as d = n - 1.
  // We only store the dimension of the vector field.
  int dynamicalDimension;

  // The number of parameters used. Should be nonnegative.
  // This number shall be denoted as p.
  int parameterVariables;

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

  std::unordered_map<parameter_window_id, parameter_window> parameterWindows;
  
  // This is an id number which is not currently mapped in parameterWindows map.
  parameter_window_id uniqueParameterId;

  
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
  
  // Indicates the current parameter values that are being used to draw on the
  // dynamical plane. This position also shows up on the parameter plane.
  math::vector parameterPosition;

  window2d defaultWindow;

  // True if the model is currently representing a compiled system.
  // False if no system is being viewed. If false, the following variables,
  // have no meaning and do not need to satisfy any invariants.
  bool compiled;

  // True once opengl is initialized. Most of the above functions require
  // that opengl be initialized in order to function correctly.
  bool glInitialized;

  // These two variables represent the opengl program and VAO.
  GLuint vao;
  GLuint program;
  
 public:
  model();
  
  ~model();

  void paint_dynamical_window(dynamical_window_id id);
  void resize_dynamical_window(dynamical_window_id id);

  // Called exactly once when opengl is initialized.
  // Within this method, the context provided is set current.
  void initialize_opengl();
  
  // The dynamical dimension is one less than the number of dynamical variables.
  int get_dynamical_dimension() const;

  // Returns the number of dynamical variables including the variable of
  // integration t.
  int get_dynamical_variables() const;

  // Sets the number of parameters used.
  // Requires that the provided integer is non-negative.
  void set_parameters(int);
  
  // Adds the initial value solution. Init, is the initial point
  // whose first entry is the time component, second entry is the first
  // variable etc... Requires the size of init to be equal to n. Requires,
  // that init[0] is strictly between tMin and tMax and that tMin
  // is strictly less than tMax. Requires that increment is positive.
  void add_initial_value_solution(const math::vector& init,
                                  double tMin, double tMax,
                                  double increment);

  // Like set_parameter_position except also updates the
  // defaultParameterPosition variable.
  void set_parameter_position(math::vector);

  // Resets most variables and compiles the array of strings into a vector
  // field system of ODE's. Sets the dimension etc... Returns true
  // on success.
  // Requires that the vector has size 1 or more.
  bool compile(std::vector<std::string>);
  
  // The integers refer to variables
  // 0 is t, 1 is x (x1), 2 is y (x2), etc...
  // Sets the dynamical plane's horizontal and vertical axes.
  void set_dynamical_axes(int horizontalVariable, int verticalVariable, int dynamicalId);
  
  // 0 is a (a1), 1 is b (a2), 2 is c (a3), etc...
  // Sets the parameter plane's horizontal and vertical axes.
  void set_parameter_axes(int horizontalVariable, int verticalVariable, int parameterId);

  // These functions add dynamical and parameter windows. They return the id,
  // of the newly added window. Requires that the axis variables fall
  // within the appropriate range.
  int add_dynamical_window(window2d window, int horizontalAxisVariable,
			   int verticalAxisVariable);
  int add_parameter_window(window2d window, int horizontalAxisVariable,
                                  int verticalAxisVariable);
  
  // Adjusts the position/scale of the dynamical/parameter
  // window according to the number of pixels shown and/or the scale.
  void move_dynamical_window(int x, int y, int dynamicalId);
  void scale_dynamical_window(double scale, int x, int y, int dynamicalId);

  void move_parameter_window(int x, int y, int parameterId);
  void scale_parameter_window(double scale, int x, int y, int parameterId);

  const dynamical_window& get_dynamical_window(dynamical_window_id) const;
};
} // namespace gui
} // namespace dynsolver
#endif

