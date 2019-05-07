src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)
CXX = g++

main: $(obj)
	$(CXX) $(obj) -v -o vgg-face-api -framework Python
%.o: %.cpp
	$(CXX) -c -v $<
clean: clean_python 
	rm -f $(obj) vgg-face-api
clean_python:
	rm -f $(wildcard *.pyc)
