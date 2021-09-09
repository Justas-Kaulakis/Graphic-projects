#ifndef UTILITY_HPP_INCLUDED
#define UTILITY_HPP_INCLUDED

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

std::string getJoinedShader(const std::string Path) {
        stringstream ss;

        ifstream mainSrc(Path);
        string line;

        while(getline(mainSrc, line)) {
            stringstream buff;
            buff << line;
            string key;

            buff >> key;



            if(key == "#extra") {
                string extraPath;
                buff >> extraPath;


                if(extraPath.size() > 5 && extraPath.substr(extraPath.size() - 5, 5) == ".glsl") {
                    ifstream extraFile(extraPath);
                    ss << extraFile.rdbuf();
                    extraFile.close();
                }
                else cout << "Err.: Shader: " << extraPath << " is not in .glsl format" << endl;


            }
            else {
                ss << line << endl;
            }
        }


        return ss.str();
    }

#endif // UTILITY_HPP_INCLUDED
