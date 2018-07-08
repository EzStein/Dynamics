#ifndef DYNSOLVER_GUI_MODEL_H_
#define DYNSOLVER_GUI_MODEL_H_

#include <vector>
#include <unordered_map>

#include <glad/glad.h>

#include "common.h"
#include "compiler/ast/AST.h"
#include "compiler/function.h"
#include "math/window2d.h"
#include "math/window3d.h"
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

class color {
private:
  double r, g, b, a;
public:
  // Constructs a color from RGBA values given in the range,
  // 0 to 1.
  color(double, double, double, double);

  // Retrieve the r g b and a values.
  double get_red() const;
  double get_green() const;
  double get_blue() const;
  double get_alpha() const;

  void set_red(double);
  void set_green(double);
  void set_blue(double);
  void set_alpha(double);

  // Inverts this color.
  void invert();
};

struct solution_specs {
  solution_specs();

  // The color that should be used to render this solution.
  color glColor;
  
  // The starting point of the solution including the initial value of t.
  math::vector init;

  // Invariant:
  // tMax must be strictly greater than tMin. initial[0] must be
  // strictly between tMax and tMin
  // increment is strictly positive.
  
  // The bounds on how far tMax and tMin should run.
  double tMax;
  double tMin;

  // Currently we use standard euler's method. This indicates the standard
  // increment.
  double inc;
};

// A specification for drawing a solution through an initial point.
struct solution {
  solution_specs specs;
  std::list<math::vector> data;
};

struct singular_point_specs {
  singular_point_specs();
  // The initial point used to compute the position of the singular point
  // in newtons method.
  math::vector init;
  color glColor;
};

struct singular_point {
  singular_point_specs specs;
  math::vector position;
  // std::vector<double> eigenvalues
};

struct isocline_specs {
  isocline_specs();
  // This is the seed used in newtons method to find the isocline.
  math::vector init;

  // This is the direction that the vector field points in on this isocline.
  math::vector direction;

  // This is the maximal increment (in some direction) that is used when
  // computing the isocline.
  double inc;

  // This is the maximum number of vertices used to render the isocline.
  size_t vertices;

  color glColor;
};

struct isocline {
  isocline_specs specs;

  // This is the data representing the isocline. It is ordered so that it
  // may be drawn in a single GL_LINE_STRIP primative.
  std::list<math::vector> data;
};

// Used to pass to and from the dynamical window dialog to obtain
// imformation on the viewport and axes for generating or changing a dynamical
// window.
struct dynamical_specs {
  bool is3d;
  
  unsigned int dynamicalVars;

  // 2d
  int horzAxisVar;
  int vertAxisVar;
  math::window2d viewport2d;

  // 3d
  math::window3d viewport3d;
  int xAxisVar;
  int yAxisVar;
  int zAxisVar;
};

// This class is used to hold all the data behind the GUI system.
// In general, the data can only be accessed using getters and setters.
// This class almost completely decoupled from the view. It is the
// responsibility of the view to update the user interface when it expects that
// a value in the model has changed. The exception is that all opengl work
// occurs in this file.
//
// NOTE: The model may NOT be constructed until an opengl context is set
// current, and the opengl function pointers are loaded.
class model {
 private:
  // Nested Inner classes
  // A tuple which associates an abstract syntax tree with its compiled
  // function.
  struct expression {
    AST ast;
    compiler::function<double, const double*> function;
  };
  
  // Represents information associated with an instance of a dynamical window.
  // Multiple dynamical windows may be open at any given time.
  class dynamical_window {
  private:
    // A reference to the containing model. Since this is an inner class of
    // model, it may access all of its private variables.
    const model& modelData;

    // The specification from which VBO's and other data is generated.
    dynamical_specs specs;

    // The VBO for storing the axes vertex info.
    gl::buffer axesVbo;
    
    GLsizei axesVboVertices;

    // Each solution is associated with render data containing a VBO each for
    // its 2d and 3d rendering.
    struct solution_data {
      // The 2d vbo is stores the projection of a solution onto the plane that
      // is currently being viewed. Each vertex contains one attribute of 2
      // floats. Thus the stride is also 2 floats.
      gl::buffer vbo2d;

