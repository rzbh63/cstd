

//#include "log.h"
#include "std/log_c.h"
#include "parser/cJSON.hpp"
#include "cpu.hpp"
#include <map>
#include <iostream>
#include <fstream>
#include "types.h"
#include "math_functions.h"
#include "math_functions.inl"



int debug_info_ = 0;

void Lock() {}
void Unlock() {}
void InitMutex() {}

void set_debug_info(int level) {
  debug_info_ = level;
}

inline string DataShape_string(const DataShape& shape) {
  char buf[256]="[";
  char buf2[32];
  int n = shape.num_axes();
  for (int i = 0; i < n; ++i) {
    if (i) strcat(buf, ",");
    strcat(buf, itoa_c(shape.dim[i], buf2, 10));
  }
  strcat(buf, "(");
  strcat(buf, itoa_c(shape.count(), buf2, 10));
  strcat(buf, ")]");
  return buf;
}
inline DataShape dataShape(int n, int c = 0, int h = 0, int w = 0) {
  DataShape shape;
  shape.set(n, c, h, w);
  return shape;
}

enum MemStage { UNINIT = 0, AT_CPU = 1, AT_GPU = 2, SYNCED = 3 };

struct DevMem {
  Buffer cpu_ptr_[1];
  Buffer gpu_ptr_[1];
  int nbytes_;
  MemStage state_;
  void free() {
    Free(cpu_ptr_);
    Free(gpu_ptr_);
    state_ = UNINIT;
    nbytes_ = 0;
  }
  void init() {
    memset(this, 0, sizeof(*this));
    state_ = UNINIT;
    nbytes_ = 0;
  }
  DevMem() { init(); }
  ~DevMem() { free(); }

  void* to(Brew brew, MemStage state) {
    if (CPU == brew) {
      switch (state_) {
      case UNINIT:
        ReAlloc(brew, cpu_ptr_, nbytes_);
        Memset(cpu_ptr_, nbytes_);
        state_ = AT_CPU;
        break;
      case AT_GPU:
#ifndef CPU_ONLY
        ReAlloc(brew, cpu_ptr_, nbytes_);
        Memcpy(cpu_ptr_, gpu_ptr_, nbytes_);
        state_ = state;
#endif
        break;
      case AT_CPU:
        break;
      case SYNCED:
        state_ = state;
        break;
      }
      return cpu_ptr_->data;
    }
    else if (GPU == brew) {
#ifndef CPU_ONLY
      void* new_ptr_ = nullptr;
      switch (state_) {
      case UNINIT:
        ReAlloc(brew, gpu_ptr_, nbytes_);
        Memset(gpu_ptr_, nbytes_);
        state_ = AT_GPU;
        break;
      case AT_CPU:
        ReAlloc(brew, gpu_ptr_, nbytes_);
        Memcpy(gpu_ptr_, cpu_ptr_, nbytes_);
        state_ = state;
        break;
      case AT_GPU:
        break;
      case SYNCED:
        state_ = state;
        break;
      }
#endif
      return gpu_ptr_->data;
    }
    return NULL;
  }

  void* cpu_ptr() {    return to(CPU, SYNCED);  }
  void* gpu_ptr() {    return to(GPU, SYNCED);  }
  void* cpu_mptr() {    return to(CPU, AT_CPU);  }
  void* gpu_mptr() {    return to(GPU, AT_GPU);  }

  void* ptr(Brew brew) { return to(brew, SYNCED); }
  void* mptr(Brew brew) { return to(brew, brew==CPU ? AT_CPU : AT_GPU); }

  void reset(size_t size) {
    nbytes_ = (int)size;
    state_ = UNINIT;
  }
};

typedef float Dtype;


#define InitString(name, def, type)  name##_ = (def);
#define InitBool(name, def, type)  name##_ = (def);
#define InitFloat(name, def, type)  name##_ = (def);
#define InitInt(name, def, type)  name##_ = (def);
#define InitEnum(name, def, type)  name##_ = (def);
#define InitStruct(name, def, type)  name##_.init();

#define GetString(name, def, type)  name##_ = cjson_GetObjectString(param, #name, def);
#define GetBool(name, def, type)  name##_ = cjson_GetObjectBool(param, #name, def);
#define GetInt(name, def, type)  name##_ = cjson_GetObjectInt(param, #name, def);
#define GetFloat(name, def, type)  name##_ = cjson_GetObjectFloat(param, #name, def);
#define GetEnum(name, def, type)  name##_ = (type)cjson_GetObjectEnum(param, #name, def, type##_Name, countof(type##_Name));
#define GetStruct(name, def, type)  name##_.fromJson(cjson_GetObjectItem(param, #name ));

