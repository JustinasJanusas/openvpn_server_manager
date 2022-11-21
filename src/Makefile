BIN:=openvpn_server_manager
LIBS:= -largp -lubus -lubox

.PHONY: all clean

all: $(BIN)

$(BIN): main.c socket_util.c ubus_util.c string_util.c linked_list.c
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f $(BIN)