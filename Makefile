NAME = OpenGLcub3D
SRC = main.cxx
OBJ_DIR = obj
OBJ = $(patsubst %.cxx, $(OBJ_DIR)/%.o, $(SRC))

CXX = c++
CXXFLAGS = -std=c++11 -DGL_SILENCE_DEPRECATION
LDFLAGS = -framework OpenGL -framework GLUT

all: $(NAME)

$(OBJ_DIR)/%.o: %.cxx
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR)
	@echo "Object files cleaned"

fclean: clean
	rm -f $(NAME)
	@echo "Executable removed: $(NAME)"

re: fclean all
