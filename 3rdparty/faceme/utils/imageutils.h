#include "opencv2/opencv.hpp"
bool ToFrImage(FR_Image& result, const cv::Mat& from)
{
    FR_INIT_STRUCT(&result, FR_Image);
    std::memset((uint8_t *)&result, 0, sizeof(FaceMeSDK::FR_Image));
    (result).sizeOfStructure = sizeof(FaceMeSDK::FR_Image);

    result.channel = from.channels();
    result.data = from.data;
    result.height = from.rows;
    result.pixelFormat = FR_PIXEL_FORMAT_BGR;
    result.stride = (uint32_t)from.step;
    result.width = from.cols;

    switch (from.type())
    {
    case CV_8UC3:
        result.pixelFormat = FaceMeSDK::FR_PIXEL_FORMAT_BGR;
        break;
    case CV_8UC4:
        result.pixelFormat = FaceMeSDK::FR_PIXEL_FORMAT_BGRA;
        break;
    default:
        result.pixelFormat = FaceMeSDK::FR_PIXEL_FORMAT_UNKNOWN;
        return false;
    }

    return true;
}

cv::Rect EnlargeRectByValue(cv::Rect& src_roi, int image_width, int image_height, const int left_space, const int top_space, const int right_space, const int bottom_space, bool is_shift)
{
    cv::Rect dst_roi;
    // for horizontal enlarge
    dst_roi.x = ch_Max(0, src_roi.x - left_space);
    if (is_shift)
    {
        dst_roi.width = src_roi.width + left_space + right_space;
        if ((dst_roi.x + dst_roi.width) > image_width)
        {
            dst_roi.x = image_width - dst_roi.width;
            if (dst_roi.x < 0)
            {
                dst_roi.x = 0;
                dst_roi.width = image_width;
            }
        }
    }
    else
    {
        int right = std::min<int>(image_width, src_roi.x + src_roi.width + right_space);
        dst_roi.width = right - dst_roi.x;
    }

    // for vertical enlarge
    dst_roi.y = std::max<int>(0, src_roi.y - top_space);
    if (is_shift)
    {
        dst_roi.height = src_roi.height + top_space + bottom_space;
        if ((dst_roi.y + dst_roi.height) > image_height)
        {
            dst_roi.y = image_height - dst_roi.height;
            if (dst_roi.y < 0)
            {
                dst_roi.y = 0;
                dst_roi.height = image_height;
            }
        }
    }
    else
    {
        int bottom = std::min<int>(image_height, src_roi.y + src_roi.height + bottom_space);
        dst_roi.height = bottom - dst_roi.y;
    }
    return dst_roi;
}

cv::Rect EnlargeRectByRatio(cv::Rect& src_roi, int image_width, int image_height, const float enlarge_ratio, bool is_shift)
{
    int h_enlarge_pixel = (int)(src_roi.width * enlarge_ratio);
    int v_enlarge_pixel = (int)(src_roi.height * enlarge_ratio);
    return EnlargeRectByValue(src_roi, image_width, image_height, h_enlarge_pixel, v_enlarge_pixel, h_enlarge_pixel, v_enlarge_pixel, is_shift);
}

FR_RETURN CropROIToImage(const FR_Image& image, const FR_Rectangle& roi, cv::Mat& dst_mat)
{
    cv::Rect detect_face_rect(roi.topLeft.x, roi.topLeft.y, roi.bottomRight.x - roi.topLeft.x, roi.bottomRight.y - roi.topLeft.y);

    // For each trained face, save the color frame (640 x 480) to the database.
    if (detect_face_rect.width > detect_face_rect.height)
    {
        int space = (detect_face_rect.width - detect_face_rect.height) / 2;
        detect_face_rect = EnlargeRectByValue(detect_face_rect, image.width, image.height, 0, space, 0, space, false);
    }
    else if (detect_face_rect.width < detect_face_rect.height)
    {
        int space = -(detect_face_rect.width - detect_face_rect.height) / 2;
        detect_face_rect = EnlargeRectByValue(detect_face_rect, image.width, image.height, space, 0, space, 0, false);
    }

    cv::Rect enlarge_rect = EnlargeRectByRatio(detect_face_rect, image.width, image.height, 0.25, false);
    int max_thumb_dim = 96;
    int scale_width = max_thumb_dim;
    int scale_height = max_thumb_dim;
    if (enlarge_rect.height >= enlarge_rect.width)
    {
        scale_width = std::max<int>(1, enlarge_rect.width * max_thumb_dim / enlarge_rect.height);
        scale_height = std::max<int>(1, max_thumb_dim);
    }
    else if (enlarge_rect.width > enlarge_rect.height)
    {
        scale_height = std::max<int>(1, enlarge_rect.height * max_thumb_dim / enlarge_rect.width);
        scale_width = std::max<int>(1, max_thumb_dim);
    }

    double x_factor = double(scale_width) / enlarge_rect.width;
    double y_factor = double(scale_height) / enlarge_rect.height;

    // Crop and scale face image by opencv
    cv::Mat source = cv::Mat(image.height, image.width, CV_8UC3, image.data);
    cv::Mat cropped_mat_tmp(source, enlarge_rect);
    cv::Mat cropped_mat;
    // Copy the data into new matrix
    cropped_mat_tmp.copyTo(cropped_mat);
    dst_mat = cv::Mat(enlarge_rect.height, enlarge_rect.width, CV_8UC3, cv::Scalar(0, 0, 0));
    dst_mat = cropped_mat;
    cv::resize(cropped_mat, dst_mat, cv::Size(0, 0), x_factor, y_factor);
    return FR_RETURN_OK;
}

