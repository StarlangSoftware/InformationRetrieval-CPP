//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <cmath>
#include "VectorSpaceModel.h"

VectorSpaceModel::VectorSpaceModel(int *termFrequencies, int *documentFrequencies, int documentSize,
                                   TermWeighting termWeighting, DocumentWeighting documentWeighting) {
    double sum = 0;
    this->documentSize = documentSize;
    model = new double[documentSize];
    for (int i = 0; i < documentSize; i++){
        model[i] = weighting(termFrequencies[i], documentFrequencies[i], documentSize, termWeighting, documentWeighting);
        sum += model[i] * model[i];
    }
    for (int i = 0; i < documentSize; i++){
        model[i] /= sqrt(sum);
    }
}

double VectorSpaceModel::get(int index) {
    return model[index];
}

double VectorSpaceModel::cosineSimilarity(VectorSpaceModel secondModel) {
    double sum = 0.0;
    for (int i = 0; i < documentSize; i++){
        sum += model[i] * secondModel.model[i];
    }
    return sum;
}

double VectorSpaceModel::weighting(double termFrequency, double documentFrequency, int documentSize,
                                   TermWeighting termWeighting, DocumentWeighting documentWeighting) {
    double multiplier1 = 1, multiplier2 = 1;
    switch (termWeighting){
        case   TermWeighting::NATURAL:
            multiplier1 = termFrequency;
            break;
        case TermWeighting::LOGARITHM:
            if (termFrequency > 0)
                multiplier1 = 1 + log(termFrequency);
            else
                multiplier1 = 0;
            break;
        case     TermWeighting::BOOLE:
            if (termFrequency > 0){
                multiplier1 = 1;
            } else {
                multiplier1 = 0;
            }
    }
    switch (documentWeighting){
        case   DocumentWeighting::NO_IDF:
            multiplier2 = 1;
            break;
        case 	   DocumentWeighting::IDF:
            multiplier2 = log(documentSize / (documentFrequency + 0.0));
            break;
        case DocumentWeighting::PROBABILISTIC_IDF:
            if (documentSize > 2 * documentFrequency){
                multiplier2 = log((documentSize - documentFrequency) / (documentFrequency + 0.0));
            } else {
                multiplier2 = 0.0;
            }
            break;
    }
    return multiplier1 * multiplier2;
}

VectorSpaceModel::VectorSpaceModel() = default;
