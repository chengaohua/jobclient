
#ifndef SRC_COMMON_TENSOR_H_
#define SRC_COMMON_TENSOR_H_
#include <memory>
#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"

namespace arctern {
enum DataFormat { NHWC = 0, NCHW, NTHWC, NCTHW};

template <int N>
class Shape {
 public:
  Shape() {
    for (int i = 0; i < N; ++i) {
        val[i] = static_cast<int>(0);
    }

    kdims = N;
  }

  explicit Shape(int v0) : kdims(1) { val[0] = v0; }

  Shape(int v0, int v1) : kdims(2) {
    val[0] = v0;
    val[1] = v1;
  }

  Shape(int v0, int v1, int v2) : kdims(3) {
    val[0] = v0;
    val[1] = v1;
    val[2] = v2;
  }

  Shape(int v0, int v1, int v2, int v3) : kdims(4) {
    val[0] = v0;
    val[1] = v1;
    val[2] = v2;
    val[3] = v3;
  }

   Shape(int v0, int v1, int v2, int v3, int v4) : kdims(5) {
        val[0] = v0;
        val[1] = v1;
        val[2] = v2;
        val[3] = v3;
        val[4] = v4;
    }
  explicit Shape(std::vector<int>& shapes) {
    kdims = shapes.size();
    if (shapes.size() > 0) val[0] = shapes[0];
    if (shapes.size() > 1) val[1] = shapes[1];
    if (shapes.size() > 2) val[2] = shapes[2];
    if (shapes.size() > 3) val[3] = shapes[3];
    if (shapes.size() > 4) val[4] = shapes[4];
  }

  int& operator[](int i) { return val[i]; }

  int operator[](int i) const { return val[i]; }

  int num_dims() const { return kdims; }

  bool operator==(const Shape& rhs) {
    if (num_dims() != rhs.num_dims()) return false;

    int i = 0;
    for (; i < num_dims() && (val[i] == rhs.val[i]); ++i) {
    }

    return (i == num_dims());
  }

  uint size() const {
    if (val[0] <= 0) { return 0; }

    uint s = 1;
    for (int i = 0; i < num_dims(); ++i) {
      if (val[i] > 0) {
        s *= val[i];
      } else {
        break;
      }
    }
    return s;
  }

  template<int M>
  friend inline std::ostream &operator<<(std::ostream &os, const Shape<M> &shape);

 protected:
  int val[N];

 private:
  int kdims = 0;  // available dimensions
};

template<int N>
inline std::ostream &operator<<(std::ostream &os, const Shape<N> &shape) {
  os << "shape dims: " << shape.num_dims() << " ( ";
  for (int i = 0; i < shape.num_dims(); ++i) {
    os << shape[i] << " ";
  }
  os << ")";
  return os;
}

typedef Shape<4> TensorShape;

template <typename T>
class Tensor {
 public:
  Tensor() {}

  explicit Tensor(const TensorShape& shape) : shape_(shape) {
    if (size() > 0) {
      data_.reset(new T[size()]());
    }
  }

  virtual ~Tensor() {}

  uint size() const { return shape_.size(); }

  const TensorShape& shape() { return shape_; }

  Tensor& reshape(const TensorShape& shape) {
    uint oldSize = shape_.size();
    uint newSize = shape.size();

    shape_ = shape;

    if (oldSize != newSize) {
      data_.reset(new T[size()]());
    }

    return *this;
  }

  const T* data() const { return &(data_.get()[0]); }

  T* data() { return &(data_.get()[0]); }

  const TensorShape& shape() const { return shape_; }

  void from_cvmat(const cv::Mat& mat, bool bswap_channels = true) {
    if (bswap_channels) {
      shape_ = TensorShape(1, mat.channels(), mat.rows, mat.cols);
      data_.reset(new T[size()]());
      data_format_ = DataFormat::NCHW;
      swap_channels(mat);
    } else {
      shape_ = TensorShape(1, mat.rows, mat.cols, mat.channels());
      data_.reset(new T[size()]());
      data_format_ = DataFormat::NHWC;
      memcpy((uchar*)data_.get(), (uchar*)mat.data, size() * sizeof(T));
    }
  }

  void from_cvmat(const std::vector<cv::Mat>& mats,
          int batchNum, bool bswap_channels = true)
  {
    assert((batchNum > 0) && (mats.size() <= batchNum));

    const int n = batchNum;
    const int c = mats[0].channels();
    const int h = mats[0].rows;
    const int w = mats[0].cols;
    const int nitems = c * h * w;

    if (bswap_channels) {
      shape_ = TensorShape(n, c, h, w);
      data_.reset(new T[size()]());
      data_format_ = DataFormat::NCHW;

      int offset = 0;
      for (size_t i = 0; i < mats.size(); ++i) {
        swap_channels(mats[i], offset);
        offset += nitems;
      }
    } else {
      shape_ = TensorShape(n, h, w, c);
      data_.reset(new T[size()]());
      data_format_ = DataFormat::NHWC;

      T* pdata = data_.get();
      for (size_t i = 0; i < mats.size(); ++i) {
        memcpy((uchar*)pdata, (uchar*)mats[i].data, nitems * sizeof(T));
        pdata += nitems;
      }
    }

    assert(batchNum * c * h * w == (int)this->size());
  }

  void from_vector(const std::vector<T>& vec) {
    data_.reset(new T[vec.size()]());
    memcpy((uchar*)data_.get(), (uchar*)vec.data(), vec.size() * sizeof(T));
  }

 protected:
  void swap_channels(const cv::Mat& image) {
    int width = image.cols;
    int height = image.rows;
    int depth = image.depth();

    T* pdata = data_.get();
    std::vector<cv::Mat> input_channels;

    for (int i = 0; i < image.channels(); ++i) {
      cv::Mat imgchannel(height, width, depth, pdata);
      input_channels.push_back(imgchannel);
      pdata += (height * width);
    }
    cv::split(image, input_channels);
  }


  void swap_channels(const cv::Mat& image, int offset) {
    int width = image.cols;
    int height = image.rows;
    int depth = image.depth();

    T* pdata = data_.get() + offset;
    std::vector<cv::Mat> input_channels;

    for (int i = 0; i < image.channels(); ++i) {
      cv::Mat imgchannel(height, width, depth, pdata);
      input_channels.push_back(imgchannel);
      pdata += (height * width);
    }

    cv::split(image, input_channels);
  }

  TensorShape shape_;
  std::shared_ptr<T> data_;
  DataFormat data_format_ = DataFormat::NCHW;
};

}  // namespace arctern
#endif  // SRC_COMMON_TENSOR_H_
