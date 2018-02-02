# solanogadea (Miguel Solano Gadea) @solanogadea
TEMPLATE = app
TARGET = c2gDiCoTAE_x

QT += qml quick core network
CONFIG += c++11

VERSION = 1.0.6.1
QMAKE_TARGET_COMPANY = TipeSoft
QMAKE_TARGET_PRODUCT = DiCoTAE
QMAKE_TARGET_DESCRIPTION = "Diccionario de la Administracion Electronica"
QMAKE_TARGET_COPYRIGHT = "Todos los derechos reservados"

HEADERS += \
    cpp/applicationui.hpp \
    cpp/uiconstants.hpp \
    cpp/gen/SettingsData.hpp \ 
    cpp/gen/Bookmark.hpp \
    cpp/gen/Dictionary.hpp \
    cpp/gen/Repository.hpp \
    cpp/gen/DataManager.hpp \
    cpp/gen/Day.hpp \
    cpp/gen/Favorite.hpp \
    cpp/gen/Category.hpp \
    cpp/gen/PersonsAPI.hpp \
    cpp/gen/Section.hpp \
    cpp/gen/DictionaryItem.hpp \
    cpp/gen/Key.hpp \
    cpp/gen/KeyAPI.hpp \
    cpp/gen/KeyTag.hpp \
    cpp/gen/Creator.hpp \
    cpp/gen/CreatorAPI.hpp \
    cpp/gen/CreatorImage.hpp \
    cpp/datautil.hpp \
    cpp/imageloader.hpp \
    cpp/gen/KeyLink.hpp \
    cpp/gen/KeyLinkAPI.hpp \
    cpp/gen/KeyLists.hpp \
    cpp/dataserver.hpp \
    cpp/gen/KeyContentBlock.hpp \
    cpp/gen/KeyContentStandardType.hpp \
    cpp/clipboardproxy.hpp

SOURCES += cpp/main.cpp \
    cpp/applicationui.cpp \
    cpp/gen/SettingsData.cpp \ 
    cpp/gen/Bookmark.cpp \
    cpp/gen/Dictionary.cpp \
    cpp/gen/Repository.cpp \
    cpp/gen/DataManager.cpp \
    cpp/gen/Day.cpp \
    cpp/gen/Favorite.cpp \
    cpp/gen/Category.cpp \
    cpp/gen/PersonsAPI.cpp \
    cpp/gen/Section.cpp \
    cpp/gen/DictionaryItem.cpp \
    cpp/gen/Key.cpp \
    cpp/gen/KeyAPI.cpp \
    cpp/gen/KeyTag.cpp \
    cpp/gen/Creator.cpp \
    cpp/gen/CreatorAPI.cpp \
    cpp/gen/CreatorImage.cpp \
    cpp/datautil.cpp \
    cpp/imageloader.cpp \
    cpp/gen/KeyLink.cpp \
    cpp/gen/KeyLinkAPI.cpp \
    cpp/gen/KeyLists.cpp \
    cpp/gen/KeyContentBlock.cpp \
    cpp/dataserver.cpp \
    cpp/gen/KeyContentStandardType.cpp \
    cpp/clipboardproxy.cpp

