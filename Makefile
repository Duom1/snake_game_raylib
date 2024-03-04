CFLAGS += -Wall -Wextra
LDFLAGS += -lraylib -lm
ifeq ($(OS),Windows_NT)
	LDFLAGS += -mwindows
endif
SOURCES = main.c
OBJS = $(SOURCES:.c=.o)
NAME = snake-game

default: release

release: CFLAGS += -DNDEBUG -O3
release: $(NAME)

debug: CFLAGS += -g
debug: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(NAME) $(OBJS)
