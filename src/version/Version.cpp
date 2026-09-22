#include "../VersionManager.h"

#include <iostream>

std::string Version::GetVer() {
    // Find path
    const char* path = "assets/version.txt";
    std::string versionTXT = "err"; // Declarar variable
    
    // Cargar el archivo
    std::ifstream file(path, std::ios::in);

    // Abre el archivo
    if (file.is_open()) {
        std::getline(file, versionTXT);
        
        // Cierra el archivo 
        file.close();

    } else {std::cout << "Error al abrir archivo";}
    
    return versionTXT;
}