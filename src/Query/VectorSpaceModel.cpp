//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <cmath>
#include "VectorSpaceModel.h"

/**
 * Constructor for the VectorSpaceModel class. Calculates the normalized tf-idf vector of a single document.
 * @param termFrequencies Term frequencies in the document
 * @param documentFrequencies Document frequencies of terms.
 * @param documentSize Number of documents in the collection
 * @param termWeighting Term weighting scheme applied in term frequency calculation.
 * @param documentWeighting Document weighting scheme applied in document frequency calculation.
 */
VectorSpaceModel::VectorSpaceModel(const int *termFrequencies, const int *documentFrequencies, int documentSize,
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

/**
 * Returns the tf-idf value for a column at position index
 * @param index Position of the column
 * @return tf-idf value for a column at position index
 */
double VectorSpaceModel::get(int index) const{
    return model[index];
}

/**
 * Calculates the cosine similarity between this document vector and the given second document vector.
 * @param secondModel Document vector of the second document.
 * @return Cosine similarity between this document vector and the given second document vector.
 */
double VectorSpaceModel::cosineSimilarity(const VectorSpaceModel& secondModel) const{
    double sum = 0.0;
    for (int i = 0; i < documentSize; i++){
        sum += model[i] * secondModel.model[i];
    }
    return sum;
}

/**
 * Calculates tf-idf value of a single word (column) of the document vector.
 * @param termFrequency Term frequency of this word in the document
 * @param documentFrequency Document frequency of this word.
 * @param documentSize Number of documents in the collection
 * @param termWeighting Term weighting scheme applied in term frequency calculation.
 * @param documentWeighting Document weighting scheme applied in document frequency calculation.
 * @return tf-idf value of a single word (column) of the document vector.
 */
double VectorSpaceModel::weighting(double termFrequency, double documentFrequency, int documentSize,
                                   TermWeighting termWeighting, DocumentWeighting documentWeighting){
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
