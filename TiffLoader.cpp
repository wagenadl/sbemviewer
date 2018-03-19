// TiffLoader.cpp

#include "TiffLoader.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

static void matdeleter(void *data) {
  cv::Mat *mat = (cv::Mat *)data;
  delete mat;
}

QImage TiffLoader::load(QString fn) {
  cv::Mat *mat = new cv::Mat;
  *mat = cv::imread(fn.toUtf8().data(), cv::IMREAD_GRAYSCALE);
  if (!mat->data || mat->depth()!=CV_8U || mat->channels()!=1) {
    delete mat;
    return QImage();
  }
  return QImage(mat->data, mat->cols, mat->rows,
                QImage::Format_Grayscale8, &matdeleter, (void*)mat);
}

QImage TiffLoader::read(QByteArray data) {
  int len = data.size();
  cv::Mat src(1, &len, CV_8UC1, data.data());
  cv::Mat *mat = new cv::Mat;
  *mat = cv::imdecode(src, cv::IMREAD_GRAYSCALE);
  if (!mat->data || mat->depth()!=CV_8U || mat->channels()!=1) {
    delete mat;
    return QImage();
  }
  return QImage(mat->data, mat->cols, mat->rows,
                QImage::Format_Grayscale8, &matdeleter, (void*)mat);
}

