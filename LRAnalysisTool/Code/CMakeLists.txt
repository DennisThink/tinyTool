cmake_minimum_required(VERSION 3.0)
SET(SOURCE_FILE main.cpp
                ActionTable.h ActionTable.cpp 
                AnalysisStack.h AnalysisStack.cpp 
                CommonData.h 
                FileManager.h FileManager.cpp 
                Formular.h Formular.cpp 
                FormularSet.h FormularSet.cpp 
                GotoTable.h GotoTable.cpp 
                Item.h Item.cpp 
                ItemSet.h ItemSet.cpp 
                ItemSetVector.h ItemSetVector.cpp 
                LabelSet.h LabelSet.cpp 
                Manager.h Manager.cpp 
                PreManager.h PreManager.cpp 
                Relation.h Relation.cpp 
                RelationSet.h RelationSet.cpp 
                SourceCodeStream.h SourceCodeStream.cpp)

add_executable(LRDemo ${SOURCE_FILE})