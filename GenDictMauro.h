//
// Created by ferris on 7/12/22.
//

#ifndef DICTGENERATOR_GENDICTMAURO_H
#define DICTGENERATOR_GENDICTMAURO_H

#include "Dict.h"
#include "future"

void GenDictMauro();
void GenEntry(const string &fileString, promise<Entry*> && pro);

#endif //DICTGENERATOR_GENDICTMAURO_H