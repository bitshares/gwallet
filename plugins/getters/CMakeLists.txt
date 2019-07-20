find_package(wxWidgets COMPONENTS core base adv xrc aui REQUIRED)
include(${wxWidgets_USE_FILE})

add_library( gwallet_getters
        getcommitteemember.cpp
        getaccounthistory.cpp
        getorderbook.cpp
        about.cpp
        info.cpp
        active_committee.cpp
        active_witnesses.cpp
        getwitness.cpp
        getaccount.cpp
        getasset.cpp
        gethtlc.cpp

        )

target_link_libraries( gwallet_getters
        PRIVATE graphene_app graphene_net graphene_chain graphene_egenesis_brief graphene_utilities graphene_wallet fc
        ${wxWidgets_LIBRARIES} ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS}
        )