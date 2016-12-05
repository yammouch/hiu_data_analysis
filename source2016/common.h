// common.h
#ifndef _COMMON_H
#define _COMMON_H
#include <string>
#include <vector>
#include <limits>
double updateWeight( std::vector<std::vector<double>>& vecs, 
                     std::vector<int>& lbls, 
                     std::vector<std::vector<double>>& weight );

double updateLabel( std::vector<std::vector<double>>& vecs, 
                    std::vector<int>& lbls, 
                    std::vector<std::vector<double>>& weight );

double SQError( std::vector<std::vector<double>>& vecs, 
                std::vector<int>& lbls, 
                std::vector<std::vector<double>>& weight );
#endif