      // Contains the projection onto the 3d space currently associated with
      // this window. Each vertex contains one attribute of 3 floats. The
      // stride is 3 floats.
      gl::buffer vbo3d;

      // The number of vertices is the same for both VBO's
      size_t vertices;
    };

    struct isocline_data {
      // This vbo stores the 2d vertex data used to render a given isocline.
      // Each vertex contains one attribute of 2 floats. The stride is thus 2.
      gl::buffer vbo2d;

      // A vbo for the vertex data for 3d rendering of the isocline.
      // Each vertex contains one attribute of 3 floats.
      gl::buffer vbo3d;

      size_t vertices;
    };

    // Maps each solution to its render data.
    std::unordered_map<solution_id, solution_data> solutionData;

    std::unordered_map<isocline_id, isocline_data> isoclineData;

    // The number of ticks between each labeled tick.
    int ticksPerLabel;

    // The font size of the labels on the ticks.
    double tickFontSize;

    // The maximal distance between two axis ticks in pixels. If the number of
    // axis tickets jumps above the maximum, then the number of tickets are
    // doubled, and the real distance between each tick is halved. If it falls
    // below the minimum, the distance between each tick is doubled.
    const int minPixelTickDist;
    const int maxPixelTickDist;

    // The current tick distance in real coordnate values.
    double realTickDistanceX;
    double realTickDistanceY;
    
  public:
    dynamical_window(model&, const dynamical_specs& spec);

    // Update
    void set_specs(const dynamical_specs& spec);

    // Sets the 2d viewport.
    void set_viewport_2d(const math::window2d&);

    // Sets the 3d viewport.
    void set_viewport_3d(const math::window3d&);

    // Renders the window to the currently bound context.
    void paint();

    // Called when the dynamical window is resized. The new
    // width and height is given.
    void resize(int width, int height);

    // Read
    // Returns true if the point given in pixels is on the vertical/horizontal
    // axis.
    bool on_vertical_axis(int x, int y) const;
    bool on_horizontal_axis(int x, int y) const;
    // Gets and sets the specifications.
    const dynamical_specs& get_specs() const;

    // Add
    // Adds a solution to this dynamical frame by adding a new solution vbo.
    void add_solution(solution_id);

    // Adds an isocline to this dynamical frame by adding a new isocline vbo.
    void add_isocline(isocline_id);

    // Delete
    // Deletes the VBO associated with the given solution.
    void delete_solution(solution_id);

    // Deletes the isocline's render data.
    void delete_isocline(isocline_id);

    // Select
    // Attempts to select the solution near the mouse cursor at position,
    // x and y. Returns the selected solution through id. If no solution is
    // found, returns false and does not modify id.
    bool select_solution(int x, int y, solution_id* id);
    bool select_isocline(int x, int y, isocline_id* id);
    bool select_singular_point(int x, int y, singular_point_id* id);

  private:
    // Generates a VBo for the corresponding solution data and id.
    // Replaces the VBO for that ID if it already exists.
    void gen_solution_vbo(solution_id);

    // Generates a VBo for the corresponding isocline data and id.
    // Replaces the VBO for that ID if it already exists.
    void gen_isocline_vbo(isocline_id);
    
    // Updates the axes vbo. This should be called whenever the axes
    // or the ticks on the axes are altered.
    void update_axes_vbo();

    // Draw
    void draw_solutions_3d();
    void draw_isoclines_3d();
    void draw_singular_points_3d();
    void draw_axes_3d();

    void draw_solutions_2d();
    void draw_isoclines_2d();
    void draw_singular_points_2d();
    void draw_axes_2d();
    
    
    typedef std::unordered_map<solution_id, solution_data>::const_iterator
    solution_data_const_iter;

    typedef std::unordered_map<solution_id, isocline_data>::const_iterator
    isocline_data_const_iter;
  };

  // Model Class
  // Static constants used for opengl shaders.
  static const std::string k2dVertexShaderFilePath;
  static const GLuint k2dPositionAttribute;
  static const GLuint k2dVertexBinding;
  static const std::string k2dTransformationUniform;
  
  static const std::string k2dFragmentShaderFilePath;
  static const std::string k2dColorUniform;

  static const std::string kPath3dVertexShaderFilePath;
  static const GLuint kPath3dPositionAttribute;
  static const GLuint kPath3dVertexBinding;
  static const std::string kPath3dTransformationUniform;
  
