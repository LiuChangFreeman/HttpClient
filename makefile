cc = g++
prom = main
src = $(shell find ./ -name "*.cpp")
obj = $(src:%.cpp=%.o) 
$(prom): $(obj)
	$(cc) -o $(prom) $(obj) -w
%.o: %.cpp
	$(cc) -c $< -o $@ -std=c++11 -w
clean:
	rm -rf $(obj) $(prom)