#if 0
#define SetString(name, def, type)  cjson_SetObjectString(param, #name, name##_);
#define SetBool(name, def, type)    cjson_SetObjectInt(param, #name, name##_);
#define SetInt(name, def, type)     cjson_SetObjectInt(param, #name, name##_);
#define SetFloat(name, def, type)   cjson_SetObjectNumber(param, #name, name##_);
#define SetEnum(name, def, type)    cjson_SetObjectString(param, #name, type##_Name[name##_]);
#define SetStruct(name, def, type)  cjson_SetObjectItem(param, #name, name##_.ToJson() );
#else
#define SetString(name, def, type)  if ((name##_ != def)) cjson_SetObjectString(param, #name, name##_);
#define SetBool(name, def, type)    if (name##_ != def) cjson_SetObjectInt(param, #name, name##_);
#define SetInt(name, def, type)     if (name##_ != def) cjson_SetObjectInt(param, #name, name##_);
#define SetFloat(name, def, type)   if (name##_ != def) cjson_SetObjectNumber(param, #name, name##_);
#define SetEnum(name, def, type)    if (name##_ != def) cjson_SetObjectString(param, #name, type##_Name[name##_]);
#define SetStruct(name, def, type)  cjson_SetObjectItem(param, #name, name##_.ToJson() );
#endif


#define DefString(name, def, type)  string name##_;
#define DefBool(name, def, type)  bool name##_;
#define DefInt(name, def, type)  int name##_;
#define DefFloat(name, def, type)  double name##_;
#define DefEnum(name, def, type)  type name##_;
#define DefStruct(name, def, type)  type name##_;


#define FILLER_PARAM_DEF(DEF) \
DEF##Enum(type, FillerMethod_constant, FillerMethod) \
DEF##Float(value, 0, 0) \
DEF##Float(min, 0, 0) \
DEF##Float(max, 1, 0) \
DEF##Float(mean, 0, 0) \
DEF##Float(std, 1, 0) \
DEF##Float(sparse, -1, 0) \
DEF##Enum(variance_norm, FAN_IN, VarianceNorm)

#define STRUCT_DEF_DEF1(STRUCT_DEF)  \
STRUCT_DEF(Def);  \
 virtual void init() { STRUCT_DEF(Init); } \
 virtual void fromJson(cjson* param) { STRUCT_DEF(Get); } \
 virtual void toJson(cjson* param) {   STRUCT_DEF(Set);   }

struct Filler {
  FILLER_PARAM_DEF(Def);
  cjson* ToJson() { cjson* param = cjson_CreateObject(); FILLER_PARAM_DEF(Set); return param; }
  typedef double Stype;


  void init() {
    FILLER_PARAM_DEF(Init);
  }
  void fromJson(cjson* param) {
    FILLER_PARAM_DEF(Get);
  }
  void toJson(cjson* param) {
    FILLER_PARAM_DEF(Set);
  }



  Filler() {
    init();
  }
  void set_value(double value) { value_ = value; }
  bool set_type(FillerMethod type) {
    type_ = type;
  }
  bool set_type(const char* name) {
    for (int i = 0; i < countof(FillerMethod_Name); ++i) {
      if (0 == strcmp(name, FillerMethod_Name[i])) {
        type_ = (FillerMethod)i;
        return true;
      }
    }
    return false;
  }
  int Fill(DataShape shape, Dtype* data) {
    //cpu_Filler(blob->shape_, blob->cpu_mdata(), param);
    //Blob::Dtype* data = blob->cpu_mdata();
    switch (type_) {
    case FillerMethod_constant:
      CHECK_EQ(sparse_, -1)
        << "Sparsity not supported by this Filler.";
      return cpu_ConstantFiller(shape, data, value_);
    case FillerMethod_gaussian:
      return (cpu_GaussianFiller)(shape, data, mean_, std_, sparse_);

    case FillerMethod_positive_unitball:
      CHECK_EQ(sparse_, -1) << "Sparsity not supported by this Filler.";
      return (cpu_PositiveUnitballFiller)(shape, data);

    case FillerMethod_uniform:
      CHECK_EQ(sparse_, -1) << "Sparsity not supported by this Filler.";
      return (cpu_UniformFiller)(shape, data, min_, max_);

    case FillerMethod_xavier:
      CHECK_EQ(sparse_, -1) << "Sparsity not supported by this Filler.";
      return (cpu_XavierFiller)(shape, data, variance_norm_);

    case FillerMethod_msra:
      CHECK_EQ(sparse_, -1) << "Sparsity not supported by this Filler.";
      return (cpu_MSRAFiller)(shape, data, variance_norm_);

    case FillerMethod_bilinear:
      CHECK_EQ(sparse_, -1)
        << "Sparsity not supported by this Filler.";
      return (cpu_BilinearFiller)(shape, data);

    default:
      CHECK(false) << "Unknown filler name: " << type_;
      break;
    }
    return 0;
  }

};

