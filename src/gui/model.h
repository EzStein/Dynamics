#ifndef DYNSOLVER_GUI_MODEL_H_
#define DYNSOLVER_GUI_MODEL_H_

#include <list>
#include <vector>
#include <unordered_map>

#include "compiler/ast/AST.h"
#include "compiler/function.h"
#include "math/vector.h"


namespace dynsolver {
namespace gui {

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
};

/*struct isocline {
  // Specifies the direction that the vector field points on the level set
  // curve. The vector should be nonzero. The magnitude of direction has
  // no effect on the resulting level set. The t variable is excluded.
  math::vector direction;
};

struct eigenvector {
  // The direction of the eigenvector.
  math::vector direction;
  
  // The eigenvalue
  double value;
};

struct singular_point {
  enum class type {
    NODE, SPIRAL, SADDLE
  };
  type type;
  math::vector location;
  
  std::vector<eigenvector> eignvectors;
};*/

// Represents a single solution
// and all of its points ordered by the time component.
class solution_data {
 public:
  // Constructs the solution data from the given points.
  solution_data(const std::vector<math::vector>&);
  
 private:
  // The list of points ordered by the time component.
  std::vector<math::vector> points;
  
  // This is a copy of the above data in a form that openGL
  // can interpret. More documentation to come.
  std::vector<float> gl_raw;
};

// A specification for drawing a solution through an initial point.
class solution {
 public:
  solution(const math::vector& initial, double tMax, double tMin, double increment,
           const solution_data& data);
 private:
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
  
  solution_data data;
};

// Represents information associated with an instance of a dynamical window.
// Multiple dynamical windows may be open at any given time.
class dynamical_window {
 public:
  // Information about the dynamical plane.
  window2d window;
  
  // An integer representing which variable corresponds to which axis.
  // A 0 indicates the variable of differentiation t. 1 indicates x (x1),
  // 2 indicates y (x2), etc..
  int horizontalAxisVariable;
  int verticalAxisVariable;

  dynamical_window(window2d window, int horizontalAxisVariable,
                   int verticalAxisVariable);
};

class parameter_window {
 public:  
  // Information about the dynamical plane.
  window2d window;
  
  // An integer representing the variable that corresponds to each axis.
  // 0 is a, 1 is b (a1), 2 is c (a2) etc...
  int horizontalAxisVariable;
  int verticalAxisVariable;
  
  parameter_window(window2d window, int horizontalAxisVariable,
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
// This class is completely decoupled from the view. It is the responsibility of
// the view to update the user interface when it expects that a value in the
// model has changed.
class model {
 private:
  // True if the model is currently representing a compiled system.
  // False if no system is being viewed. If false, the following variables,
  // have no meaning and do not need to satisfy invariants.
  bool compiled;

  // These are id's which are currently not mapped in the unordered_map
  // of dynamical and parameter windows.
  int uniqueDynamicalId;
  int uniqueParameterId;
  
  // The number of variables involved including the variable of differentiation, t.
  // Should always be 2 or greater. This number shall be denoted as n, and the dimension
  // of the vector field shall be denoted as d = n - 1.
  int dynamicalVariables;
  // One less than the number of dynamical variables.
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
  std::unordered_map<int, dynamical_window> dynamicalWindows;
  std::unordered_map<int, parameter_window> parameterWindows;

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
  std::list<solution> solutions;
  
  // Indicates the current parameter values that are being used to draw on the
  // dynamical plane. This position also shows up on the parameter plane.
  math::vector parameterPosition;

  window2d defaultWindow;
 public:
  model();

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
};
} // namespace gui
} // namespace dynsolver
#endif
