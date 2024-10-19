/**
* @file hit_or_miss.hpp
* @brief Header-only library hitOrMiss
* 
* Provided an interface for working with the Hit or miss algorithm and 
* the Boundary Extraction (implemented on the Hit or Miss) on binary images 
*
* @author Kiselev K.A.
* @date 10.06.2023
*/

#pragma once
#ifndef HITORMISS_HITORMISS_HPP_20230706 
#define HITORMISS_HITORMISS_HPP_20230706


#include <stdio.h>
#include <opencv2/opencv.hpp>
#include<iosfwd>

/**
* @brief Функция этого класса: создать изображение обработанное методом Hit or Miss
* 
* Единственный класс библиотеки, содержит параметры для работы алгоритма
* а также функции для их проверки и обработки. 
* Все передаваемые изображения должны быть в оттенках серого и иметь тип CV_8UC1
*/
class HitOrMiss {
public:
    /**
    * @brief Конструктор по умолчанию
    * 
    * изображение для обработки: белая картинка 200*200 (kDefaultImageDimension)
    * 
    * структурный элемент переднего плана: черная картинка 3*3 (kDefaulKernel)
    * 
    * структурный элемент заднего плана: черная картинка 3*3 (kDefaulKernel)
    * 
    * структурный элемент, отвечающий за выделение при попадании: 
    * черная картинка 1*1 (kDefaulHitHighlight)
    */
    HitOrMiss();

    /**
   * @brief Конструктор
   *
   * Конструктор от изображения для обработки
   * 
   * Остальные параметры установлены по умолчанию
   * @param[in] image изображение для обработки
   * @throw invalid_argument если тип изображения, не соответстует описанию
   */
    explicit HitOrMiss(cv::Mat image);
    
    /**
    * @brief Конструктор 
    * 
    * Конструктор от изображения для обработки, структурного элемента
    * для переднего плана
    * 
    * Остальные параметры установлены по умолчанию
    * @param[in] image изображение для обработки
    * @param[in] kernel_foreground структурный элемент переднего плана
    * @throw invalid_argument если тип изображения, не соответстует описанию
    */
    HitOrMiss(cv::Mat image, cv::Mat kernel_foreground);

    /**
    * @brief Конструктор
    *
    * Конструктор от изображения для обработки, структурного элемента
    * для переднего плана, структурного элемента для заднего плана
    * 
    * Остальные параметры установлены по умолчанию
    * @param[in] image изображение для обработки
    * @param[in] kernel_foreground структурный элемент переднего плана
    * @param[in] kernel_background структурный элемент заднего плана
    * @throw invalid_argument если тип изображения, не соответстует описанию
    */
    HitOrMiss(cv::Mat image, cv::Mat kernel_foreground, cv::Mat kernel_background);

    /**
    * @brief Конструктор
    *
    * Конструктор от изображения для обработки, структурного элемента
    * для переднего плана, структурного элемента для заднего плана,
    * структурного элемента, отвечающего за выделение при попаднии
    *
    * Остальные параметры установлены по умолчанию
    * @param[in] image изображение для обработки
    * @param[in] kernel_foreground структурный элемент переднего плана
    * @param[in] kernel_background структурный элемент заднего плана
    * @param[in] hit_highlight структурный элемент, структурный элемент,
    * отвечающий за выделение при попадании
    * @throw invalid_argument если тип изображения, не соответстует описанию
    */
    HitOrMiss(cv::Mat image, cv::Mat kernel_foreground, 
        cv::Mat kernel_background, cv::Mat hit_highlight);

    /**
    * @brief Конструктор копирования 
    * 
    * param[in] Объект типа HitOrMiss
    */
    HitOrMiss(const HitOrMiss& rhs);
    
    /**
    * @brief Деструктор по умолчанию
    */
    ~HitOrMiss() = default;

    /**
    * @brief Оператор присваивания
    */
    HitOrMiss& operator=(const HitOrMiss& rhs);

public:
    /**
    * @brief setter: изображения для обработки
    * @param[in] lhs бинарное изображение
    * @throw invalid_argument если тип изображения, не соответстует описанию
    */
    void set_image(cv::Mat lhs);