enum { MAX_NAME = 64 };
struct Blob {
  typedef Dtype Dtype;
  char name_[MAX_NAME];
  Dtype loss_weight_;
  Dtype loss_;
  Dtype lr_mult_;
  Dtype decay_mult_;
  Filler filler_;
#if 0
  union {
    DataShape shape_;
    struct { int n, c, h, w; };
  };
#else
  DataShape shape_;
#endif
  DevMem data_[1];
  DevMem diff_[1];
  bool propagate_down_;
  int top_cnt_;
  int bottom_cnt_;
  int offset(int i) {
    return i*shape_.c*shape_.h*shape_.w;
  }

  int count() const { return shape_.count(); }
  Blob() {
    init();
    this->Reshape(dataShape(1));
  }
  Blob(int n, int c=0, int h=0, int w=0) {
    init();
    this->Reshape(n, c, h, w);
  }
  Blob(const std::vector<int>& shape) {
    init();
    this->Reshape(shape);
  }
  Blob(const DataShape& shape) {
    init();
    this->Reshape(shape);
  }
  void init() {
    memset(this, 0, sizeof(Blob));
    shape_.set(1);
    bottom_cnt_ = top_cnt_ = 0;
    propagate_down_ = true;
    loss_weight_ = 0;
  }
  ~Blob() {
    free();
  }
  void free() {
    data_->free();
    diff_->free();
  }

  const Dtype* data() { return (const Dtype*)(data_)->ptr(BREW); }
  const Dtype* diff() { return (const Dtype*)(diff_)->ptr(BREW); }
  Dtype* mdata() { return (Dtype*)(data_)->mptr(BREW); }
  Dtype* mdiff() { return (Dtype*)(diff_)->mptr(BREW); }

  const Dtype* cpu_data() { return (Dtype*)data_->cpu_ptr(); }
  const Dtype* cpu_diff() { return (Dtype*)diff_->cpu_ptr(); }
  Dtype* cpu_mdata() { return (Dtype*)data_->cpu_mptr(); }
  Dtype* cpu_mdiff() { return (Dtype*)diff_->cpu_mptr(); }

  const Dtype* gpu_data() { return (Dtype*)data_->gpu_ptr(); }
  const Dtype* gpu_diff() { return (Dtype*)diff_->gpu_ptr(); }
  Dtype* gpu_mdata() { return (Dtype*)data_->gpu_mptr(); }
  Dtype* gpu_mdiff() { return (Dtype*)diff_->gpu_mptr(); }

  int Reshape(const DataShape& shape) {
    //if (shape_ != shape)
    {
      int nbytes = shape.count() * sizeof(Dtype);
      data_->reset(nbytes);
      diff_->reset(nbytes);
      shape_ = shape;
    }
    return 0;
  }
  int Reshape(int n, int c, int h, int w) {
    return Reshape(dataShape(n,c,h,w));
  }
  const DataShape& shape() const { return shape_; }
  void ReshapeLike(const Blob & other) {
    if (this != &other) {
      Reshape(other.shape());
    }
  }
  void CopyFrom(Blob* other, bool copy_diff = false, bool reshape = true) {
    if (reshape) { Reshape(other->shape()); }
    caffe_copy(count(), other->data(), mdata());
    if (copy_diff) {
      caffe_copy(count(), other->diff(), mdiff());
    }
  }
  void CopyTo(Blob* other, bool copy_diff) {
    other->Reshape(shape());
    caffe_copy(count(), data(), other->mdata());
    if (copy_diff) {
      caffe_copy(count(), diff(), other->mdiff());
    }
  }
  void zeroData() {
    caffe_set(count(), (Dtype)0, this->mdata());
  }
  void zeroDiff() {
    caffe_set(count(), (Dtype)0, this->mdiff());
  }
  // this->data = other->data
  void ShareData(Blob* other) {
    assert(count()==other->count());
    caffe_copy(count(), other->data(), mdata());
  }
  void ShareDiff(Blob* other) {
    assert(count() == other->count());
    caffe_copy(count(), other->diff(), mdiff());
  }
  int Fill(Filler* filler) {
    Blob* blob = this;
    DataShape shape = blob->shape_;
    Dtype* data = blob->cpu_mdata();
    return filler->Fill(shape, data);
  }

#include "blob.inl"
};

