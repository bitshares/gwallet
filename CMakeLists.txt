project( GWallet )
cmake_minimum_required( VERSION 2.8.12 )

include_directories(bitshares-core/libraries/chain/include)
include_directories(bitshares-core/libraries/fc/include)

add_subdirectory(bitshares-core)
#add_subdirectory(bitshares-core/libraries/fc)
#add_subdirectory(bitshares-core/libraries/db)
#add_subdirectory(bitshares-core/libraries/chain)
#add_subdirectory(bitshares-core/libraries/egenesis)
#add_subdirectory(bitshares-core/libraries/net)
#add_subdirectory(bitshares-core/libraries/utilities)
#add_subdirectory(bitshares-core/libraries/app)
#add_subdirectory(bitshares-core/libraries/wallet)

add_executable( gwallet
    main.cpp gwallet.cpp bitshares.cpp

    dialogs/importkey.cpp
    dialogs/importbalance.cpp

    panels/transfer.cpp
    panels/sellasset.cpp
    panels/borrowasset.cpp
    panels/cancelorder.cpp
    panels/setproxy.cpp
    panels/getcommitteemember.cpp
    panels/getaccounthistory.cpp
    panels/getorderbook.cpp
    panels/info.cpp
    panels/wallet.cpp
    panels/cli.cpp
    panels/dashboard.cpp
    panels/commands.cpp

    panels/about.cpp
    panels/active_witnesses.cpp
    panels/active_committee.cpp
    panels/upgradeaccount.cpp
    panels/createaccountwithbrainkey.cpp
    panels/registeraccount.cpp

    wizards/registration.cpp)

find_package(wxWidgets COMPONENTS core base adv xrc aui REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries( gwallet
    PRIVATE graphene_app graphene_net graphene_chain graphene_egenesis_brief graphene_utilities graphene_wallet fc
	${wxWidgets_LIBRARIES} ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS}
)
