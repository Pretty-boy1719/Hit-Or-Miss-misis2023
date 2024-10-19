#pragma once

#ifndef CTIKZ_HPP

#include <string>
#include <sstream>
#include <cmath>
#include <vector>

/**
* @brief Структура, которая задает стиль функций.
*/
struct FunctionStyle{
    //r();
    std::string color = ""; /**< цвет линии и точек */
    std::string mark = ""; /**< обозначение отметок графика функции (по умолчанию толстые точки) */
    std::string mark_size = ""; /**< толщина отметок */
    std::string custom="";
    bool only_marks = 0; /**< отображение только ометок */
    int samples = 200; /**< шаг значений, повышающий качество графика */

    void write(std::stringstream& writing){ /**< записывает параметры стиля в поток */
         writing << "\\addplot [" << std::endl;
        writing << "mark=" << mark << "," << std::endl;
        if(only_marks){
            writing << "only marks," << std::endl;
        }
        if(!mark_size.empty()){
            writing << "mark size=" << mark_size << "," << std::endl;
        }
        writing << "samples=" << samples << "," << std::endl;
        if(!color.empty())
            writing << "color=" << color << "]" << std::endl;
    }
};

/**
 * @brief Структура, которая задает стиль фигур.
 */
struct FigureStyle{
    std::string color; /**< цвет контура фигуры */
    std::string patterned = ""; /**< толщина линии, также можно указать числом, например 4pt (смотреть документацию) */
    std::string arrow = ""; /**< стрелка (для отрезков) */
    std::string thickness = ""; /**< стиль линии */
    std::string fill; /**< заливка (цвет) */
	std::string custom ="";
    void write(std::stringstream& writing){ /**< записывает параметры стиля в поток */
        writing << "\\draw[";
        if (!color.empty()) {
            writing << "color=" << color << ", ";
        }
        if (!fill.empty()) {
            writing << "fill=" << fill << ", ";
        }
		if(!custom.empty()){
			writing<<custom<<", ";
		}
        if (!thickness.empty()){
            writing << thickness << ", ";
        }
        if(!arrow.empty()){
            writing << arrow << ", ";
        }
        if (!patterned.empty())
            writing << patterned <<" ";
        writing << "] ";
    }

};

/**
 * @brief Структура, которая задает стиль Axis-блока (Системы координат).
 */
struct AxisStyle{
    std::string title = ""; /**< заголовок над Axis-блоком */
    std::string xlabel = ""; /**< подпись горизонтальной оси (абсцисс) */
    std::string ylabel = ""; /**< подпись вертикальной оси (ординат) */
    std::string ymin = ""; /**< минимальное значение y */
    std::string xmin = ""; /**< минимальное значение x */
    std::string xmax = ""; /**< максимальное значение x */
    std::string ymax = ""; /**< максимальное значение y */

    std::string axis_lines = "box"; /**< расположение осей относительно Axis-блока (Например left, right и др.) */
    bool grid = 0; /**< вспомогательная сетка */
    std::string grid_style = ""; /**< стиль вспомогательной сетки  */
    double scale = 1; /**< масштаб */
    bool enlarge_limits = 1; /**< расположение точек на краях Axis-блока */

    void write(std::stringstream& writing){ /**< записывает параметры стиля в поток */
        if (!title.empty()){
            writing << "title=" << title << "," << std::endl;
        }
        if (!xlabel.empty()){
            writing << "xlabel=" << xlabel << "," << std::endl;
        }
        if (!ylabel.empty()){
            writing << "ylabel=" << ylabel << "," << std::endl;
        }       
        if(!xmin.empty()){
            writing << "xmin=" << xmin << "," << std::endl;
        }
        if(!ymin.empty()){
            writing << "ymin=" << ymin << "," << std::endl;
        }
        if(!xmax.empty()){
            writing << "xmax=" << xmax << "," << std::endl;
        }
        if(!ymax.empty()){
            writing << "ymax=" << ymax << "," << std::endl;
        }
        writing << "axis lines=" << axis_lines << "," << std::endl;
        if(grid){
            writing << "grid," << std::endl;
            if(!grid_style.empty()){
                writing << "grid style=" << grid_style << "," << std::endl;
            }
        }
        writing << "scale=" << scale << "," << std::endl;
        writing << "enlargelimits=" << std::boolalpha << enlarge_limits << "]" << std::endl;
    }
};

/**
 * @brief C++ library to work with LaTeX
 */