static void ConstantFiller(Blob* blob, double x = 1) {
  cpu_ConstantFiller(blob->shape_, blob->cpu_mdata(), x);
}
static void UniformFiller(Blob* blob, double min = 0, double max = 1) {
  cpu_UniformFiller(blob->shape_, blob->cpu_mdata(), min, max);
}
static void GaussianFiller(Blob* blob, double mu = 0, double st = 1) {
  cpu_GaussianFiller(blob->shape_, blob->cpu_mdata(), mu, st, -1);
}

static int Fill(Blob* blob, Filler* filler) {
  DataShape shape = blob->shape_;
  Dtype* data = blob->cpu_mdata();
  return filler->Fill(shape, data);
}

bool dirExists(string dirStr)
{
  //     const char* dirCStr = dirStr.c_str();
  //     DIR* dir = opendir(dirCStr);
  //     if (ENOENT == errno){
  //       return false;
  //     }
  //     closedir(dir);
  return true;
}

void tryCreateDirectory(string fileName)
{
  //     vector<string> strVec;
  //     boost::split(strVec,fileName,boost::is_any_of("/"));
  //     string newStr="";
  //     for (int i=0;i<strVec.size()-1;++i){
  //       newStr += strVec[i] + (i==strVec.size()-2?"":"/");
  //     }
  //     boost::filesystem::path dirToCreate(newStr);
  //     if (!dirExists(newStr)){
  //       boost::filesystem::create_directories(dirToCreate);
  //     }
}


 static void logBlob(Blob* B, string fileName)
{
  string dataNameStr = fileName + "_data.txt";
  string gradNameStr = fileName + "_grad.txt";
  const char* dataName = (dataNameStr).c_str();
  const char* gradName = (gradNameStr).c_str();
  tryCreateDirectory(dataName);
  tryCreateDirectory(gradName);
  std::ofstream outWriter_data(dataName, std::ofstream::out);
  std::ofstream outWriter_grad(gradName, std::ofstream::out);
  for (int n = 0; n < B->shape(0); ++n) {
    for (int c = 0; c < B->shape(1); ++c) {
      for (int h = 0; h < B->shape(2); ++h) {
        for (int w = 0; w < B->shape(3); ++w) {
          outWriter_data << B->data_at(n, c, h, w) << ",";
          outWriter_grad << B->diff_at(n, c, h, w) << ",";
        }
      }
    }
  }
  outWriter_data << std::endl;
  outWriter_grad << std::endl;
}

static void logBlobs(const vector<Blob*>& blobs, const char* fileName) {
  char blobStr[256];
  for (int i = 0; i < blobs.size(); ++i) {
    _snprintf(blobStr,256, "%s_%d", fileName, i);
    logBlob(blobs[i], blobStr);
  }
}


static void blobs_reset(vector<Blob*>& blobs_, int blob_size) {
  int blob_size_ = blobs_.size();
  for (; blob_size_>blob_size; ) {
    --blob_size_;
    delete blobs_[blob_size_];
    blobs_[blob_size_] = NULL;
  }
  blobs_.resize(blob_size);
  for (; blob_size_<blob_size; ++blob_size_) {
    blobs_[blob_size_] = new Blob();
  }
}

int blobs_count(const vector<Blob*>& blobs_, const char* name) {
  int i, count = 0;
  for (i = 0; i < blobs_.size(); ++i) {
    if (0 == strcmp(name, blobs_[i]->name_)) { ++count; }
  }
  return count;
}

Blob* blobs_get(const vector<Blob*>& blobs_, const char* name) {
  int i;
  for (i = 0; i < blobs_.size(); ++i) {
    if (0 == strcmp(name, blobs_[i]->name_)) { return blobs_[i]; }
  }
  return NULL;
}


Blob* blobs_add(vector<Blob*>& blobs_, const char* name) {
  Blob* new_blob = new Blob();
  strncpy(new_blob->name_, name, MAX_NAME);
  blobs_.push_back(new_blob);
  return new_blob;
}

Blob* blobs_aget(vector<Blob*>& blobs_, const char* name) {
  Blob* bi = NULL;
  if (blobs_count(blobs_, name) == 0) {
    bi = blobs_add(blobs_, name);
  }
  else {
    bi = blobs_get(blobs_, name);
  }
  return bi;
}

#include "layer.inl"

//#define REG_LAYER(name) Layer::fun_type f ## Bias = Layer::reg(new ## Bias, #name );

