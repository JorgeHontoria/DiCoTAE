# includes openssl libs onto android build
contains(ANDROID_TARGET_ARCH, x86) {
    ANDROID_EXTRA_LIBS += \
    C:/Qt/Proyectos/c2gDiCoTAE_x-master/../android-openssl-qt/prebuilt/arch-x86/libcrypto.so \
    C:/Qt/Proyectos/c2gDiCoTAE_x-master/../android-openssl-qt/prebuilt/arch-x86/libssl.so
} else {
    ANDROID_EXTRA_LIBS += \
    C:/Qt/Proyectos/c2gDiCoTAE_x-master/../android-openssl-qt/prebuilt/armeabi-v7a/libcrypto.so \
    C:/Qt/Proyectos/c2gDiCoTAE_x-master/../android-openssl-qt/prebuilt/armeabi-v7a/libssl.so
}
