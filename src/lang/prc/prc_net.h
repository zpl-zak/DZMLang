
// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

#if !defined(PRC_NET_H)

def_proc(is_socket)
{
     return(is_socket(pair_get_a(Args)) ? True : False);
}

def_proc(make_socket)
{
     OBJECT *Obj = make_socket();

     if(Obj->uData.SOCKET.SocketId < 0)
     {
          LOG(ERR_WARN, "Could not create socket.");
          return(Nil);
     }
     return(Obj);
}

def_proc(connect)
{
     struct hostent *server;
     struct sockaddr_in serv_addr;
     int portno = pair_get_a(pair_get_b(pair_get_b(Args)))->uData.FIXNUM.Value;

     server = gethostbyname((char *)(pair_get_a(pair_get_b(Args))->uData.STRING.Value));
     bzero((char *)&serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     bcopy((char *)server->h_addr,
           (char *)&serv_addr.sin_addr.s_addr,
           server->h_length);
     serv_addr.sin_port = htons(portno);

     if(connect(pair_get_a(Args)->uData.SOCKET.SocketId,
                (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) < 0)
     {
          return(False);
     }
     return(True);
}

def_proc(socket_write)
{
     OBJECT *Send = pair_get_a(pair_get_b(Args));
     OBJECT *Sock = pair_get_a(Args);

     int n = write(Sock->uData.SOCKET.SocketId, (char *)Send, sizeof(OBJECT));
     return(make_fixnum((s64)n));
}


def_proc(socket_read)
{
     OBJECT *Sock = pair_get_a(Args);
     OBJECT *Recv = alloc_object();

     int n = read(Sock->uData.SOCKET.SocketId, (char *)Recv, sizeof(OBJECT));
     return(make_pair(Recv, make_fixnum((s64)n)));
}

def_proc(socket_close)
{
     OBJECT *Sock = pair_get_a(Args);

     close(Sock->uData.SOCKET.SocketId);
     return(OKSymbol);
}

def_proc(socket_read_raw)
{
     OBJECT *Sock = pair_get_a(Args);
     OBJECT *Size = pair_get_a(pair_get_b(Args));

     TEMP_MEMORY BufMemory = begin_temp(&StringArena);

     int n = read(Sock->uData.SOCKET.SocketId, (char *)(StringArena.Base),
                  (int)(Size->uData.FIXNUM.Value));

     StringArena.Base[n] = 0; //NOTE(zaklaus): You have to trim the string afterwards, we won't do it implicitly here, as we don't know what you are expecting from the output.
     end_temp(BufMemory);

     return(make_pair(make_string(StringArena.Base), make_fixnum((s64)n)));
}

def_proc(socket_write_raw)
{
     OBJECT *Sock = pair_get_a(Args);
     OBJECT *Size = pair_get_a(pair_get_b(Args));
     OBJECT *Base = pair_get_a(pair_get_b(pair_get_b(Args)));

     int n = write(Sock->uData.SOCKET.SocketId, (char *)(Base->uData.STRING.Value),
                  (int)(Size->uData.FIXNUM.Value));

     return(make_fixnum((s64)n));
}
static inline void
install_net_module(OBJECT *Env)
{
     /* TODO(zaklaus):
       make-socket ()
       connect (socket, ip, port)
       write (socket, object)
       write-raw (socket, string, size) '<== how about binary data?'
       close (socket)
       read (socket)
       read-raw ???
      */

     add_procedure("make-socket", make_socket_proc);
     add_procedure("socket?", is_socket_proc);
     add_procedure("socket-connect", connect_proc);
     add_procedure("socket-write", socket_write_proc);
     add_procedure("socket-read", socket_read_proc);
     add_procedure("socket-close", socket_close_proc);
     add_procedure("socket-read-string", socket_read_raw_proc);
     add_procedure("socket-write-string", socket_write_raw_proc);
}

#define PRC_NET_H
#endif