#define log_blob(blob)    LOG_IF(INFO, root_solver()) << " denseblock blob " << (blob).to_debug_string()
#define INSTANTIATE_CLASS(name, Class) Layer* new ## Class () { Class * p = new Class(); return p;} \
Layer::fun_type f ## Class = Layer::reg(&new ## Class, #name );

//#define INSTANTIATE_CLASS(Bias) INSTANTIATE_CLASS2(Bias, Bias)
#define REGISTER_LAYER_CLASS INSTANTIATE_CLASS
//Layer<double>::fun_type d ## Bias = Layer<double>::reg(&new ## Bias ## Layer<double>, #Bias )
//#define REGISTER_LAYER_CLASS(Bias)

int CreateLayer(Layer*& layer, const char* type) {
  Layer::fun_type fun = Layer::reg(NULL, type);
  CHECK(fun) << "CreateLayer can not find unkown layer type = " << type;
  if (NULL == fun) {
    return 0;
  }
  layer = fun();
  //layer->type_ = (char*)type;
  strcpy(layer->type_, type);
  return 1;
}

#define SolverBase_DEF(DEF) \
DEF##Bool(test_compute_loss, false, 0) \
DEF##Bool(test_initialization, true, 0) \
DEF##Int(test_iter, 1, 0) \
DEF##Int(iter_size, 1, 0) \
DEF##Int(stepsize, 1, 0) \
DEF##Int(average_loss, 1, 0) \
DEF##Int(max_iter, 100, 0) \
DEF##Int(display, 100, 0) \
DEF##Int(snapshot, 100, 0) \
DEF##Int(test_interval, 100, 0) \
DEF##Float(base_lr, 0.01, 0) \
DEF##Float(gamma, 1, 0) \
DEF##Float(power, 1, 0) \
DEF##Float(momentum, 0.999, 0) \
DEF##Float(momentum2, 0.999, 0) \
DEF##Float(delta, 1e-8, 0) \
DEF##Float(weight_decay, 0.0005, 0) \
DEF##Float(clip_gradients, -1, 0) \
DEF##Enum(regularization_type, RegularizationType_L2, RegularizationType) \
DEF##Enum(lr_policy, LrPolicy_inv, LrPolicy) \

struct SolverBase {
  SolverBase_DEF(Def);
  char snapshot_prefix_[256];
  void init() {
    SolverBase_DEF(Init);
  }
  void FromJson(cjson* param) {
    SolverBase_DEF(Get);
    LOG(INFO) << "Solving " << cjson_GetObjectString(param, "name", "");
    debug_info_ = cjson_GetObjectInt(param, "debug_info", 0);
    strncpy(snapshot_prefix_, cjson_GetObjectString(param, "snapshot_prefix", ""), 256);
    //regularization_type_  = param->GetObjectString("regularization_type", "L2");
  }
  cjson* ToJson() {
    cjson* param = cjson_CreateObject();
    SolverBase_DEF(Set);
    return param;
  }
};

struct Net {
  SolverBase solver_param;
  //cJSON* param_;
  vector<Layer* > layers_;
  vector<Blob* > blobs_;
  int size() { return (int)layers_.size(); }
  Net() {
    init();
  }
  void init() {
    //param_ = NULL;
    reset(0);
  }
  void Free() {
    reset(0);
    init();
  }
  void reset(int layer_size) {
    int layer_size_ = (int)layers_.size();
    for (; layer_size_>layer_size; ) {
      --layer_size_;
      delete layers_[layer_size_];
      layers_[layer_size_] = NULL;
    }
    layers_.resize(layer_size);
    for (; layer_size_<layer_size; ++layer_size_) {
     // Layer* layer = layers_[layer_size_] = Layer::reg(NULL);
      //layer->init();
      //layers_[layer_size_].init();
    }
  }
#include "net.inl"
};


#include "solver.hpp"

void ClearParamDiffs(Blob** learnable_params_, int learnable_params_size)
{
  for (int i = 0; i < learnable_params_size; ++i) {
    Blob* blob = learnable_params_[i];
    cpu_caffe_set(blob->count(), (0), blob->cpu_mdiff());
  }
}

#include "layers/layers.hpp"

#undef SetString
#undef SetBool
#undef SetFloat
#undef SetInt
#undef SetEnum
#undef SetStruct

#undef GetString
#undef GetBool
#undef GetFloat
#undef GetInt
#undef GetEnum
#undef GetStruct

#undef DefString
#undef DefBool
#undef DefFloat
#undef DefInt
#undef DefEnum
#undef DefStruct
#undef log_blob


