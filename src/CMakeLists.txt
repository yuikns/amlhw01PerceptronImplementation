# src/

# include settings
#INCLUDE_DIRECTORIES(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/lib)
#INCLUDE_DIRECTORIES(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/lib/include)
INCLUDE_DIRECTORIES(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include)

# lib settings
LINK_DIRECTORIES(${${CMAKE_PROJECT_NAME}_BINARY_DIR}/lib)



# all related files
AUX_SOURCE_DIRECTORY(. DIR_TARGET)
ADD_LIBRARY (relfiles ${DIR_TARGET})



# STEP I
# SET(SRC_LIST dsassign.cc)
ADD_EXECUTABLE(dsassign dsassign.cc)

#  executable dependencies
ADD_DEPENDENCIES(dsassign iniparser)
ADD_DEPENDENCIES(dsassign dirtrav)

#  executable target links
TARGET_LINK_LIBRARIES(dsassign relfiles)
TARGET_LINK_LIBRARIES(dsassign iniparser)
TARGET_LINK_LIBRARIES(dsassign dirtrav)

#  executable properties
SET_TARGET_PROPERTIES(dsassign PROPERTIES OUTPUT_NAME "dsassign")


# STEP II
# model generate 
ADD_EXECUTABLE(train train.cc)

#  executable dependencies
ADD_DEPENDENCIES(train iniparser)
ADD_DEPENDENCIES(train dirtrav)

#  executable target links
TARGET_LINK_LIBRARIES(train relfiles)
TARGET_LINK_LIBRARIES(train iniparser)
TARGET_LINK_LIBRARIES(train dirtrav)
TARGET_LINK_LIBRARIES(train stemmer)


#  executable properties
SET_TARGET_PROPERTIES(train PROPERTIES OUTPUT_NAME "train")




# STEP III
# model generate 
ADD_EXECUTABLE(classify test.cc)

#  executable dependencies
ADD_DEPENDENCIES(classify iniparser)
ADD_DEPENDENCIES(classify dirtrav)

#  executable target links
TARGET_LINK_LIBRARIES(classify relfiles)
TARGET_LINK_LIBRARIES(classify iniparser)
TARGET_LINK_LIBRARIES(classify dirtrav)
TARGET_LINK_LIBRARIES(classify stemmer)

#  executable properties
SET_TARGET_PROPERTIES(classify PROPERTIES OUTPUT_NAME "classify")






