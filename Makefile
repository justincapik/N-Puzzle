NAME = N-Puzzle

SRC		=	PRQPuzzle.class.cpp\
			PRQSolver.class.cpp\
			Visual.class.cpp\
			BinaryTree.class.cpp\
			main.cpp

SRCDIR = srcs
SRC_E = $(addprefix $(SRCDIR)/, $(SRC))
FLAGS = -Wall -Wextra -Werror
CC = clang++
OBJ = $(SRC:.cpp=.o)
INCPATH = incs

LIB = -lncurses

all: $(NAME)

$(NAME): $(SRC_E)
	$(CC) -I $(INCPATH) -c $(FLAGS) $(SRC_E)
	$(CC) -o $(NAME) $(OBJ) $(LIB)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