  static const std::string kPath3dFragmentShaderFilePath;
  static const std::string kPath3dColorUniform;

  // A font and text renderer
  gl::font font;
  gl::text_renderer textRenderer;
  
  // The number of variables involved excludin gthe variable of
  // differentiation, t. Should always be 1 or greater. This number shall be
  // denoted as d, and the number  of dynamical variables will be denoted as n.
  int dynamicalDimension;

  // A list of expressions corresponding to the abstract syntax trees.
  // The size is d.
  std::vector<std::string> expressions;
  
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

  // Each dynamical and parameter window will have an id. The id is associated
  // with the some information in the following maps. The id is also used to
  // lookup the appropriate frames in the app class.
  // Consider using strong typdefs
  std::unordered_map<dynamical_id, dynamical_window> dynamicalWindows;


  // The following variables are updated on each change to parameterPosition
  // and to each change to system or jacobian (on any recompilation).
  
  // The following variables demonstrate what to draw on the dynamical plane.
  // They are specifications for what to draw, and the actually pixel data.
  // Each entry in the following maps is given by a unique id.
  std::unordered_map<solution_id, solution> solutions;
  std::unordered_map<singular_point_id, singular_point> singularPoints;
  std::unordered_map<isocline_id, isocline> isoclines;

  // This value is never mapped. Usually this is set to 0.
  const dynamical_id kNoDynamicalId;
  const solution_id kNoSolutionId;
  const singular_point_id kNoSingularPointId;
  const isocline_id kNoIsoclineId;

  // This value indicates which object is currently selected. If the value is
  // set to one of the kNoId constants above, then no object of that type is
  // selected.
  solution_id selectedSolutionId;
  singular_point_id selectedSingularPointId;
  isocline_id selectedIsoclineId;

  // These unique id's are all greater than or equal to any id's mapped in the
  // corresponding maps. When adding a new value, the unique id is incremented
  // and then the new value is inserted with the new id.
  dynamical_id uniqueDynamicalId;
  solution_id uniqueSolutionId;
  singular_point_id uniqueSingularPointId;
  isocline_id uniqueIsoclineId;
  

  // True if the model is currently representing a compiled system.
  // False if no system is being viewed. If false, the following variables,
  // have no meaning and do not need to satisfy any invariants.
  bool compiled;

  // Opengl variables. There are two programs, the 2d and Path3d.
  gl::vertex_array vao2d;
  gl::program program2d;

  gl::vertex_array vaoPath3d;
  gl::program programPath3d;
  
  const GLuint k2dTransformationUniformLocation;
  const GLuint k2dColorUniformLocation;

  const GLuint kPath3dTransformationUniformLocation;
  const GLuint kPath3dColorUniformLocation;

  // Contains vertex data for rendering a circle of radius 1.
  gl::buffer circleVbo;
  // The number of vertices stored in the circle vbo.
  GLsizei circleVboVertices;

  // Private functions.
  std::vector<gl::shader> build_shaders_2d();
  std::vector<gl::shader> build_shaders_path_3d();

  // (Re)Generates the solution data for the given id using the
  // specification associated with that solution.
  void generate_solution_data(solution_id id);

  typedef std::unordered_map<solution_id, solution>::const_iterator
  solution_const_iter;
  typedef std::unordered_map<dynamical_id, dynamical_window>::const_iterator
  dynamical_const_iter;
  typedef std::unordered_map<singular_point_id, singular_point>::const_iterator
  singular_point_const_iter;
  typedef std::unordered_map<isocline_id, isocline>::const_iterator
  isocline_const_iter;

  typedef std::unordered_map<solution_id, solution>::iterator
  solution_iter;
  typedef std::unordered_map<dynamical_id, dynamical_window>::iterator
  dynamical_iter;
  typedef std::unordered_map<singular_point_id, singular_point>::iterator
  singular_point_iter;
  typedef std::unordered_map<isocline_id, isocline>::iterator
  isocline_iter;
  
public:
  model();

  // Called to clear what is currently compiled.
  void set_no_compile();

  // Tells opengl to paint the given dynamical window according to the state
  // contained in this model. Note the the opengl context must be bound to the
  // appropriate window before calling this method, and the buffers must
  // be swapped after calling this method.
  void paint_dynamical(dynamical_id id);

