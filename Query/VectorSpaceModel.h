//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_VECTORSPACEMODEL_H
#define INFORMATIONRETRIEVAL_VECTORSPACEMODEL_H


#include "../Index/TermWeighting.h"
#include "../Document/DocumentWeighting.h"

class VectorSpaceModel {
private:
    double* model;
    int documentSize;
public:
    VectorSpaceModel(int* termFrequencies, int* documentFrequencies, int documentSize, TermWeighting termWeighting, DocumentWeighting documentWeighting);
    double get(int index);
    double cosineSimilarity(VectorSpaceModel vectorSpaceModel);
    static double weighting(double termFrequency, double documentFrequency, int documentSize, TermWeighting termWeighting, DocumentWeighting documentWeighting);
};


#endif //INFORMATIONRETRIEVAL_VECTORSPACEMODEL_H