class CTikz{ 
public:
    /**
     * @brief Construct a new CTikz::CTikz object
     * Добавляет преамбулу в документ.
     */
    CTikz() noexcept;

    /**
    * @brief Генерирует tikz файл
    * @param filename название генерируемого файла (можно указать путь)
    * @throw "File not created" файл не был создан (запустите от имени администратора или проверьте название файла)
    */
    void create_tikz_file(std::string filename);

    /**
    * @brief Блок, отвечающий за начало системы координат.
    * @param style Стиль системы координат (структура AxisStyle).
    * @throw "Block not ended" Если какой-то из блоков не был закончен.
    */
    void start_axis(AxisStyle style=AxisStyle()); // enlargelimits

    /**
    * @brief Конец блока с системой координат.
    * @throw "Axis not started" В случае, если система координат не была начата.
    */
    void end_axis();

    /**
    * @brief Добавляет функцию, которая будет нанесена на график по точкам.
    * @param points Вектор пар точек (x, y).
    * @param style Стиль функции (структура FunctionStyle).
    * @throw "Axis not started" В случае, когда система координат не была начата.
    */
    void drawFunc(const std::vector<std::pair<double, double> >& points, FunctionStyle style=FunctionStyle());

    /**
    * @brief Добавляет функцию, которая будет нанесена на график по заданному уравнению
    * @param func Уравнение функции, например x + 5
    * @param style Стиль функции (структура FunctionStyle)
    * @throw "Axis not started" В случае, когда система координат не была начата.
    */
    void drawFunc(std::string func, FunctionStyle style=FunctionStyle());

    /**
    * @brief Начинает tikzpicture-блок, отвечающий за отрисовку фигур (без осей координат).
    * @throw "Block not ended" Если какой-то из блоков не был закончен.
    */
    void start_picture();

    /**
    * @brief Блок заканчивающий tikzpicture-блок.
    * @throw "Block not ended" Если какой-то из блоков не был закончен.
    */
    void end_picture();

    /**
    * @brief Рисует окружность
    * @param x координата центра окр-ти по горизонтальной оси (абсцисс)
    * @param y координата центра окр-ти по вертикальной оси (ординат)
    * @param r радиус окружности
    * @param style стиль фигуры (структура FigureStyle)
    * @throw "Picture not found" если tikzpicture-блок не был начат.
    */
    void drawCircle(double x1, double y1, double r, FigureStyle style=FigureStyle());

    /**
    * @brief Рисует прямоугольник по двум противоположным вершинам
    * @param x1 координата левой нижней вершины по горизонтальной оси (абсцисс)
    * @param y1 координата левой нижней вершины вертикальной оси (ординат)
    * @param x2 координата правой верхней вершины по горизонтальной оси (абсцисс)
    * @param y2 координата правой верхней вершины вертикальной оси (ординат)
    * @param style стиль фигуры (структура FigureStyle)
    * @throw "Picture not found" если tikzpicture-блок не был начат.
    */
    void drawRectangle(double x1, double y1, double x2, double y2, FigureStyle style=FigureStyle());

    /**
    * @brief Рисует вспомогательную сетку
    * @param x1 координата левой нижней вершины по горизонтальной оси (абсцисс)
    * @param y1 координата левой нижней вершины вертикальной оси (ординат)
    * @param x2 координата правой верхней вершины по горизонтальной оси (абсцисс)
    * @param y2 координата правой верхней вершины вертикальной оси (ординат)
    * @param step шаг сетки
    * @param style стиль фигуры (структура FigureStyle)
    * @throw "Picture not found" если tikzpicture-блок не был начат.
    */
    void drawGrid(double x1, double y1, double x2, double y2, double step=1, FigureStyle style=FigureStyle());

    /**
    * @brief Рисует ломанные линии, заданные по коориднатам
    * @param point вектор точек формата (x, y)
    * @param style стиль фигуры (структура FigureStyle)
    * @throw "Picture not found" если tikzpicture-блок не был начат.
    */
    void drawLines(const std::vector<std::pair<double, double> >& points, FigureStyle style=FigureStyle());

    /**
    * @brief добавляет отступ между блоками, в нашем случае блоки будут располагаться друг под другом
    * @throw "Block not ended" если один из блоков не был закончен.
    */
    void add_parse();

    void add_caption(std::string);

    /**
     * @brief Отображает матрицу в документе
     * @param points вектор точек формата (x, y)
     */
    void draw_table(const std::vector<std::vector<double> >& points);


private:
    int block_status = 0;
    std::stringstream writing;
};

#endif //CTIKZ_HPP