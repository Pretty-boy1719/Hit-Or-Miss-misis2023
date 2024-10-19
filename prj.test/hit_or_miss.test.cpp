#include<hitOrMiss/hit_or_miss.hpp>
#include<fstream>
#include<ctikz/ctikz.hpp>

void DrawPicture(CTikz& latex, const FigureStyle& style_hit, const FigureStyle& style_miss
    ,cv::Mat image, int x, int y, const double& pixel_size) {

    FigureStyle style;
    for (int temp_row = 0; temp_row < image.rows; temp_row += 1) {
        for (int temp_col = 0; temp_col < image.cols; temp_col += 1) {

            if (image.at<uchar>(temp_row, temp_col) == 0) {
                style = style_hit;
            }
            else {
                style = style_miss;
            }

            latex.drawRectangle(x + pixel_size*temp_col, y + pixel_size*(image.rows - temp_row),
                x + pixel_size*temp_col + pixel_size, y + pixel_size*(image.rows - temp_row) + pixel_size, style);
        }
    }
}

void WriteHelp() {
    std::cout << "Description of available commands.\n";
    std::cout << "-H : Show available parameters." << std::endl;
    std::cout << "The following commands should be used as a template:" << "\n   -[command]=[way/num/key]" << '\n';
    std::cout << "-A : Please specify an integer from 1 to 9 to execute a predefined test." << '\n';
    std::cout << "-K : Please specify the processing method. The <H> key corresponds to the method hit-or-miss, <B> boundary-extraction" << '\n';
    std::cout << "-I : Please specify the path to the image to be processed." << '\n';
    std::cout << "-F : Please specify the path for the foreground structural element." << '\n';
    std::cout << "-B : Please specify the path for the background structural element." << std::endl;
    std::cout << "-S : Please specify the path for the structural element responsible for highlighting in case of a match." << std::endl;
}