#define TARGET_WIDTH_HEIGHT_L 1920
#define TARGET_WIDTH_HEIGHT 1280
#define TARGET_WIDTH_HEIGHT_S 640

void ResizeImages(std::vector<cv::Mat>& images_mat)
{
    auto image_count = images_mat.size();

    int maxWidth = 0, maxHeight = 0, minWidth = 99999, minHeight = 99999;
    for (int i = 0; i < image_count; i++) {
        if (images_mat[i].data == nullptr) continue;
        if (maxWidth < images_mat[i].cols)
            maxWidth = images_mat[i].cols;
        if (maxHeight < images_mat[i].rows)
            maxHeight = images_mat[i].rows;
        if (minWidth > images_mat[i].cols)
            minWidth = images_mat[i].cols;
        if (minHeight > images_mat[i].rows)
            minHeight = images_mat[i].rows;
    }

    int targetH = TARGET_WIDTH_HEIGHT;
    int targetW = TARGET_WIDTH_HEIGHT;
    if (maxHeight <= TARGET_WIDTH_HEIGHT_S) {
        targetH = TARGET_WIDTH_HEIGHT_S;
    }
    else if (maxHeight <= TARGET_WIDTH_HEIGHT) {
        targetH = TARGET_WIDTH_HEIGHT;
    }
    else {
        targetH = TARGET_WIDTH_HEIGHT_L;
    }

    if (maxWidth <= TARGET_WIDTH_HEIGHT_S) {
        targetW = TARGET_WIDTH_HEIGHT_S;
    }
    else if (maxWidth <= TARGET_WIDTH_HEIGHT) {
        targetW = TARGET_WIDTH_HEIGHT;
    }
    else {
        targetW = TARGET_WIDTH_HEIGHT_L;
    }

    std::vector<cv::Mat> resizeImage(image_count);
    for (int i = 0; i < image_count; i++)
    {
        //if (images_mat[i].data == nullptr) continue;

        resizeImage[i] = cv::Mat(targetH, targetW, CV_8UC3, cv::Scalar(0, 0, 0));
        cv::Mat imageROI;
        if (images_mat[i].rows >= targetH || images_mat[i].cols >= targetW)
        {
            int newWidth;
            int newHeight;
            float ImageAspectRatio = (float)images_mat[i].cols / images_mat[i].rows;
            float fixAspectRatio = (float)targetW / targetH;
            if (fixAspectRatio >= ImageAspectRatio)
            {
                newHeight = targetH;
                newWidth = int(images_mat[i].cols * (float)newHeight / images_mat[i].rows);
            }
            else
            {
                newWidth = targetW;
                newHeight = int(images_mat[i].rows * (float)newWidth / images_mat[i].cols);
            }
            cv::Mat dstMat;
            cv::resize(images_mat[i], dstMat, cv::Size(newWidth, newHeight), cv::INTER_LANCZOS4);
            imageROI = resizeImage[i](cv::Rect(0, 0, dstMat.cols, dstMat.rows));
            dstMat.copyTo(imageROI);
        }
        else
        {
            imageROI = resizeImage[i](cv::Rect(0, 0, images_mat[i].cols, images_mat[i].rows));
            images_mat[i].copyTo(imageROI);
        }

        images_mat[i] = resizeImage[i];
    }
}

void ResizeImage(cv::Mat& image_mat)
{
    std::vector<cv::Mat> images_mat(1);
    images_mat[0] = image_mat;
    ResizeImages(images_mat);
    image_mat = images_mat[0];
}