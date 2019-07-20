find_package(wxWidgets COMPONENTS core base adv xrc aui REQUIRED)
include(${wxWidgets_USE_FILE})

add_library( gwallet_governance
        voteforwitness.cpp
        )

target_link_libraries( gwallet_governance
        PRIVATE graphene_app graphene_net graphene_chain graphene_egenesis_brief graphene_utilities graphene_wallet fc
        ${wxWidgets_LIBRARIES} ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS}
        )