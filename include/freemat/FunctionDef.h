
typedef enum {
  FM_M_FUNCTION,
  FM_BUILT_IN_FUNCTION,
  FM_SPECIAL_FUNCTION,
  FM_IMPORTED_FUNCTION,
  FM_MEX_FUNCTION
} FunctionType;

typedef struct interpreter_t interpreter_t;

typedef int (*BuiltInFuncPtr)(int n, const mxArray* arr1, int n1, mxArray* arr2, int n2);
typedef int (*SpecialFuncPtr)(interpreter_t* eval, int n, const mxArray* arr1, int n1, mxArray* arr2, int n2);

/** Base class for the function types
 * A FunctionDef class is a base class for the different types
 * of function pointers used.  There are three types of functions
 * available:
 *    - M-functions - these are functions or scripts written by the
 *      user in the interpreted language.
 *    - Built-in functions - these are functions coded in C++ that
 *      implement functionality too difficult/impossible to do in
 *      the language itself.
 *    - Special functions - these are functions coded in C++ that
 *      require access to the internals of the interpreter object.
 * All of these functions have in common a name, a script classification
 * (is it a script or not), a well defined number of input arguments,
 * a well defined number of output arguments, and some means of
 * being evaluated.
 */

typedef struct fun_t {
  /**
   * The name of the function - must follow identifier rules.
   */
  char name[32];
  /**
   * Returns the "detailed" name of the function -- a full path for example
  */
  char detailed[32];
  /**
   * The reference count for this functiondef
   */
  int refcount;
  /**
   * Is this a script?
   */
  BOOL scriptFlag;
  /**
   * Is this a temporary function?
   */
  BOOL temporaryFlag;
  /**
   * If a function requires access to the graphics subsystem,
   * it must declare itself as such.  Such functions are executed
   * through the main thread.
   */
  //BOOL graphicsFunction;
  /**
   * The list of functions that may be called from JIT-ted code
   * is limited.  Set this flag to TRUE if the function can be called
   * from JIT-ted code.
   */
  BOOL jitsafe;
  /**
   * The names of the arguments to the fuction (analogous to returnVals).
   * Should have "varargin" as the last entry for variable argument
   * functions.
   */
  strv_t arguments[1];
  /**
   * The virtual destructor
   */
  //int FunctionDef();
  /**
   * The type of the function (FM_M_FUNCTION, FM_BUILT_IN_FUNCTION,
   * FM_SPECIAL_FUNCTION, FM_IMPORTED_FUNCTION).
   */
  FunctionType type;
  /**
   * Returns the name of the function
   */
  //virtual QString functionName() = 0;
  /**
   * Print a description of the function
   */
  void (*printMe)(struct fun_t* s, interpreter_t* io);
  /**
   * The number of inputs required by this function (-1 if variable).
   */
  int inputArgCount;
  /**
   * The number of outputs returned by this function (-1 if variable).
  */
  int outputArgCount;
  /**
   * Evaluate the function and return its output.
   */
  //int (*evaluateFunc)(struct fun_t* s, interpreter_t* eval, mxArray* arr, int n, int, VariableTable* Workspace, mxArray* out);
  /**
   * Compile the function (if it is required).  We guarantee that this
   * function will be called at least once before evaluateFunction is called.
   * Return TRUE if the updateCode call did anything.
   */
  //BOOL (*updateCode)(struct fun_t* s, interpreter_t * eval);
  // MFunctionDef
  /**
   * An MFunctionDef is a FunctionDef for an interpreted function.  The
   * function pointer stores the name of the file where the function is
   * located, and a time stamp as to when the function was last modified.
   * When the updateCode() member function is called, the contents of
   * the file are parsed (if necessary), and the resulting AST code TREE
   * is stored.  The number of input and output arguments are computed
   * based on the contents of the returnVals and arguments StringVectors.
   */
  /**
   * The names of the return values - this is a vector of strings with
   * one entry for each return value in the declared function.  Thus,
   * if the function is declared as "function [a,b] = foo(x)", then
   * returnVals contains two entries: "a", and "b".  For variable
   * return functions, the last entry should be "varargout".
   */
  strv_t returnVals[1];
  /**
   * The AST for the code that defines the function (only the body of the
   * function is contained in this AST, not the function declaration itself).
   */
  tree_t code[1];
  /**
   * Flag to indicate if the function has been compiled.
   */
  BOOL functionCompiled;
  /**
   * Location of the function's defining file in the current filesystem.
   */
  char fileName[64];
  /**
   * Time function was last modified.
  */
  time_t timeStamp;
  /**
   * Set to TRUE for all of the localFunctions.
   */
  BOOL localFunction;
  BOOL pcodeFunction;
  BOOL nestedFunction;
  /**
   * The help text.
   */
  strv_t helpText[1];
  /**
   * The variable access list - used as a hinting mechanism to try and
   * resolve scope rules for nested functions.
   */
  strv_t variablesAccessed[1];

} fun_t;

