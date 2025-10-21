#ifndef TOOLS_H
#define TOOLS_H

#include <string>

class Tools {
    public:
        // Retorna o caminho completo a partir de um caminho relativo
        static std::string getPath(const std::string& relativePath);
};

#endif // TOOLS_H