    /**
     * @brief setter: структурный элемент для переднего плана
     * @param[in] lhs бинарное изображение
     * @throw invalid_argument если тип изображения, не соответстует описанию
     */
    void set_kernel_foreground(cv::Mat lhs);

    /**
    * @brief setter: структурный эелемент для заднего плана
    * @param[in] lhs бинарное изображение, такого же размера как и 
    * структурный элемент переднего плана
    * @throw invalid_argument если тип изображения, не соответстует описанию
    */
    void set_kernel_background(cv::Mat lhs);

    /**
    * @brief setter: структурный эелемент, отвечающий за выделение при попадании
    * @param[in] lhs бинарное изображение, такого же размера как и 
    * структурный элемент переднего плана
    * @throw invalid_argument если тип изображения, не соответстует описанию
    */
    void set_hit_highlight(cv::Mat lhs);

    /**
    * @brief getter: изображение для обработки
    * @return сыллка на константу изображение для обработки
    */
    const cv::Mat& get_image() const { return image_; }

    /**
    * @brief getter: структурный элемент для переднего плана
    * @return сыллка на константу структурного элемента для переднего плана
    */
    const cv::Mat& get_kernel_foreground() const { return kernel_foreground_; }

    /**
    * @brief getter: структурный элемент для заднего плана
    * @return сыллка на константу структурного элемента для пзаднего плана
    */
    const cv::Mat& get_kernel_background() const { return kernel_background_; }
    
    /**
    * @brief getter: структурный элемент, отвечающий за выделение при попадании
    * @return сыллка на константу структурного элемена, отвечающего за выделение при попадании
    */
    const cv::Mat& get_hit_highlight() const { return hit_highlight_; }

    /**
    * @brief Метод обрабатывающий изображение алгоритмом Hit or Miss
    * @return обработанное бинарное изображение
    * @throw invalid_argument если размеры изображений не соответствуют описанию
    */
    cv::Mat DoHitOrMiss() const;

    /**
    * @brief Метод извлечения границ объектов из изображения
    * с помощью алгоритма Hit or Miss
    * @return извлеченные границы на исходном бинарном изображении
    * @throw invalid_argument если размеры изображений не соответствуют описанию
    */
    cv::Mat DoBoundaryExtraction() const;
    

private:
    // Проверка типа изображения, а также бинаризация
    cv::Mat TypeCheck(cv::Mat lhs) const; 

    // Проверка на соответствие размеров изображений
    void SizeCheck(const cv::Mat& origin, const cv::Mat& check) const; 

    // Проход по изображению структурным элементом 
    //(при foreground=true - переднего плана, иначе заднего), при Hit отметить место попадания
    cv::Mat MaskMatching(const bool& foreground) const; 

    // Сравнение двух изображений как множеств с помощью оператора and (где черный пиксель логически 1, а белый 0)
    cv::Mat AndOperation(const cv::Mat& lhs, const cv::Mat& rhs) const; 

    // Сравнение двух изображений как множеств с помощью оператора or (где черный пиксель логически 1, а белый 0)
    cv::Mat OrOperation(const cv::Mat& lhs, const cv::Mat& rhs) const;

    // Вычитание из первого изображения второго, как множества (где черный пиксель логически 1, а белый 0)
    cv::Mat SubstractionOperation(const cv::Mat& lhs, const cv::Mat& rhs) const;

private:
    // изображение для обработки
    cv::Mat image_;
    // структурный элемент для переднего плана
    cv::Mat kernel_foreground_; 
    // структурный элемент для заднего плана
    cv::Mat kernel_background_; 
    // структурный элемент, отвечающий за выделение при попадании
    cv::Mat hit_highlight_; 

private:
    const int kWhite = 255; // код белого пикселя
    const int kBlack = 0; // код черного пкселя
    const int kDefaulHitHighlight = 1; // размер структурного элемента по выделению (по умолчанию)
    const int kDefaulKernelForeground = 3; // размер структурного элемента (по умолчанию)
    const int kDefaulKernelBackground = 1; // размер структурного элемента (по умолчанию)
    const int kThresholdValue = 127; // пороговое значение бинаризации (по умолчанию)
    const int kDefaultImageDimension = 200; // размер изображения для обработки (по умочанию)
};

#endif