BOOL updateCode(struct fun_t* s, interpreter_t* eval)
{
  return FALSE;
}


#if 0

typedef struct MFunctionDef {
  /** The type of the function
   */
  virtual const FunctionType type() {
    return FM_M_FUNCTION;
  }
  virtual QString functionName() {
    return fileName;
  }
  virtual QString detailedName() {
    return name;
  }
  /** Print a description of the function
   */
  virtual void printMe(interpreter_t* io);
  /**
   * The number of inputs required by this function, which is the number of
   * elements in arguments unless the last element is the keyword "varargin"
   * in which case the answer is -1.
   */
  virtual int inputArgCount();
  /**
   * The number of outputs returned by this function, which is the number of
   * elements in returnVals unless the last element is the keyword "varargout"
   * in which case the answer is -1.
   */
  virtual int outputArgCount();
  /**
   * Evaluate the function and return the outputs.
   * Throws an Exception if
   *   - the special variable 'varargout' is not defined in the body of the
   *     of the function as promised by the function declaration.
   *   - the variable 'varargout' contains too few elements to satisfy the
   *     number of return values in the call
   *   - the variable 'varargout' is the wrong type.
   */
  virtual ArrayVector evaluateFunc(interpreter_t*, ArrayVector&, int, VariableTable*);
  /**
   * Check the timestamp on the file, and if necessary, recompile the
   * function.  Throws an exception if a syntax error occurs in the
   * parsing of the file (i.e., it cannot be classified as either a
   * script or a function definition).
   */
  virtual BOOL updateCode(interpreter_t* m_eval);
  /**
   * Find the line number closest to the requested one
   */
  unsigned ClosestLine(unsigned line);
};


class BuiltInFunctionDef : public FunctionDef
{
public:
  /** The number of return args for this function (-1 for variable).
   */
  int retCount;
  /** The number of input args for this function (-1 for variable).
   */
  int argCount;
  /**
   * The pointer to (address of) the function.
   */
  BuiltInFuncPtr fptr;
  /**
   * Default constructor.
   */
  BuiltInFunctionDef();
  /**
   * Default destructor.
   */
  ~BuiltInFunctionDef();
  /**
   * The type of the function is FM_BUILT_IN_FUNCTION.
   */
  virtual const FunctionType type() {
    return FM_BUILT_IN_FUNCTION;
  }
  virtual QString functionName() {
    return name;
  }
  virtual QString detailedName() {
    return "builtin";
  }
  /** Print a description of the function
   */
  virtual void printMe(interpreter_t* io);
  /**
   * The number of inputs required by this function.
   */
  virtual int inputArgCount();
  /**
   * The number of outputs returned by this function.
   */
  virtual int outputArgCount();
  /**
   * Evaluate the function and return the values.
   */
  virtual ArrayVector evaluateFunc(interpreter_t*, ArrayVector&, int, VariableTable*);
};

class SpecialFunctionDef : public FunctionDef
{
public:
  /**
   * The number of return args for this function (-1 for variable).
   */
  int retCount;
  /** The number of input args for this function (-1 for variable).
   */
  int argCount;
  /**
   * The pointer to (address of) the function.
   */
  SpecialFuncPtr fptr;
  /**
   * Default constructor.
   */
  SpecialFunctionDef();
  /**
   * Default destructor.
   */
  ~SpecialFunctionDef();
  /**
   * The type of the function is FM_SPECIAL_FUNCTION.
   */
  virtual const FunctionType type() {
    return FM_SPECIAL_FUNCTION;
  }
  virtual QString functionName() {
    return name;
  }
  virtual QString detailedName() {
    return "builtin";
  }
  /** Print a description of the function
   */
  virtual void printMe(interpreter_t*);
  /**
   * The number of inputs required by this function.
   */
  virtual int inputArgCount() {
    return argCount;
  }
  /**
   * The number of outputs returned by this function.
   */
  virtual int outputArgCount() {
    return retCount;
  }
  /**
   * Evaluate the function and return the values.
   */
  virtual ArrayVector evaluateFunc(interpreter_t*, ArrayVector& , int, VariableTable*);
};

typedef void (*GenericFuncPointer)();

