#include<hitOrMiss/hit_or_miss.hpp>



HitOrMiss::HitOrMiss() {
    image_ = cv::Mat{ kDefaultImageDimension,kDefaultImageDimension,CV_8UC1, cv::Scalar(kWhite) };
    kernel_foreground_ = cv::Mat{ kDefaulKernelForeground,kDefaulKernelForeground, CV_8UC1, cv::Scalar(kBlack) };
    kernel_background_ = cv::Mat{ kDefaulKernelBackground,kDefaulKernelBackground, CV_8UC1, cv::Scalar(kBlack) };
    hit_highlight_ = cv::Mat{ kDefaulHitHighlight,kDefaulHitHighlight, CV_8UC1, cv::Scalar(kBlack) };
}

HitOrMiss::HitOrMiss(cv::Mat image) :HitOrMiss() {
    image_ = TypeCheck(image);
}
HitOrMiss::HitOrMiss(cv::Mat image, cv::Mat kernel_foreground) :HitOrMiss(image) {
    kernel_foreground_ = TypeCheck(kernel_foreground);
}
HitOrMiss::HitOrMiss(cv::Mat image, cv::Mat kernel_foreground, cv::Mat kernel_background)
    :HitOrMiss(image, kernel_foreground) {
    SizeCheck(kernel_foreground_, kernel_background);
    kernel_background_ = TypeCheck(kernel_background);
}
HitOrMiss::HitOrMiss(cv::Mat image, cv::Mat kernel_foreground, cv::Mat kernel_background, cv::Mat hit_highlight)
    :HitOrMiss(image, kernel_foreground, kernel_background)
{
    SizeCheck(kernel_foreground_, kernel_background);
    SizeCheck(kernel_foreground_, hit_highlight);
    hit_highlight_ = TypeCheck(hit_highlight);
}

HitOrMiss::HitOrMiss(const HitOrMiss& rhs) {
    if (this == &rhs) {
        return;
    }
    this->image_ = rhs.get_image();
    this->kernel_foreground_ = rhs.get_kernel_foreground();
    this->kernel_foreground_ = rhs.get_kernel_background();
    this->hit_highlight_ = rhs.get_hit_highlight();
}

HitOrMiss& HitOrMiss::operator=(const HitOrMiss& rhs) {
    if (this == &rhs) {
        return *this;
    }
    image_ = rhs.image_;
    kernel_foreground_ = rhs.kernel_foreground_;
    kernel_background_ = rhs.kernel_background_;
    hit_highlight_ = rhs.hit_highlight_;

    return *this;
}

void HitOrMiss::set_image(cv::Mat lhs) {
    image_ = TypeCheck(lhs);
}
void HitOrMiss::set_kernel_foreground(cv::Mat lhs) {
    kernel_foreground_ = TypeCheck(lhs);
}
void HitOrMiss::set_kernel_background(cv::Mat lhs) {
    SizeCheck(kernel_foreground_, lhs);
    kernel_background_ = TypeCheck(lhs);
}
void HitOrMiss::set_hit_highlight(cv::Mat lhs) {
    hit_highlight_ = TypeCheck(lhs);
}

cv::Mat HitOrMiss::DoHitOrMiss() const {

    SizeCheck(kernel_foreground_, kernel_background_);

    // Проход по изображению структурным элементом переднего плана
    cv::Mat dst_foreground = MaskMatching(true);
    // Проход по изображению структурным элементом заднего плана
    cv::Mat dst_background = MaskMatching(false);

    cv::Mat dst = AndOperation(dst_foreground, dst_background);

    return dst;
}

cv::Mat HitOrMiss::DoBoundaryExtraction() const {

    SizeCheck(kernel_foreground_, kernel_background_);

    cv::Mat hit_or_miss = DoHitOrMiss();
    cv::Mat dst = SubstractionOperation(image_, hit_or_miss);

    return dst;
}

