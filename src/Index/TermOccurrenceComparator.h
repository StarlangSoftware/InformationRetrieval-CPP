//
// Created by Olcay Taner YILDIZ on 24.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_TERMOCCURRENCECOMPARATOR_H
#define INFORMATIONRETRIEVAL_TERMOCCURRENCECOMPARATOR_H

#include "TermOccurrence.h"

struct WordComparator{
    map<string, int> comparatorMap;
/**
 * The wordComparator takes two {@link Word}s as inputs; wordA and wordB and compares their names. Returns the result of this comparison.
 *
 * @param wordA Word type input.
 * @param wordB Word type input.
 * @return the value {@code 0} if the wordA is equal to the wordB; a value less than {@code 0} if this wordA is
 * lexicographically less than wordB; and a value greater than {@code 0} if this wordA is lexicographically greater
 * than wordB.
 */
    explicit WordComparator(const map<string, int>& comparatorMap){
        this->comparatorMap = comparatorMap;
    }

    bool operator() (const Word& wordA, const Word& wordB){
        string nameA = wordA.getName();
        const char* charPtr1 = nameA.c_str();
        string nameB = wordB.getName();
        const char* charPtr2 = nameB.c_str();
        if (wordA.getName() == wordB.getName()){
            return false;
        }
        if (wordB.getName().starts_with(wordA.getName())){
            return true;
        }
        if (wordA.getName().starts_with(wordB.getName())){
            return false;
        }
        while (*charPtr1 && *charPtr2){
            string char1;
            if ((*charPtr1 & 0xC0) != 0x80){
                do{
                    char1 += *charPtr1;
                    charPtr1++;
                } while ((*charPtr1 & 0xC0) == 0x80);
            }
            string char2;
            if ((*charPtr2 & 0xC0) != 0x80){
                do{
                    char2 += *charPtr2;
                    charPtr2++;
                } while ((*charPtr2 & 0xC0) == 0x80);
            }
            if (comparatorMap.contains(char1) && comparatorMap.contains(char2)){
                int index1 = comparatorMap.at(char1), index2 = comparatorMap.at(char2);
                if (index1 < index2){
                    return true;
                } else {
                    if (index1 > index2){
                        return false;
                    }
                }
            } else {
                return true;
            }
        }
        return true;
    }
};

struct termOccurrenceComparator{

    explicit termOccurrenceComparator(){
    }

    static bool compareWord(const Word& wordA, const Word& wordB)
    {
        return wordA.getName() < wordB.getName();
    }

    /**
     * Compares two term occurrences.
     * @param termA the first term occurrence to be compared.
     * @param termB the second term occurrence to be compared.
     * @return If the term of the first term occurrence is different from the term of the second term occurrence then
     * the method returns the comparison result between those two terms lexicographically. If the term of the first term
     * occurrence is same as the term of the second term occurrence then the term occurrences are compared with respect
     * to their document ids. If the first has smaller document id, the method returns -1; if the second has smaller
     * document id, the method returns +1.  As the third comparison criteria, if also the document ids are the same,
     * the method compares term occurrences with respect to the position. If the first has smaller position, the method
     * returns -1; if the second has smaller position id, the method returns +1, and if all three features are the same,
     * the method returns 0.
     */
    bool operator() (const TermOccurrence& termA, const TermOccurrence& termB){
        if (termA.getTerm() != termB.getTerm()){
            return compareWord(termA.getTerm(),termB.getTerm());
        } else {
            if (termA.getDocId() == termB.getDocId()){
                if (termA.getPosition() == termB.getPosition()){
                    return false;
                } else {
                    if (termA.getPosition() < termB.getPosition()){
                        return true;
                    } else {
                        return false;
                    }
                }
            } else {
                if (termA.getDocId() < termB.getDocId()){
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
};

#endif //INFORMATIONRETRIEVAL_TERMOCCURRENCECOMPARATOR_H
