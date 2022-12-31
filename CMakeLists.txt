cmake_minimum_required(VERSION 3.22)
project(esmtranslationlib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")

include_directories(
        "."
        "external/openmw"
        "external/osg/include"
        "external/json/single_include"
)

set (library_files
        external/openmw/components/debug/debuglog.cpp

        external/openmw/components/esm/attr.cpp
        external/openmw/components/esm/format.cpp
        external/openmw/components/esm/refid.cpp

        external/openmw/components/esm3/aipackage.cpp
        external/openmw/components/esm3/cellid.cpp
        external/openmw/components/esm3/cellref.cpp
        external/openmw/components/esm3/effectlist.cpp
        external/openmw/components/esm3/esmreader.cpp
        external/openmw/components/esm3/esmwriter.cpp
        external/openmw/components/esm3/loadacti.cpp
        external/openmw/components/esm3/loadalch.cpp
        external/openmw/components/esm3/loadappa.cpp
        external/openmw/components/esm3/loadarmo.cpp
        external/openmw/components/esm3/loadbody.cpp
        external/openmw/components/esm3/loadbook.cpp
        external/openmw/components/esm3/loadbsgn.cpp
        external/openmw/components/esm3/loadcell.cpp
        external/openmw/components/esm3/loadclas.cpp
        external/openmw/components/esm3/loadclot.cpp
        external/openmw/components/esm3/loadcont.cpp
        external/openmw/components/esm3/loadcrea.cpp
        external/openmw/components/esm3/loaddial.cpp
        external/openmw/components/esm3/loaddoor.cpp
        external/openmw/components/esm3/loadench.cpp
        external/openmw/components/esm3/loadfact.cpp
        external/openmw/components/esm3/loadglob.cpp
        external/openmw/components/esm3/loadgmst.cpp
        external/openmw/components/esm3/loadinfo.cpp
        external/openmw/components/esm3/loadingr.cpp
        external/openmw/components/esm3/loadland.cpp
        external/openmw/components/esm3/loadlevlist.cpp
        external/openmw/components/esm3/loadligh.cpp
        external/openmw/components/esm3/loadlock.cpp
        external/openmw/components/esm3/loadltex.cpp
        external/openmw/components/esm3/loadmgef.cpp
        external/openmw/components/esm3/loadmisc.cpp
        external/openmw/components/esm3/loadnpc.cpp
        external/openmw/components/esm3/loadpgrd.cpp
        external/openmw/components/esm3/loadprob.cpp
        external/openmw/components/esm3/loadrace.cpp
        external/openmw/components/esm3/loadregn.cpp
        external/openmw/components/esm3/loadrepa.cpp
        external/openmw/components/esm3/loadscpt.cpp
        external/openmw/components/esm3/loadskil.cpp
        external/openmw/components/esm3/loadsndg.cpp
        external/openmw/components/esm3/loadsoun.cpp
        external/openmw/components/esm3/loadspel.cpp
        external/openmw/components/esm3/loadsscr.cpp
        external/openmw/components/esm3/loadstat.cpp
        external/openmw/components/esm3/loadtes3.cpp
        external/openmw/components/esm3/loadweap.cpp
        external/openmw/components/esm3/readerscache.cpp
        external/openmw/components/esm3/spelllist.cpp
        external/openmw/components/esm3/transport.cpp
        external/openmw/components/esm3/variant.cpp
        external/openmw/components/esm3/variantimp.cpp

        external/openmw/components/files/conversion.cpp
        external/openmw/components/files/openfile.cpp

        external/openmw/components/to_utf8/to_utf8.cpp

        external/openmw/apps/esmtool/labels.cpp
        external/openmw/apps/esmtool/record.cpp

        esmtranslationlib/generator.cpp
        esmtranslationlib/loader.cpp
        esmtranslationlib/textholder.cpp
        esmtranslationlib/texts.cpp
        esmtranslationlib/translationstate.cpp
        esmtranslationlib/translationrecord.cpp
        esmtranslationlib/writer.cpp
)

add_library(esmtranslationlib_static STATIC
        ${library_files}
)

add_library(esmtranslationlib SHARED
        ${library_files}
)

target_compile_options(esmtranslationlib PUBLIC)

target_compile_definitions(esmtranslationlib PUBLIC ESMTRANSLATIONLIB_EXPORT OSG_EXPORT_ OSG_EXPORT=)
target_compile_definitions(esmtranslationlib_static PUBLIC OSG_EXPORT_ OSG_EXPORT=)

#add_executable(test
#        main.cpp
#        ${library_files}
#)
#target_compile_definitions(test PUBLIC OSG_EXPORT_ OSG_EXPORT=)
