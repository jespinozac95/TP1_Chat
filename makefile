all : CHAT


CPPFLAGS = -g -I.

CHAT : Messenger.c
cc -g -I. Messenger.c -o CHAT
