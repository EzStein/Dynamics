
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
};

struct solution_specification {
  solution_specification();

  // The color that should be used to render this solution.
  color glColor;
  
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
  bool is3d;
  
  unsigned int dynamicalVariables;

  // 2d
  int horizontalAxisVariable;
  int verticalAxisVariable;
  math::window2d viewport2d;

  // 3d
  math::window3d viewport3d;
  int xAxisVariable;
  int yAxisVariable;
  int zAxisVariable;
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
    model& modelData;

    // The specification from which VBO's and other data is generated.
    dynamical_window_specification specification;

    // The VBO for storing the axes vertex info.
    gl::buffer axesVbo;
    GLsizei axesVboVertices;

    // Each solution is associated with render data containing a VBO each for
    // its 2d and 3d rendering.
    struct solution_render_data {
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

    // Maps each solution to its render data.
    std::unordered_map<solution_id, solution_render_data> solutionRenderData;

    // The number of ticks between each labeled tick.
    int ticksPerLabel;

    // The font size of the labels on the ticks.
    double tickFontSize;

    // The maximal distance between two axis ticks in pixels. If the number of
    // axis tickets jumps above the maximum, then the number of tickets are
    // doubled, and the real distance between each tick is halved. If it falls
    // below the minimum, the distance between each tick is doubled.
    const int minimumPixelTickDistance;
    const int maximumPixelTickDistance;

    // The current tick distance in real coordnate values.
    double realTickDistanceX;
    double realTickDistanceY;
    
  public:
    dynamical_window(model&, const dynamical_window_specification& spec);

    // Gets and sets the specifications.
    const dynamical_window_specification& get_specification() const;
    void set_specification(const dynamical_window_specification& spec);

    // Sets the 2d viewport.
    void set_viewport_2d(const math::window2d&);

    // Sets the 3d viewport.
    void set_viewport_3d(const math::window3d&);

    // Performs the same thing as model::select_solution except,
    bool select_solution(int x, int y, solution_id* id);

    // Renders the window to the currently bound context.
    void paint();

    // Called when the dynamical window is resized. The new
    // width and height is given.
    void resize(int width, int height);

    // Returns true if the point given in pixels is on the vertical/horizontal
    // axis.
    bool on_vertical_axis(int x, int y) const;
    bool on_horizontal_axis(int x, int y) const;

    // Deletes the VBO associated with the given solution.
    void delete_solution(solution_id);
    void add_solution(solution_id);
    
  private:
    // Generates a VBo for the corresponding solution data and id.
    // Replaces the VBO for that ID if it already exists.
    void generate_vbo(solution_id);
    
    // Updates the axes vbo. This should be called whenever the axes
    // or the ticks on the axes are altered.
    void update_axes_vbo();
    
    typedef std::unordered_map<solution_id, solution_render_data>::
    const_iterator solution_render_data_const_iter;
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

  // Private functions.
  std::vector<gl::shader> build_shaders_2d();
  std::vector<gl::shader> build_shaders_path_3d();

  // (Re)Generates the solution data for the given id using the
  // specification associated with that solution.
  void generate_solution_data(solution_id id);

  typedef std::unordered_map<solution_id, solution>::const_iterator
  solution_const_iter;
public:
  model();

  // Tells opengl to paint the given dynamical window according to the state
  // contained in this model. Note the the opengl context must be bound to the
  // appropriate window before calling this method, and the buffers must
  // be swapped after calling this method.
  void paint_dynamical_window(dynamical_window_id id);

  // Resizes the dynamical window so that the width and height are given by
  // the new values in pixels.
  void resize_dynamical_window(dynamical_window_id id, int width, int height);

  // Returns the dynamical dimension.
  // The dynamical dimension is one less than the number of dynamical variables.
  int get_dynamical_dimension() const;

  // Returns the number of dynamical variables including the variable of
  // integration t.
  int get_dynamical_variables() const;

  // Resets most variables and compiles the array of strings into a vector
  // field system of ODE's.  Returns true
  // on success. In the future, this function will return information
  // on why the compilation failed if it does fail.
  // Requires that the vector has size 1 or more.
  bool compile(std::vector<std::string>);

  // Adds the dynamical window according to the specification provided.
  dynamical_window_id add_dynamical_window
  (const dynamical_window_specification&);

  // Sets the dynamical window specification. This will update the
  // necessary VBO's if that is needed.
  void set_dynamical_window_specification
  (dynamical_window_id id, const dynamical_window_specification& spec);
  const dynamical_window_specification&
  get_dynamical_window_specification(dynamical_window_id id) const;

  // Deletes all the data associated with the given window and removes
  // it from the model.
  void delete_dynamical_window(dynamical_window_id id);

  // Sets the 2d viewport of the dynamical window without altering the rest
  // of the specification.
  void set_dynamical_viewport_2d(dynamical_window_id id,
					const math::window2d& window);

  // Sets the 2d viewport of the dynamical window without altering the rest of
  // the specification.
  void set_dynamical_viewport_3d(dynamical_window_id id,
					const math::window3d& window);

  // Sets the color of the solution provided without altering the rest of
  // the specification.
  void set_solution_color(solution_id id, const color& color);

  // Sets the solution specification. Updates VBO's if necessary.
  void set_solution_specification(solution_id id,
				  const solution_specification& spec);

  // Deletes the solution.
  void delete_solution(solution_id id);

  // Adds the initial value solution. The solution is added according to the
  // specification provided.
  void add_solution(const solution_specification&);

  const std::unordered_map<solution_id, solution>& get_solutions() const;

  // Attempts to select the solution below the cursor at x, y in the dynamical
  // window given by id. On success, returns true and stores the found solution
  // in the reference provided.
  bool select_solution(dynamical_window_id id, int x, int y, solution_id*);

  // True if the given position in pixels lies on the vertical or horizontal
  // axes respectively for the given dynamical_window
  bool on_dynamical_vertical_axis(dynamical_window_id id, int x, int y) const;
  bool on_dynamical_horizontal_axis(dynamical_window_id id, int x, int y) const;
};
} // namespace gui
} // namespace dynsolver
#endif