cv::Mat HitOrMiss::MaskMatching(const bool& foreground) const {

    cv::Mat kernel = foreground ? kernel_foreground_ : kernel_background_;

    cv::Mat dst{ image_.rows,image_.cols, CV_8UC1, cv::Scalar(kWhite) };

    /*
    * структурный элемент левым верхним краем совпадает с исходным изображением
    * и проходит по всему изображению не пересекая границ
    */
    for (int mask_row = 0; mask_row <= image_.rows - kernel.rows; mask_row += 1) {
        for (int mask_col = 0; mask_col <= image_.cols - kernel.cols; mask_col += 1) {

            bool hit = true; // пока не нашлось противоречий, считается, что все совпало

            //проверка всех пикселей в радиусе в покрытии структурного элемента
            for (int step_row = 0; step_row < kernel.rows; step_row += 1) {
                for (int step_col = 0; step_col < kernel.cols; step_col += 1) {

                    // текущий пиксель с изображения
                    int image_pixel = image_.at<uchar>(mask_row + step_row, mask_col + step_col);
                    // текущий пиксель с структорного элемента
                    int kernel_pixel = kernel.at<uchar>(step_row, step_col);

                    //если проверяется передний план, то белые пиксели структурного элемента не имеют значения
                    if (foreground && kernel_pixel == kWhite) continue;
                    //если проверяется задний план, то чеерные пиксели структурного элемента не имеют значения
                    if (!foreground && kernel_pixel == kBlack) continue;

                    if (image_pixel != kernel_pixel) {
                        hit = false;
                        break;
                    }
                }
                if (!hit) break;
            }

            //если структурный элемент совпал, то выделить пиксели в соответствии с структурным элементом,
            //отвечающим за выделение
            if (hit) {
                if (hit_highlight_.rows == 1 && hit_highlight_.cols == 1) {
                    dst.at<uchar>(mask_row + kernel.rows / 2, mask_col + kernel.cols / 2) = kBlack;
                }
                else {
                    for (int step_row = 0; step_row < kernel.rows; step_row += 1) {
                        for (int step_col = 0; step_col < kernel.cols; step_col += 1) {

                            if (mask_row + step_row<image_.rows && mask_col + step_col<image_.cols)
                                if (hit_highlight_.at<uchar>(step_row, step_col) == kBlack)
                                dst.at<uchar>(mask_row + step_row, mask_col + step_col) = kBlack;
                        }
                    }
                }
            }
        }
    }
    return dst;
}

cv::Mat HitOrMiss::AndOperation(const cv::Mat& lhs, const cv::Mat& rhs) const {

    SizeCheck(lhs, rhs);

    cv::Mat result{ lhs.rows,lhs.cols, CV_8UC1, cv::Scalar(kWhite) };

    for (int lhs_row = kBlack; lhs_row < lhs.rows; lhs_row += 1) {
        for (int lhs_col = kBlack; lhs_col < lhs.cols; lhs_col += 1) {
            if (lhs.at<uchar>(lhs_row, lhs_col) == kBlack && rhs.at<uchar>(lhs_row, lhs_col) == kBlack) {
                result.at<uchar>(lhs_row, lhs_col) = kBlack;
            }
            else {
                result.at<uchar>(lhs_row, lhs_col) = kWhite;
            }
        }
    }
    return result;
}

cv::Mat HitOrMiss::OrOperation(const cv::Mat& lhs, const cv::Mat& rhs) const {

    SizeCheck(lhs, rhs);

    cv::Mat result{ lhs.rows,lhs.cols, CV_8UC1, cv::Scalar(kWhite) };

    for (int lhs_row = kBlack; lhs_row < lhs.rows; lhs_row += 1) {
        for (int lhs_col = kBlack; lhs_col < lhs.cols; lhs_col += 1) {
            if (lhs.at<uchar>(lhs_row, lhs_col) == kBlack || rhs.at<uchar>(lhs_row, lhs_col) == kBlack) {
                result.at<uchar>(lhs_row, lhs_col) = kBlack;
            }
            else {
                result.at<uchar>(lhs_row, lhs_col) = kWhite;
            }
        }
    }
    return result;
}

cv::Mat HitOrMiss::SubstractionOperation(const cv::Mat& lhs, const cv::Mat& rhs) const {

    SizeCheck(lhs, rhs);

    cv::Mat result{ lhs.rows,lhs.cols, CV_8UC1, cv::Scalar(kWhite) };

    for (int lhs_row = kBlack; lhs_row < lhs.rows; lhs_row += 1) {
        for (int lhs_col = kBlack; lhs_col < lhs.cols; lhs_col += 1) {
            if (lhs.at<uchar>(lhs_row, lhs_col) == kBlack && rhs.at<uchar>(lhs_row, lhs_col) == kBlack) {
                result.at<uchar>(lhs_row, lhs_col) = kWhite;
            }
            else {
                if (lhs.at<uchar>(lhs_row, lhs_col) == kBlack && rhs.at<uchar>(lhs_row, lhs_col) != kBlack) {
                    result.at<uchar>(lhs_row, lhs_col) = kBlack;
                }
                else {
                    result.at<uchar>(lhs_row, lhs_col) = kWhite;
                }
            }

        }
    }
    return result;
}


cv::Mat HitOrMiss::TypeCheck(cv::Mat src) const {
    if (src.empty()) {
        throw std::invalid_argument("The uploaded image was empty");
    }
    else {
        CV_Assert(src.type() == CV_8U && src.channels() == 1);
        cv::Mat dst = src;
        cv::threshold(src, dst, kThresholdValue, kWhite, cv::THRESH_BINARY);
        return dst;
    }
}

void HitOrMiss::SizeCheck(const cv::Mat& origin, const cv::Mat& check) const {
    if (check.cols > 1 || check.rows > 1)
        if (check.rows != origin.rows || check.cols != origin.cols)
            throw std::invalid_argument("The uploaded images have wrong size");
}