class ImportedFunctionDef : public FunctionDef
{
public:
  /**
   * The number of return args for this function (either 0 or 1).
   */
  int retCount;
  /**
   * The number of input args for this function (cannot be variable).
   */
  int argCount;
  /**
   * The pointer to the function to be called.
   */
  GenericFuncPointer address;
  /**
   * The call interface object
   */
  ffi_cif cif;
  /**
   * The types of each argument
   */
  StringVector types;
  /**
   * The guard expressions associated with each argument
   */
  TreeList sizeCheckExpressions;
  /**
   * The return type of the function
   */
  QString retType;
  /**
   * Default constructor
   */
  ImportedFunctionDef(GenericFuncPointer address_arg,
      StringVector types_arg,
      StringVector arguments_arg,
      TreeList sizeChecks,
      QString retType_arg,
      QString name);
  /**
   * Default destructor
   */
  ~ImportedFunctionDef();
  /**
   * The type of the function is FM_IMPORTED_FUNCTION.
   */
  virtual const FunctionType type() {
    return FM_IMPORTED_FUNCTION;
  }
  virtual QString functionName() {
    return name;
  }
  virtual QString detailedName() {
    return "imported";
  }
  /** Print a description of the function
   */
  virtual void printMe(interpreter_t*);
  /**
   * The number of inputs required by this function.
   */
  virtual int inputArgCount() {
    return argCount;
  }
  /**
   * The number of outputs returned by this function.
   */
  virtual int outputArgCount() {
    return retCount;
  }
  /**
   * Evaluate the function and return the values.
   */
  virtual ArrayVector evaluateFunc(interpreter_t*, ArrayVector& , int, VariableTable*);
  /**
   * Helper function -- is the ith argument passed as a pointer
   */
  BOOL isPassedAsPointer(int arg);
};

typedef void (*mexFuncPtr)(int, mxArray**, int, const mxArray**);

class MexFunctionDef : public FunctionDef
{
public:
  /**
   * The full name of the library to link to
   */
  QString fullname;
  /**
   * The dynamic library object
   */
  DynLib* lib;
  /**
   * The following flag is set to TRUE if the library is
   * successfully imported
   */
  BOOL importSuccess;
  /**
   * The pointer to the function to be called.
   */
  mexFuncPtr address;
  /**
   * Default constructor
   */
  MexFunctionDef(QString fullpathname);
  /**
   * Default destructor
   */
  ~MexFunctionDef();
  BOOL LoadSuccessful();
  /**
   * The type of the function is FM_MEX_FUNCTION.
   */
  virtual const FunctionType type() {
    return FM_MEX_FUNCTION;
  }
  virtual QString functionName() {
    return name;
  }
  virtual QString detailedName() {
    return "mex";
  }

  /** Print a description of the function
   */
  virtual void printMe(interpreter_t*);
  /**
   * The number of inputs required by this function.
   */
  virtual int inputArgCount() {
    return -1;
  }
  /**
   * The number of outputs returned by this function.
   */
  virtual int outputArgCount() {
    return -1;
  }
  /**
   * Evaluate the function and return the values.
   */
  virtual ArrayVector evaluateFunc(interpreter_t*, ArrayVector& , int, VariableTable*);
};

// This used to be a simple typedef to a pointer of a functiondef
// Now, it is a reference counted class.
class fun_t*
{
private:
  FunctionDef* d;
public:
  fun_t*() : d(NULL) {}
  ~fun_t*() {
    if (d) {
      d->unlock();

      if (!d->referenced()) {
        delete d;
      }
    }
  }
  fun_t*(SpecialFunctionDef* ptr) {
    d = ptr;

    if (d) {
      d->lock();
    }
  }
  fun_t*(BuiltInFunctionDef* ptr) {
    d = ptr;

    if (d) {
      d->lock();
    }
  }
  fun_t*(FunctionDef* ptr) {
    d = ptr;

    if (d) {
      d->lock();
    }
  }
  fun_t*(const fun_t*& copy) {
    d = copy.d;

    if (d) {
      d->lock();
    }
  }
  fun_t*& operator=(const fun_t*& copy) {
    if (copy.d == d) {
      return *this;
    }

    if (d) {
      d->unlock();

      if (!d->referenced()) {
        delete d;
      }
    }

    d = copy.d;

    if (d) {
      d->lock();
    }

    return *this;
  }
  FunctionDef* operator->() const {
    return d;
  }
  FunctionDef& operator*() const {
    return *d;
  }
  BOOL operator!() const {
    return (d == NULL);
  }
  operator FunctionDef* () const {
    return d;
  }
  operator MFunctionDef* () const {
    return ((MFunctionDef*)d);
  }
};

#endif

