cc = gcc
prom = main
src = $(shell find ./ -name "*.cpp")
obj = $(src:%.cpp=%.o) 
$(prom): $(obj)
	g++ -o $(prom) $(obj) -w
%.o: %.cpp
	gcc -c $< -o $@ -std=c++11 -w
clean:
	rm -rf $(obj) $(prom)
