all: test
CXX = g++

libfilecleaner.so: filecleaner.cpp
	$(CXX) filecleaner.cpp -fPIC -shared -g  -o $@
	
libfilecleaner.dll: filecleaner.cpp
	$(CXX) filecleaner.cpp -fPIC -shared  -g  -o $@ -DWINDOWS
	
test: libfilecleaner.so test.cpp
	$(CXX) -L./ -I./ -Wl,-rpath=./ test.cpp  -g -o $@ -lfilecleaner

test_win: libfilecleaner.dll test.cpp
	$(CXX) -L./ -I./ -Wl,-rpath=./ test.cpp  -g -o $@ -lfilecleaner
