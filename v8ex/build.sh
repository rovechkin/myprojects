V8=../v8
g++ -I. -I$V8/include $1.cpp -o $1 -Wl,--start-group \
$V8/out.gn/x64.release/obj/{libv8_{base,libbase,external_snapshot,libplatform,libsampler},\
third_party/icu/libicu{uc,i18n},src/inspector/libinspector}.a \
-Wl,--end-group -lrt -ldl -pthread -std=c++0x
