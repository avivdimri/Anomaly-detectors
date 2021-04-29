#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}

void CLI::start(){
    Data* data= new Data();
    MenuCommand menu(dio,data);
    menu.execute();
}


CLI::~CLI() {
}

