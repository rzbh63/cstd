#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>

#include <fstream>  // NOLINT(readability/streams)
#include <iostream>  // NOLINT(readability/streams)
#include <string>
#include <utility>


#include "caffe/data_transformer.hpp"
#include "caffe/layers/base_data_layer.hpp"
#include "caffe/layers/image_data_layer.hpp"
#include "caffe/util/benchmark.hpp"
#include "caffe/util/io.hpp"

#include "caffe/util/rng.hpp"

namespace
{

  template <typename Dtype>
  ImageDataLayer<Dtype>::~ImageDataLayer()
  {
    this->StopInternalThread();
  }
  void SplitString(const string & str, const string & strSep, std::vector<std::string> & vStrings)
  {
    size_t pos = 0, prepos = 0;
    while (pos <= str.size()) {
      pos = str.find(strSep, prepos);
      if (pos == string::npos) {
        if (prepos <= (int)str.size() - 1) {
          vStrings.push_back(str.substr(prepos, str.size() - prepos));
        }
        break;
      }
      if (pos - prepos > 0) {
        vStrings.push_back(str.substr(prepos, pos - prepos));
      }
      prepos = pos + strSep.size();
    }
  }

  bool is_digital(const string & str)
  {
    int dotnum = 0;
    for (size_t i = 0; i < str.size(); i++) {
      if (str[i] == '.') {
        dotnum++;
        if (dotnum > 1) {
          return false;
        }
      } else if (str[i] < '0' || str[i] > '9') {
        return false;
      }
    }
    return true;
  }

  template <typename Dtype>
  void ImageDataLayer<Dtype>::DataLayerSetUp(const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top)
  {
    const int new_height = this->param_->image_data_param().new_height();
    const int new_width  = this->param_->image_data_param().new_width();
    const bool is_color  = this->param_->image_data_param().is_color();
    const int lebel_size = this->param_->image_data_param().lebel_size();
    const bool is_regression = this->param_->image_data_param().regression();
    string root_folder = this->param_->image_data_param().root_folder();
    CHECK((new_height == 0 && new_width == 0) ||
          (new_height > 0 && new_width > 0)) << "Current implementation requires "
              "new_height and new_width to be set at the same time.";
    // Read the file with filenames and labels
    const string & source = this->param_->image_data_param().source();
    LOG(INFO) << "Opening file " << source;
    std::ifstream infile(source.c_str());
    string line;
    //size_t pos;
    std::string filename;
    if (!is_regression) {
      int label;
      while (std::getline(infile, line)) {
        std::istringstream iss(line);
        iss >> filename;
        vector<int> labels;
        while (iss >> label) {
          labels.push_back(label);
        }
        lines_.push_back(std::make_pair(filename, labels));
      }
      CHECK(!lines_.empty()) << "File is empty";
      if (this->param_->image_data_param().shuffle()) {
        // randomly shuffle data
        LOG(INFO) << "Shuffling data";
        //const unsigned int prefetch_rng_seed = caffe_rng_rand();
        //prefetch_rng_.reset(new Caffe::RNG(prefetch_rng_seed));
        ShuffleImages();
      }
      LOG(INFO) << "A total of " << lines_.size() << " images.";
      lines_id_ = 0;
      // Check if we would need to randomly skip a few data points
      if (this->param_->image_data_param().rand_skip()) {
        unsigned int skip = caffe_rng_rand() %
                            this->param_->image_data_param().rand_skip();
        LOG(INFO) << "Skipping first " << skip << " data points.";
        CHECK_GT(lines_.size(), skip) << "Not enough points to skip";
        lines_id_ = skip;
      }
      // Read an image, and use it to initialize the top blob.
      cv::Mat cv_img = ReadImageToCVMat(root_folder + lines_[lines_id_].first,
                                        new_height, new_width, is_color);
      CHECK(cv_img.data) << "Could not load " << lines_[lines_id_].first;
      // Use data_transformer to infer the expected blob shape from a cv_image.
      vector<int> top_shape = this->data_transformer_->InferBlobShape(cv_img);
      this->transformed_data_.Reshape(top_shape);
      // Reshape prefetch_data and top[0] according to the batch_size.
      const int batch_size = this->param_->image_data_param().batch_size();
      CHECK_GT(batch_size, 0) << "Positive batch size required";
      top_shape[0] = batch_size;
      for (int i = 0; i < this->prefetch_.size(); ++i) {
        this->prefetch_[i]->data[0]_.Reshape(top_shape);
      }
      top[0]->Reshape(top_shape);
      LOG(INFO) << "output data size: " << top[0]->num() << ","
                << top[0]->channels() << "," << top[0]->height() << ","
                << top[0]->width();
      // label
      vector<int> label_shape;
      label_shape.push_back(batch_size);
      label_shape.push_back(lines_[0].second.size());
      label_shape.push_back(1);
      label_shape.push_back(1);
      top[1]->Reshape(label_shape);
      for (int i = 0; i < this->prefetch_.size(); ++i) {
        this->prefetch_[i]->data_[1].Reshape(label_shape);
      }
    } else {
      float value = 0;
      while (std::getline(infile, line)) {
        std::istringstream iss(line);
        iss >> filename;
        vector<float> values;
        while (iss >> value) {
          values.push_back(value);
        }
        regression_lines_.push_back(std::make_pair(filename, values));
      }
      CHECK(!regression_lines_.empty()) << "File is empty";
      if (this->param_->image_data_param().shuffle()) {
        // randomly shuffle data
        LOG(INFO) << "Shuffling data";
        //const unsigned int prefetch_rng_seed = caffe_rng_rand();
        //prefetch_rng_.reset(new Caffe::RNG(prefetch_rng_seed));
        caffe::rng_t* prefetch_rng = caffe_rng();
          //static_cast<caffe::rng_t*>(prefetch_rng_->generator());
        shuffle(regression_lines_.begin(), regression_lines_.end(), prefetch_rng);
      }
      LOG(INFO) << "A total of " << regression_lines_.size() << " images.";
      lines_id_ = 0;
      // Check if we would need to randomly skip a few data points
      if (this->param_->image_data_param().rand_skip()) {
        unsigned int skip = caffe_rng_rand() %
                            this->param_->image_data_param().rand_skip();
        LOG(INFO) << "Skipping first " << skip << " data points.";
        CHECK_GT(regression_lines_.size(), skip) << "Not enough points to skip";
        lines_id_ = skip;
      }
      // Read an image, and use it to initialize the top blob.
      cv::Mat cv_img = ReadImageToCVMat(root_folder + regression_lines_[lines_id_].first,
                                        new_height, new_width, is_color);
      CHECK(cv_img.data) << "Could not load " << regression_lines_[lines_id_].first;
      // Use data_transformer to infer the expected blob shape from a cv_image.
      vector<int> top_shape = this->data_transformer_->InferBlobShape(cv_img);
      this->transformed_data_.Reshape(top_shape);
      // Reshape prefetch_data and top[0] according to the batch_size.
      const int batch_size = this->param_->image_data_param().batch_size();
      CHECK_GT(batch_size, 0) << "Positive batch size required";
      top_shape[0] = batch_size;
      for (int i = 0; i < this->prefetch_.size(); ++i) {
        this->prefetch_[i]->data_[0].Reshape(top_shape);
      }
      top[0]->Reshape(top_shape);
      LOG(INFO) << "output data size: " << top[0]->num() << ","
                << top[0]->channels() << "," << top[0]->height() << ","
                << top[0]->width();
      vector<int> regression_values_shape;
      regression_values_shape.push_back(batch_size);
      regression_values_shape.push_back(regression_lines_[0].second.size());
      regression_values_shape.push_back(1);
      regression_values_shape.push_back(1);
      top[1]->Reshape(regression_values_shape);
      for (int i = 0; i < this->prefetch_.size(); ++i) {
        this->prefetch_[i]->label_.Reshape(regression_values_shape);
      }
    }
  }