  // Resizes the dynamical window so that the width and height are given by
  // the new values in pixels.
  void resize_dynamical(dynamical_id id, int width, int height);

  // Resets most variables and compiles the array of strings into a vector
  // field system of ODE's.  Returns true
  // on success. In the future, this function will return information
  // on why the compilation failed if it does fail.
  // Requires that the vector has size 1 or more.
  bool compile(std::vector<std::string>);

  // Add
  // Adds the dynamical window according to the specification provided.
  dynamical_id add_dynamical(const dynamical_specs&);

  // Adds the initial value solution. The solution is added according to the
  // specification provided.
  void add_solution(const solution_specs&);
  
  // Attempts find and add the singular point. If newtons method fails,
  // this method returns false. If the singular point already exists,
  // nothing more is done.
  bool add_singular_point(const singular_point_specs&);

  // Attempts to add the isocline. Returns true on success, and false
  // if the isocline could not be found with the given conditions.
  bool add_isocline(const isocline_specs& specs);


  // Delete
  // Deletes all the data associated with the given window and removes
  // it from the model.
  void delete_dynamical(dynamical_id id);

  // Deletes the solution.
  void delete_solution(solution_id id);

  // Deletes this singular point.
  void delete_singular_point(singular_point_id id);

  // Deletes the isocline.
  void delete_isocline(isocline_id);

  // Select
  // Attempts to select the solution below the cursor at x, y in the dynamical
  // window given by id. On success, returns true. The solution found can be
  // checked throught get_selected_solution_id().
  // This function will work regardless of whether
  // the dynamical plane is viewing in 2 or 3 dimensions.
  bool select_solution(dynamical_id id, int x, int y);

  // Performs the same as select_solution except operates on isoclines and
  // singular points
  bool select_isocline(dynamical_id, int x, int y);
  bool select_singular_point(dynamical_id, int x, int y);

  // Selects the object directly
  void select_solution(solution_id id);
  void select_singular_point(singular_point_id id);
  void select_isocline(isocline_id id);

  // Deselects the solution that is currently selected. Does nothing if no
  // solution is selected.
  void deselect_solution();

  // Does the same as the above functions but operates on isoclines or
  // singular points.
  void deselect_isocline();
  void deselect_singular_point();

  // Update: Some specs need to be dynamically updated. More fine grained
  // control is provided if such functions are common.
  // Sets the dynamical window specification. This will update the
  // necessary VBO's if that is needed.
  void set_dynamical_specs(dynamical_id id, const dynamical_specs& spec);
  
  // Sets the 2d viewport of the dynamical window without altering the rest
  // of the specification.
  void set_dynamical_viewport_2d(dynamical_id id,
				 const math::window2d& window);

  // Sets the 2d viewport of the dynamical window without altering the rest of
  // the specification.
  void set_dynamical_viewport_3d(dynamical_id id,
				 const math::window3d& window);

  // Sets the solution specification. Updates VBO's if necessary.
  void set_solution_specs(solution_id id, const solution_specs& spec);


  // Read
  const dynamical_specs& get_dynamical_specs(dynamical_id id) const;

  const std::unordered_map<solution_id, solution>& get_solutions() const;
  
  const std::unordered_map<singular_point_id, singular_point>&
  get_singular_points() const;
  const std::unordered_map<isocline_id, isocline>& get_isoclines() const;

  // Returns the selected id for the given object or kNoId if no such
  // object is selected.
  solution_id get_selected_solution() const;
  isocline_id get_selected_isocline() const;
  singular_point_id get_selected_singular_point() const;

  // True if the given position in pixels lies on the vertical or horizontal
  // axes respectively for the given dynamical_window
  bool on_dynamical_vertical_axis(dynamical_id id, int x, int y) const;
  bool on_dynamical_horizontal_axis(dynamical_id id, int x, int y) const;

  // Returns the dynamical dimension.
  // The dynamical dimension is one less than the number of dynamical variables.
  int get_dynamical_dimension() const;

  // Returns the number of dynamical variables including the variable of
  // integration t.
  int get_dynamical_vars() const;
};
} // namespace gui
} // namespace dynsolver
#endif

