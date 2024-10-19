#include <ctikz/ctikz.hpp>
#include <fstream>


void CTikz::create_tikz_file(std::string filename){
    if (block_status == 1){
        end_axis();
    }
    if (block_status == 2){
        end_picture();
    }
    block_status = 0;
    writing << "\\end{document}";
    filename += ".tex";
    std::ofstream file;
    file.open(filename);
    if (file.bad()){
        throw "File not created";
    }
    file << writing.str();
    return;
}

CTikz::CTikz() noexcept{
    writing.clear();
    writing << "\\documentclass[a1paper, 10pt]{article}" << std::endl;
    writing << "\\usepackage[margin=0.25in]{geometry}" << std::endl;
    writing << "\\usepackage{tikz, pgfplots}" << std::endl;
    writing << "\\pgfplotsset{compat=1.9}" << std::endl;
    writing << "\\usepackage[T2A]{fontenc}			% кодировка" << std::endl;
    writing << "\\usepackage[utf8]{inputenc}			% кодировка исходного текста" << std::endl;
    writing << "\\usepackage[english,russian]{babel}	% локализация и переносы" << std::endl;
    writing << "\\begin{document}" << std::endl;

}

void CTikz::end_axis(){
    if (block_status == 1){
        writing << "\\end{axis}" << std::endl;
        writing << "\\end{tikzpicture}" << std::endl;
    }
    else {
        throw "Axis not started";
    }
    block_status = 0;
}

void CTikz::start_axis(AxisStyle style){
    if(block_status != 0){
        throw "Block not ended";
    }
    block_status = 1;
    writing << "\\begin{tikzpicture}[thick, scale=0.5]" << std::endl;
    writing << "\\begin{axis}[" << std::endl;
    style.write(writing);

}

void CTikz::drawFunc(const std::vector<std::pair<double, double>>& points, FunctionStyle style){
    if (block_status == 1){
        style.write(writing);
        writing << "coordinates{" << std::endl;
        for(auto & i: points){
            writing << "(" << i.first << "," << i.second << ")";
        }
        writing << std::endl << "};" << std::endl;
    }
    else{
        throw "Axis not started";
    }
}

void CTikz::drawFunc(std::string func, FunctionStyle style){
    if(block_status != 1){
        throw "Axis not started";
    }
    style.write(writing);
    if (func.empty()){
        throw "Func is empty";
    }
    // check func !!!!!
    writing << "{" << func << "};" << std::endl;

}

void CTikz::start_picture(){
    if(block_status != 0){
        throw "Block not ended";
    }
    writing << "\\begin{tikzpicture}[thick, scale=0.5]" << std::endl;
    block_status = 2;
}

void CTikz::end_picture(){
    if(block_status == 2){
        writing << "\\end{tikzpicture}" << std::endl;
        block_status = 0;
    }
    else{
        throw "Picture not started";
    }
}

void CTikz::drawCircle(double x, double y, double r, FigureStyle style){
    if(block_status != 2){
        throw "Picture not found";
    }
    style.write(writing);
    writing << "(" << x << "," << y << ")" << " circle (" << r << ");" << std::endl;
}

void CTikz::drawRectangle(double x1, double y1, double x2, double y2, FigureStyle style){
    if(block_status != 2){
        throw "Picture not found";
    }
    style.write(writing);
    writing << "(" << x1 << "," << y1 << ") rectangle (" << x2 << "," << y2 << ");" << std::endl;
}

void CTikz::drawGrid(double x1, double y1, double x2, double y2, double step, FigureStyle style){
    if(block_status != 2){
        throw "Picture not found";
    }
    style.write(writing);
    writing << "(" << x1 << "," << y1 << ") grid " << "(" << x2 << "," << y2 << ");" << std::endl;
}

void CTikz::drawLines(const std::vector<std::pair<double, double>>& point, FigureStyle style){
    if(block_status != 2){
        throw "Picture not found";
    }
    style.write(writing);
    for(int i = 0; i < point.size() - 1; ++i){
        writing << "(" << point[i].first << "," << point[i].second << ") -- ";
    }
    writing << "(" << point[point.size() - 1].first << "," << point[point.size() - 1].second << ");" << std::endl;
    return;
}

void CTikz::add_parse(){
    if(block_status != 0){
        throw "Blocks not ended";
    }
    writing << "\\newpage"<<std::endl;
}

void CTikz::add_caption(std::string s){
    writing <<std::endl;
}

void CTikz::draw_table(const std::vector< std::vector<double> > &matrix){
    if(block_status != 0){
        return;
    }
    writing << "\\begin{center}" << std::endl << "\\begin{tabular}{ |";
    for(int i_columns = 0; i_columns < matrix[0].size(); ++i_columns){
        writing << "c|";
    }
    writing << "}" << std::endl << "\t\\hline" << std::endl;
    for(int temp_row = 0; temp_row < matrix.size(); ++temp_row){
        writing << "\t";
        for(int temp_col = 0; temp_col < matrix[0].size() - 1; ++temp_col){
            writing << matrix[temp_row][temp_col] << " & ";
        }
        writing << matrix[temp_row][matrix[0].size() - 1] << "\\" << "\\" << std::endl << "\t\\hline" << std::endl;
    }
    writing << "\\end{tabular}" << std::endl << "\\end{center}" << std::endl;
    return;
}