  template <typename Dtype>
  void ImageDataLayer<Dtype>::ShuffleImages()
  {
    caffe::rng_t* prefetch_rng = caffe_rng();
      //static_cast<caffe::rng_t*>(prefetch_rng_->generator());
    shuffle(lines_.begin(), lines_.end(), prefetch_rng);
  }

// This function is called on prefetch thread
  template <typename Dtype>
  void ImageDataLayer<Dtype>::load_batch(Batch<Dtype>* batch)
  {
    CPUTimer batch_timer;
    batch_timer.Start();
    double read_time = 0;
    double trans_time = 0;
    CPUTimer timer;
    ASSERT(batch.data_.size()==2);
    CHECK(batch->data_.count());
    CHECK(this->transformed_data_.count());
    ImageDataParameter image_data_param = this->param_->image_data_param();
    const int batch_size = image_data_param.batch_size();
    const int new_height = image_data_param.new_height();
    const int new_width = image_data_param.new_width();
    const bool is_color = image_data_param.is_color();
    string root_folder = image_data_param.root_folder();
    const bool is_regression = image_data_param.regression();
    if (!is_regression) {
      // Reshape according to the first image of each batch
      // on single input batches allows for inputs of varying dimension.
      cv::Mat cv_img = ReadImageToCVMat(root_folder + lines_[lines_id_].first,
                                        new_height, new_width, is_color);
      CHECK(cv_img.data) << "Could not load " << lines_[lines_id_].first;
      // Use data_transformer to infer the expected blob shape from a cv_img.
      vector<int> top_shape = this->data_transformer_->InferBlobShape(cv_img);
      this->transformed_data_.Reshape(top_shape);
      // Reshape batch according to the batch_size.
      top_shape[0] = batch_size;
      batch->data_.Reshape(top_shape);
      Dtype* prefetch_data = batch->data_[0].mutable_data<Context>();
      Dtype* prefetch_label = batch->data_[1].mutable_data<Context>();
      // datum scales
      const int lines_size = lines_.size();
      for (int item_id = 0; item_id < batch_size; ++item_id) {
        // get a blob
        timer.Start();
        CHECK_GT(lines_size, lines_id_);
        cv::Mat cv_img = ReadImageToCVMat(root_folder + lines_[lines_id_].first,
                                          new_height, new_width, is_color);
        CHECK(cv_img.data) << "Could not load " << lines_[lines_id_].first;
        read_time += timer.MicroSeconds();
        timer.Start();
        // Apply transformations (mirror, crop...) to the image
        int offset = batch->data_[0].offset(item_id);
        this->transformed_data_.set_data<Context>(prefetch_data + offset);
        this->data_transformer_->Transform(cv_img, &(this->transformed_data_));
        trans_time += timer.MicroSeconds();
        //prefetch_label[item_id] = lines_[lines_id_].second;
        int label_size = lines_[lines_id_].second.size();
        for (int label_id = 0; label_id < label_size; label_id++) {
          prefetch_label[item_id * label_size + label_id] = lines_[lines_id_].second[label_id];
        }
        // go to the next iter
        lines_id_++;
        if (lines_id_ >= lines_size) {
          // We have reached the end. Restart from the first.
          //DLOG(INFO) << "Restarting data prefetching from start.";
          lines_id_ = 0;
          if (this->param_->image_data_param().shuffle()) {
            ShuffleImages();
          }
        }
      }
    } else {
      // Reshape according to the first image of each batch
      // on single input batches allows for inputs of varying dimension.
      cv::Mat cv_img = ReadImageToCVMat(root_folder + regression_lines_[lines_id_].first,
                                        new_height, new_width, is_color);
      CHECK(cv_img.data) << "Could not load " << regression_lines_[lines_id_].first;
      // Use data_transformer to infer the expected blob shape from a cv_img.
      vector<int> top_shape = this->data_transformer_->InferBlobShape(cv_img);
      this->transformed_data_.Reshape(top_shape);
      // Reshape batch according to the batch_size.
      top_shape[0] = batch_size;
      batch->data_[0].Reshape(top_shape);
      Dtype* prefetch_data = batch->data_[0].mutable_data<Context>();
      Dtype* prefetch_label = batch->data_[1].mutable_data<Context>();
      // datum scales
      const int lines_size = regression_lines_.size();
      for (int item_id = 0; item_id < batch_size; ++item_id) {
        // get a blob
        timer.Start();
        CHECK_GT(lines_size, lines_id_);
        cv::Mat cv_img = ReadImageToCVMat(root_folder + regression_lines_[lines_id_].first,
                                          new_height, new_width, is_color);
        CHECK(cv_img.data) << "Could not load " << regression_lines_[lines_id_].first;
        read_time += timer.MicroSeconds();
        timer.Start();
        // Apply transformations (mirror, crop...) to the image
        int offset = batch->data_[0].offset(item_id);
        this->transformed_data_.set_data<Context>(prefetch_data + offset);
        this->data_transformer_->Transform(cv_img, &(this->transformed_data_));
        trans_time += timer.MicroSeconds();
        int valuenum = regression_lines_[lines_id_].second.size();
        for (int value_id = 0; value_id < valuenum; value_id++) {
          prefetch_label[item_id * valuenum + value_id] = regression_lines_[lines_id_].second[value_id];
        }
        // go to the next iter
        lines_id_++;
        if (lines_id_ >= lines_size) {
          // We have reached the end. Restart from the first.
          //DLOG(INFO) << "Restarting data prefetching from start.";
          lines_id_ = 0;
          if (this->param_->image_data_param().shuffle()) {
            caffe::rng_t* prefetch_rng = caffe_rng();
              //static_cast<caffe::rng_t*>(prefetch_rng_->generator());
            shuffle(regression_lines_.begin(), regression_lines_.end(), prefetch_rng);
          }
        }
      }
    }
    batch_timer.Stop();
    //DLOG(INFO) << "Prefetch batch: " << batch_timer.MilliSeconds() << " ms.";
    //DLOG(INFO) << "     Read time: " << read_time / 1000 << " ms.";
// DLOG(INFO) << "Transform time: " << trans_time / 1000 << " ms.";
  }

  INSTANTIATE_CLASS(ImageDataLayer);
  REGISTER_LAYER_CLASS(ImageData);

}  // namespace
#endif  // USE_OPENCV