int main(int argc, char** argv)
{
    std::string input_auto_param{};
    std::string input_name_method{};
    std::string input_name_image{};
    std::string input_name_foreground{};
    std::string input_name_background{};
    std::string input_name_highlight{};


    bool ready_test = false;
    bool help = false;
    bool input_image = false;
    bool struct_element1 = false;
    bool struct_element2 = false;
    bool struct_element3 = false;
    bool method_hit_or_miss = true;
    

    for (int i = 1; i < argc; i++) {
        if (static_cast<std::string>(argv[i]) == "-H") {
            help = true;
        }
        else if (static_cast<std::string>(argv[i]).size() > 2) {
            std::string param = static_cast<std::string>(argv[i]);
            if (param[0] == '-' && param[2] == '=') {
                if (param[1] == 'A') {
                    ready_test = true;
                    input_auto_param = param.substr(3, param.size() - 3);
                }
                else if (param[1] == 'I') {
                    input_image = true;
                    input_name_image = param.substr(3, param.size() - 3);
                }
                else if (param[1] == 'F') {
                    struct_element1 = true;
                    input_name_foreground = param.substr(3, param.size() - 3);
                }
                else if (param[1] == 'B') {
                    struct_element2 = true;
                    input_name_foreground = param.substr(3, param.size() - 3);
                }
                else if (param[1] == 'S') {
                    struct_element2 = true;
                    input_name_highlight = param.substr(3, param.size() - 3);
                }
                else if (param[1] == 'K') {
                    input_name_method = param.substr(3, param.size() - 3);
                    if (input_name_method == "H") {
                        method_hit_or_miss = true;
                    }
                    else {
                        method_hit_or_miss = false;
                    }
                }
                else {
                    throw std::invalid_argument("invalid argument");
                }
            }
            else {
                throw std::invalid_argument("invalid argument");
            }
        }
        else {
            throw std::invalid_argument("invalid argument");
        }

    }

    if (help) {
        WriteHelp();
    }

    HitOrMiss test{};

    if (ready_test) {
        int min_test_num = 1;
        int max_test_num = 9;
        
        int test_case = std::stoi(input_auto_param);

        if (test_case<min_test_num || test_case>max_test_num) {
            printf("The number entered is not from the specified range!");
            return -1;
        }

        std::string test_path = "..//tests/test_" + std::to_string(test_case) + "/";

        std::ifstream check_image(test_path + "image.png");
        std::ifstream check_kernel_foreground(test_path + "kernel_foreground.png");
        std::ifstream check_kernel_background(test_path + "kernel_background.png");
        std::ifstream check_hit_highlight(test_path + "hit_highlight.png");
        std::ifstream check_test_type_hit(test_path + "hit_or_miss.png");
        std::ifstream check_test_type_boundary(test_path + "boundary_extraction.png");
        std::ifstream welcome_message(test_path + "README.txt");

        std::string line;
        while (std::getline(welcome_message, line))
        {
            std::cout << line << "\n";
        }

        cv::Mat image{};
        cv::Mat kernel_foreground{};
        cv::Mat kernel_background{};
        cv::Mat hit_highlight{};
        cv::Mat test_type_hit{};
        cv::Mat test_type_boundary{};

        if (check_image.good()) image = cv::imread(test_path + "image.png", cv::IMREAD_GRAYSCALE);
        if (check_kernel_foreground.good()) kernel_foreground = cv::imread(test_path + "kernel_foreground.png", cv::IMREAD_GRAYSCALE);
        if (check_kernel_background.good()) kernel_background = cv::imread(test_path + "kernel_background.png", cv::IMREAD_GRAYSCALE);
        if (check_hit_highlight.good()) hit_highlight = cv::imread(test_path + "hit_highlight.png", cv::IMREAD_GRAYSCALE);
        if (check_test_type_hit.good()) test_type_hit = cv::imread(test_path + "hit_or_miss.png", cv::IMREAD_GRAYSCALE);
        if (check_test_type_boundary.good()) test_type_boundary = cv::imread(test_path + "boundary_extraction.png", cv::IMREAD_GRAYSCALE);


        HitOrMiss temp_test;

        //сократить до сеттеров
        if (image.empty() && kernel_foreground.empty() && kernel_background.empty() && hit_highlight.empty()) {
            temp_test = HitOrMiss();
        }
        else if (!image.empty() && kernel_foreground.empty() && kernel_background.empty() && hit_highlight.empty()) {
            temp_test = HitOrMiss(image);
        }
        else if (!image.empty() && !kernel_foreground.empty() && kernel_background.empty() && hit_highlight.empty()) {
            temp_test = HitOrMiss(image, kernel_foreground);
        }
        else if (!image.empty() && !kernel_foreground.empty() && !kernel_background.empty() && hit_highlight.empty()) {
            temp_test = HitOrMiss(image, kernel_foreground, kernel_background);
        }
        else if (!image.empty() && !kernel_foreground.empty() && !kernel_background.empty() && !hit_highlight.empty()) {
            temp_test = HitOrMiss(image, kernel_foreground, kernel_background, hit_highlight);
        }
        else {
            printf("Wrong input format\n");
        }

        if (test_type_hit.empty() && test_type_boundary.empty()) {
            printf("Operation type not selected\n");
            return -1;
        }


        test = temp_test;
        if (test_type_boundary.empty()) {
            method_hit_or_miss = true;
        }
        else {
            method_hit_or_miss = false;
        }
    }
    else {
        if (!help && !input_image && !struct_element1 &&
            !struct_element2 && !struct_element3) {
            WriteHelp();
            return 0;
        }

        HitOrMiss temp_test{};
        
        if (input_image) {
            cv::Mat image = cv::imread(input_name_image, cv::IMREAD_GRAYSCALE);
            temp_test.set_image(image);
        }
        
        if (struct_element1) {
            cv::Mat kernel_foreground = cv::imread(input_name_foreground, cv::IMREAD_GRAYSCALE);
            temp_test.set_kernel_foreground(kernel_foreground);
        }
        
        if (struct_element2) {
            cv::Mat kernel_background = cv::imread(input_name_background, cv::IMREAD_GRAYSCALE);
            temp_test.set_kernel_background(kernel_background);
        }
        
        if (struct_element3) {
            cv::Mat hit_highlight = cv::imread(input_name_highlight, cv::IMREAD_GRAYSCALE);
            temp_test.set_hit_highlight(hit_highlight);
        }

        test = temp_test;
    }


    cv::Mat result_hit_or_miss = test.DoHitOrMiss();
    cv::Mat result_boundary_extraction = test.DoBoundaryExtraction();

    cv::imshow("Input Image", test.get_image());
    cv::moveWindow("Input Image", 0, 0);

    if (method_hit_or_miss) {
        cv::imshow("Result of Hit or Miss", result_hit_or_miss);
        cv::moveWindow("Result of Hit or Miss", 0, test.get_image().rows + 50);
    }

    if (!method_hit_or_miss) {
        cv::imshow("Result of Boundary Extraction", result_boundary_extraction);
        cv::moveWindow("Result of Boundary Extraction", 0, test.get_image().rows + 50);
    }

    cv::waitKey();

    CTikz latex;
    latex.start_picture();

    FigureStyle style_image_hit;
    FigureStyle style_image_miss;
    style_image_hit.color = "black";
    style_image_miss.color = "black";
    style_image_hit.fill = "black";
    style_image_miss.fill = "white";

    FigureStyle style_mask_hit;
    FigureStyle style_mask_miss;
    style_mask_hit.custom = "color=red, dashed, ultra thick, fill=black, fill opacity=0.7";
    style_mask_miss.custom = "color=red, dashed, ultra thick, fill=white, fill opacity=0.7";

    double current_x = 0;
    double current_y = 0;
    double pixel_size_image = 1;
    double pixel_size_kernel = 1;

    if (test.get_image().rows < 21) {
        pixel_size_image = 3;
        pixel_size_kernel = 5;
    }
    else if (test.get_image().rows < 31) {
        pixel_size_image = 1.5;
        pixel_size_kernel = 5;
    }
    else {
        pixel_size_image = 1;
        pixel_size_kernel = 5;
    }


    pixel_size_image = 1;
    pixel_size_kernel = 1.2;

    //вывод изначальной картинки в латех
    DrawPicture(latex, style_image_hit, style_image_miss, test.get_image(), current_x, current_y, pixel_size_image);

    current_x += pixel_size_image*(test.get_image().cols + 1.5);
    //вывод kernel_background справа от изначального изображения
    DrawPicture(latex, style_image_hit, style_image_miss, test.get_kernel_background(), current_x, current_y, pixel_size_kernel);

    //вывод kernel_foreground справа
    DrawPicture(latex, style_image_hit, style_image_miss, test.get_kernel_foreground(),
        current_x, current_y + pixel_size_kernel*(test.get_kernel_foreground().rows + 2), pixel_size_kernel);

   
    
    current_x -= pixel_size_image * (test.get_image().cols + 1.5);
    current_y -= pixel_size_image * (test.get_image().rows + 2);

   
    cv::Mat dst{ test.get_image().rows,test.get_image().cols, CV_8UC1, cv::Scalar(255) };
    int count = 0;
    const int kWhite = 255; // код белого пикселя
    const int kBlack = 0; // код черного пкселя
    cv::Mat kernel = test.get_kernel_foreground();
    bool foreground = true;

    for (int mask_row = 0; mask_row <= test.get_image().rows - kernel.rows; mask_row += 1) {
        for (int mask_col = 0; mask_col <= test.get_image().cols - kernel.cols; mask_col += 1) {
            
            if (count >= 5) break;
            if (std::abs(current_y / pixel_size_image) > 20000) break;

            foreground = true;
            kernel = test.get_kernel_foreground();
            bool hit_foreground = true; // пока не нашлось противоречий, считается, что все совпало

            //проверка всех пикселей в радиусе в покрытии структурного элемента
            for (int step_row = 0; step_row < kernel.rows; step_row += 1) {
                for (int step_col = 0; step_col < kernel.cols; step_col += 1) {

                    // текущий пиксель с изображения
                    int image_pixel = test.get_image().at<uchar>(mask_row + step_row, mask_col + step_col);
                    // текущий пиксель с структорного элемента
                    int kernel_pixel = kernel.at<uchar>(step_row, step_col);

                    //если проверяется передний план, то белые пиксели структурного элемента не имеют значения
                    if (foreground && kernel_pixel == kWhite) continue;
                    //если проверяется задний план, то чеерные пиксели структурного элемента не имеют значения
                    if (!foreground && kernel_pixel == kBlack) continue;

                    if (image_pixel != kernel_pixel) {
                        hit_foreground = false;
                        break;
                    }
                }
                if (!hit_foreground) break;
            }


            foreground = false;
            kernel = test.get_kernel_background();
            bool hit_background = true; // пока не нашлось противоречий, считается, что все совпало

            //проверка всех пикселей в радиусе в покрытии структурного элемента
            for (int step_row = 0; step_row < kernel.rows; step_row += 1) {
                for (int step_col = 0; step_col < kernel.cols; step_col += 1) {

                    // текущий пиксель с изображения
                    int image_pixel = test.get_image().at<uchar>(mask_row + step_row, mask_col + step_col);
                    // текущий пиксель с структорного элемента
                    int kernel_pixel = kernel.at<uchar>(step_row, step_col);

                    //если проверяется передний план, то белые пиксели структурного элемента не имеют значения
                    if (foreground && kernel_pixel == kWhite) continue;
                    //если проверяется задний план, то чеерные пиксели структурного элемента не имеют значения
                    if (!foreground && kernel_pixel == kBlack) continue;

                    if (image_pixel != kernel_pixel) {
                        hit_background = false;
                        break;
                    }
                }
                if (!hit_background) break;
            }

            

            //если структурный элемент совпал, то выделить пиксели в соответствии с структурным элементом,
            //отвечающим за выделение
            if (hit_foreground && hit_background) {
                if (std::abs(current_y - test.get_image().rows) > 150) {
                    latex.end_picture();
                    latex.add_parse();
                    latex.start_picture();
                }
                count += 1;
                if (test.get_hit_highlight().rows == 1 && test.get_hit_highlight().cols == 1) {
                    dst.at<uchar>(mask_row + kernel.rows / 2, mask_col + kernel.cols / 2) = kBlack;
                }
                else {
                    for (int step_row = 0; step_row < kernel.rows; step_row += 1) {
                        for (int step_col = 0; step_col < kernel.cols; step_col += 1) {

                            if (test.get_hit_highlight().at<uchar>(step_row, step_col) == kBlack)
                                dst.at<uchar>(mask_row + step_row, mask_col + step_col) = kBlack;
                        }
                    }
                }
                //картина с наложением маски
                DrawPicture(latex, style_image_hit, style_image_miss, test.get_image(), current_x, current_y, pixel_size_image);
                DrawPicture(latex, style_mask_hit, style_mask_miss, test.get_kernel_foreground(),
                    current_x + pixel_size_image * (mask_col),
                    current_y + pixel_size_image * (test.get_image().rows - mask_row - test.get_kernel_foreground().rows), pixel_size_image);

                //картинка с выделением hit
                current_x += pixel_size_image * (test.get_image().cols + 10);
                DrawPicture(latex, style_image_hit, style_image_miss, dst, current_x, current_y, pixel_size_image);

                current_x-= pixel_size_image * (test.get_image().cols + 10);
                current_y -= pixel_size_image * (test.get_image().rows + 2);
            }
            kernel = test.get_kernel_foreground();
        }
        if (count >= 3) break;
    }

    //конечная
    if (method_hit_or_miss)
        DrawPicture(latex, style_image_hit, style_image_miss, result_hit_or_miss, current_x, current_y, pixel_size_image);
    else 
        DrawPicture(latex, style_image_hit, style_image_miss, result_boundary_extraction, current_x, current_y, pixel_size_image);

    if (method_hit_or_miss)
        cv::imwrite("output_image.png", result_hit_or_miss);
    else
        cv::imwrite("output_image.png", result_boundary_extraction);
    
    latex.end_picture();
    latex.create_tikz_file("visualization");
 
    
}