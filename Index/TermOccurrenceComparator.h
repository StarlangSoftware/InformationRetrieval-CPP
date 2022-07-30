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
    WordComparator(map<string, int> comparatorMap){
        this->comparatorMap = comparatorMap;
    }

    bool operator() (Word wordA, Word wordB){
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

    map<string, int> comparatorMap;

    termOccurrenceComparator(map<string, int> comparatorMap){
        this->comparatorMap = comparatorMap;
    }

    bool operator() (TermOccurrence termA, TermOccurrence termB){
        WordComparator wordComparator = WordComparator(Dictionary::turkishComparatorMap);
        if (termA.getTerm() != termB.getTerm()){
            return wordComparator.operator()(termA.getTerm() ,termB.getTerm());
        } else {
            if (termA.getDocId() == termB.getDocId()){
                if (termA.getPosition() == termB.getPosition()){
                    return false;
                } else {
                    if (termA.getPosition() < termB.getPosition()){
                        return false;
                    } else {
                        return true;
                    }
                }
            } else {
                if (termA.getDocId() < termB.getDocId()){
                    return false;
                } else {
                    return true;
                }
            }
        }
    }
};

#endif //INFORMATIONRETRIEVAL_TERMOCCURRENCECOMPARATOR_H