lupdate_only {
    SOURCES +=  qml/main.qml \
    qml/common/*.qml \
    qml/navigation/*.qml \
    qml/pages/*.qml \
    qml/popups/*.qml \
    qml/tabs/*.qml
}

OTHER_FILES += images/black/*.png \
    images/black/x18/*.png \
    images/black/x36/*.png \
    images/black/x48/*.png \
    images/white/*.png \
    images/white/x18/*.png \
    images/white/x36/*.png \
    images/white/x48/*.png \
    images/extra/*.png \
    images/extra/*.jpg \
    translations/*.* \
    data-assets/*.json \
    data-assets/prod/*.json \
    data-assets/test/*.json \
    data-assets/repository/*.json \
    data-assets/repository/creatorImages/*.* \
    data-assets/repository/categoryplan/*.png \
    images/LICENSE \
    LICENSE \
    *.md

RESOURCES += qml.qrc \
    translations.qrc \
    images.qrc \
    data-assets.qrc


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

# T R A N S L A T I O N S

# if languages are added:
# 1. rebuild project to generate *.qm
# 2. add existing .qm files to translations.qrc

# if changes to translatable strings:
# 1. Run Tools-External-Linguist-Update
# 2. Run Linguist and do translations
# 3. Build and run on iOS and Android to verify translations
# 4. Optional: if translations not done: Run Tools-External-Linguist-Release

# Supported languages
LANGUAGES = es en

# used to create .ts files
defineReplace(prependAll) {
    for(a,$$1):result += $$2$${a}$$3
    return($$result)
}

# Available translations
tsroot = $$join(TARGET,,,.ts)
tstarget = $$join(TARGET,,,_)
TRANSLATIONS = $$PWD/translations/$$tsroot
TRANSLATIONS += $$prependAll(LANGUAGES, $$PWD/translations/$$tstarget, .ts)

# run LRELEASE to generate the qm files
qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
    command = $$LRELEASE $$tsfile
    system($$command)|error("Failed to run: $$command")
}

DISTFILES += \
    gen-model/README.md \
    gen-model/*.pdf \
    gen-model/*.txt \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    winrt/*.appxmanifest \
    winrt/assets/*.png \
    data-assets/prod/cacheKeyContentStandardType.json \
    ios/LaunchScreen.xib

# android (Android Marketplace)
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
# you must provide openssl libs
include(android-openssl.pri)

# Windows 7 (Win32 config)
# ico for win32
RC_ICONS = images/icons/c2gDiCoTAE_xWe_icon.ico

# ios (iOS Marketplace)
ios {
    # framework needed for Reachability classes
    LIBS += -framework SystemConfiguration
    # Reachability to get reliable online state
    SOURCES += ios/src/Reachability.mm \
    ios/src/ReachabilityListener.mm

    QMAKE_INFO_PLIST = ios/Info.plist
    ios_icon.files = $$files($$PWD/ios/Icon*.png)
    QMAKE_BUNDLE_DATA += ios_icon
    ios_artwork.files = $$files($$PWD/ios/iTunesArtwork*.png)
    QMAKE_BUNDLE_DATA += ios_artwork
    app_launch_screen.files = $$files($$PWD/ios/LaunchScreen.xib)
    QMAKE_BUNDLE_DATA += app_launch_screen

    QMAKE_IOS_DEPLOYMENT_TARGET = 8.2

    disable_warning.name = GCC_WARN_64_TO_32_BIT_CONVERSION
    disable_warning.value = NO
    QMAKE_MAC_XCODE_SETTINGS += disable_warning

    # QtCreator 4.3 provides an easy way to select the development team
    # see Project - Build - iOS Settings
    # I have to deal with different development teams,
    # so I include my signature here
    # ios_signature.pri not part of project repo because of private signature details
    # contains:
    # QMAKE_XCODE_CODE_SIGN_IDENTITY = "iPhone Developer"
    # MY_DEVELOPMENT_TEAM.name = DEVELOPMENT_TEAM
    # MY_DEVELOPMENT_TEAM.value = your team Id from Apple Developer Account
    # QMAKE_MAC_XCODE_SETTINGS += MY_DEVELOPMENT_TEAM
    # include(ios_signature.pri)

    # Note for devices: 1=iPhone, 2=iPad, 1,2=Universal.
    QMAKE_APPLE_TARGETED_DEVICE_FAMILY = 1,2
}

# mac (MacOSX Marketplace)
mac {
    CONFIG += app_bundle
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10
}

# winrt (Windows 10 Marketplace)
winrt {
    # This performs basic variable replacement on the contents of the WinRT manifest template, as
    # specified by WINRT_MANIFEST. The resulting manifest file is written to the output directory.
    # While the most common options are covered by the default template, the developer is expected
    # to make an application-level copy of the template in order to customize the manifest further.
    # Afterwards, they can override the default template by assigning their template to WINRT_MANIFEST.
    #
    # All subkeys in WINRT_MANIFEST will be replaced if defined/found, so new variables can be easily
    # added. The following keys have default values and are present in the default templates:
    # WINRT_MANIFEST: The name of the input manifest file. Defaults to a file defined by the mkspec.
    # WINRT_MANIFEST.target: The name of the target (.exe). Defaults to TARGET.
    # WINRT_MANIFEST.identity: The unique ID of the app. Defaults to reusing the existing generated manifest's UUID, or generates a new UUID if none is present.
    # WINRT_MANIFEST.name: The name of the package as displayed to the user. Defaults to TARGET.
    # WINRT_MANIFEST.architecture: The target architecture. Defaults to VCPROJ_ARCH.
    # WINRT_MANIFEST.version: The version number of the package. Defaults to "1.0.0.0".
    # WINRT_MANIFEST.arguments: Allows arguments to be passed to the executable.
    # WINRT_MANIFEST.publisher: Display name of the publisher. Defaults to "Default publisher display name".
    # WINRT_MANIFEST.publisher_id: On Windows 8/RT, the publisher's distinguished name (default: CN=MyCN). On Windows Phone, the publisher's UUID (default: invalid UUID string).
    # WINRT_MANIFEST.description: Package description. Defaults to "Default package description".
    # WINRT_MANIFEST.author: Package author (Windows Phone only). Defaults to "Default package author".
    # WINRT_MANIFEST.genre: Package genre (Windows Phone only). Defaults to "apps.normal".
    # WINRT_MANIFEST.background: Tile background color. Defaults to "green".
    # WINRT_MANIFEST.foreground: Tile foreground (text) color (Windows 8/RT only). Defaults to "light".
    # WINRT_MANIFEST.logo_store: Logo image file for Windows Store. Default provided by the mkspec.
    # WINRT_MANIFEST.logo_small: Small logo image file. Default provided by the mkspec.
    # WINRT_MANIFEST.logo_medium: Medium logo image file. Default provided by the mkspec.
    # WINRT_MANIFEST.logo_large: Large logo image file. Default provided by the mkspec.
    # WINRT_MANIFEST.splash_screen: Splash screen image file. Default provided by the mkspec.
    # WINRT_MANIFEST.iconic_tile_icon: Image file for the "iconic" tile template icon. Default provided by the mkspec.
    # WINRT_MANIFEST.iconic_tile_small: Image file for the small "iconic" tile template logo. Default provided by the mkspec.
    # WINRT_MANIFEST.capabilities: Specifies capabilities to add to the capability list.
    # WINRT_MANIFEST.dependencies: Specifies dependencies required by the package.

    # WINRT_MANIFEST = winrt/AppxManifest.in
    # Experimental: Add custom deployment step in Qt Creator.
    #   Command: %{sourceDir}\winrt\CreateAppxManifestMap.cmd
    #   Arguments: %{sourceDir} %{buildDir} %{Env:QTDIR}


    WINRT_MANIFEST.name = DiCoTAE
    WINRT_MANIFEST.description = "Diccionario de la Administracion Electronica"
    WINRT_MANIFEST.background = white
    WINRT_MANIFEST.default_language = es
    WINRT_MANIFEST.version = 1.0.6.1

    WINRT_MANIFEST.publisher = Tipesoft-MSG
    WINRT_MANIFEST.publisher_id = CN=FF4C90D8-EB22-4F1F-87BD-44E2CEA60DDB

    WINRT_MANIFEST.identity = "3224DiccionariodeConcepto.25442D97BEF97"
    WINRT_MANIFEST.phone_product_id = "4582bfec-05b4-4050-8b69-d3a4867b26b2"

    WINRT_MANIFEST.logo_store = assets\DiCoTAEAppx.50x50.png
    WINRT_MANIFEST.logo_44x44 = assets\DiCoTAEAppx.44x44.png
    # WINRT_MANIFEST.logo_71x71 = winrt/assets/Square71x71Logo.png
    WINRT_MANIFEST.logo_150x150 = assets\DiCoTAEAppx.150x150.png
    # WINRT_MANIFEST.logo_480x800 = winrt/assets/SplashScreen.png
    WINRT_MANIFEST.logo_620x300 = assets\DiCoTAEAppx.620x300.png

    # WINRT_MANIFEST.background = $${LITERAL_HASH}00a2ff
    # WINRT_MANIFEST.rotation_preference = portrait

    # WINRT_MANIFEST.logo_small = winrt/assets/SmallLogo.png
    # WINRT_MANIFEST.logo_large = winrt/assets/Logo.png
    # WINRT_MANIFEST.splash_screen = winrt/assets/SplashScreen.png

    # Cambiar a myPackageMobile si es ARM
    # Cambiar a myPackageWindows si es x86 o x64
    WINRT_MANIFEST = winrt/myPackageMobile.appxmanifest
    # WINRT_MANIFEST = winrt/myPackageWindows.appxmanifest

    # Default - 10.0.16299.0
    WINRT_MANIFEST.minVersion = 10.0.16299.0
    WINRT_MANIFEST.maxVersionTested = 10.0.16299.0

    CONFIG += windeployqt
}
