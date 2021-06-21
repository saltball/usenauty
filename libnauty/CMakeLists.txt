project("libnauty" VERSION 1.2.3)

include_directories("${CMAKE_SOURCE_DIR}/include")



# Set some compiler options
SET(SMALL "-DMAXN=WORDSIZE")
SET(S1 "S1=-DMAXN=WORDSIZE -DWORDSIZE=16")
SET(W1 "-DMAXN=WORDSIZE -DWORDSIZE=32")
SET(L1 "-DMAXN=WORDSIZE -DWORDSIZE=64")
SET(S "-DWORDSIZE=16")
SET(W "-DWORDSIZE=32")
SET(L "-DWORDSIZE=64")
SET(USELIB "-DASLIB")

# Set some obj targets
# add_library(nauty OBJECT nauty.c)

# add_library(nauty1 OBJECT nauty.c)
# SET_TARGET_PROPERTIES(nauty1 PROPERTIES COMPILE_FLAGS "${SMALL}")
# add_library(nautyS OBJECT nauty.c)
# SET_TARGET_PROPERTIES(nautyS PROPERTIES COMPILE_FLAGS "${S}")
# add_library(nautyS1 OBJECT nauty.c)
# SET_TARGET_PROPERTIES(nautyS1 PROPERTIES COMPILE_FLAGS "${S1}")
# add_library(nautyW OBJECT nauty.c)
# SET_TARGET_PROPERTIES(nautyW PROPERTIES COMPILE_FLAGS "${W}")
# add_library(nautyW1 OBJECT nauty.c)
# SET_TARGET_PROPERTIES(nautyW1 PROPERTIES COMPILE_FLAGS "${W1}")
# add_library(nautyL OBJECT nauty.c)
# SET_TARGET_PROPERTIES(nautyL PROPERTIES COMPILE_FLAGS "${L}")
# add_library(nautyL1 OBJECT nauty.c)
# SET_TARGET_PROPERTIES(nautyL1 PROPERTIES COMPILE_FLAGS "${L1}")


# add_library(nautil OBJECT nautil.c)
# add_library(nautil1 OBJECT nautil.c)
# SET_TARGET_PROPERTIES(nautil1 PROPERTIES COMPILE_FLAGS "${SMALL}")
# add_library(nautilS OBJECT nautil.c)
# SET_TARGET_PROPERTIES(nautilS PROPERTIES COMPILE_FLAGS "${S}")
# add_library(nautilS1 OBJECT nautil.c)
# SET_TARGET_PROPERTIES(nautilS1 PROPERTIES COMPILE_FLAGS "${S1}")
# add_library(nautilW OBJECT nautil.c)
# SET_TARGET_PROPERTIES(nautilW PROPERTIES COMPILE_FLAGS "${W}")
# add_library(nautilW1 OBJECT nautil.c)
# SET_TARGET_PROPERTIES(nautilW1 PROPERTIES COMPILE_FLAGS "${W1}")
# add_library(nautilL OBJECT nautil.c)
# SET_TARGET_PROPERTIES(nautilL PROPERTIES COMPILE_FLAGS "${L}")
# add_library(nautilL1 OBJECT nautil.c)
# SET_TARGET_PROPERTIES(nautilL1 PROPERTIES COMPILE_FLAGS "${L1}")


# add_library(naugraph OBJECT naugraph.c)
# add_library(naugraph1 OBJECT naugraph.c)
# SET_TARGET_PROPERTIES(naugraph1 PROPERTIES COMPILE_FLAGS "${SMALL}")
# add_library(naugraphS OBJECT naugraph.c)
# SET_TARGET_PROPERTIES(naugraphS PROPERTIES COMPILE_FLAGS "${S}")
# add_library(naugraphS1 OBJECT naugraph.c)
# SET_TARGET_PROPERTIES(naugraphS1 PROPERTIES COMPILE_FLAGS "${S1}")
# add_library(naugraphW OBJECT naugraph.c)
# SET_TARGET_PROPERTIES(naugraphW PROPERTIES COMPILE_FLAGS "${W}")
# add_library(naugraphW1 OBJECT naugraph.c)
# SET_TARGET_PROPERTIES(naugraphW1 PROPERTIES COMPILE_FLAGS "${W1}")
# add_library(naugraphL OBJECT naugraph.c)
# SET_TARGET_PROPERTIES(naugraphL PROPERTIES COMPILE_FLAGS "${L}")
# add_library(naugraphL1 OBJECT naugraph.c)
# SET_TARGET_PROPERTIES(naugraphL1 PROPERTIES COMPILE_FLAGS "${L1}")
# add_executable(my_app my_app.cpp $<TARGET_OBJECTS:my_library>)
# add_library(my_static_lib STATIC some_other_source.cpp $<TARGET_OBJECTS:my_library>)


FILE(GLOB NAUTY nauty.c nautil.c nausparse.c naugraph.c schreier.c naurng.c)
FILE(GLOB NAUTYLIB_USE traces.c gtools.c naututil.c nautinv.c gutil1.c gutil2.c gtnauty.c naugroup.c showg.c)
FILE(GLOB ULTI times.c)



add_library(nauty ${NAUTY} ${NAUTYLIB_USE})
SET_TARGET_PROPERTIES(nauty PROPERTIES COMPILE_FLAGS "${USELIB}")
add_library(nauty1  ${NAUTY} ${NAUTYLIB_USE})
SET_TARGET_PROPERTIES(nauty1 PROPERTIES COMPILE_FLAGS "${SMALL} ${USELIB}")
add_library(nautyW  ${NAUTY} ${NAUTYLIB_USE})
SET_TARGET_PROPERTIES(nautyW PROPERTIES COMPILE_FLAGS "${W} ${USELIB}")
add_library(nautyW1   ${NAUTY} ${NAUTYLIB_USE})
SET_TARGET_PROPERTIES(nautyW1 PROPERTIES COMPILE_FLAGS "${W1} ${USELIB}")
add_library(nautyL  ${NAUTY} ${NAUTYLIB_USE})
SET_TARGET_PROPERTIES(nautyL PROPERTIES COMPILE_FLAGS "${L} ${USELIB}")
add_library(nautyL1  ${NAUTY} ${NAUTYLIB_USE})
SET_TARGET_PROPERTIES(nautyL1 PROPERTIES COMPILE_FLAGS "${L1} ${USELIB}")

add_executable(vcolg vcolg.c gtnauty.c naututil.c nautinv.c gtools.c naugroup.c ${NAUTY} ${ULTI})